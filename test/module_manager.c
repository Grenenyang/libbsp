#define _GNU_SOURCE
#include "init_common.h"
#include <link.h>
#include <string.h>
#include <dlfcn.h>
#include <elf.h>
#include <stdint.h>
#include <stdio.h>

// 适配32/64位
#if defined(__x86_64__) || defined(__aarch64__)
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Shdr Elf_Shdr;
#else
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Shdr Elf_Shdr;
#endif

typedef struct {
    const char *section_name;
    int init_ret;
} iterate_data_t;

// 过滤：只处理当前目录的自定义so（./开头）
static int is_custom_so(const char *so_name) {
    return (so_name && strlen(so_name) >= 2 && strncmp(so_name, "./", 2) == 0);
}

// 直接从so基地址获取有效的ELF头
static Elf_Ehdr* get_elf_hdr_from_base(uintptr_t base_addr) {
    if (base_addr == 0) return NULL;
    Elf_Ehdr *ehdr = (Elf_Ehdr*)base_addr;
    
    // 仅校验ELF魔数
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3) {
        return NULL;
    }
    return ehdr;
}

// 核心修正：获取内存中SO的字符串表（用sh_addr而非sh_offset）
static char* get_shstrtab_in_memory(uintptr_t base_addr, Elf_Ehdr *ehdr, Elf_Shdr *shdr) {
    if (!ehdr || !shdr) return NULL;
    
    // 字符串表Section头
    Elf_Shdr *shstrtab_shdr = &shdr[ehdr->e_shstrndx];
    printf("[DEBUG] 字符串表Section: sh_addr=0x%lx, sh_offset=0x%lx, sh_size=%zu\n",
           (uintptr_t)shstrtab_shdr->sh_addr,
           (uintptr_t)shstrtab_shdr->sh_offset,
           shstrtab_shdr->sh_size);
    
    // 关键修正：内存中用sh_addr（文件中用sh_offset）
    uintptr_t shstrtab_addr = base_addr + shstrtab_shdr->sh_addr;
    if (shstrtab_addr == 0 || shstrtab_shdr->sh_size == 0) {
        // 兜底：尝试文件偏移（兼容部分特殊SO）
        shstrtab_addr = base_addr + shstrtab_shdr->sh_offset;
        printf("[DEBUG] 内存地址失败，尝试文件偏移: 0x%lx\n", shstrtab_addr);
        if (shstrtab_addr == 0) return NULL;
    }
    
    return (char*)shstrtab_addr;
}

// 遍历回调：修正字符串表地址计算
static int iterate_phdr_callback(struct dl_phdr_info *info, size_t size, void *data) {
    iterate_data_t *iter_data = (iterate_data_t *)data;
    const char *target_sec = iter_data->section_name;

    // 1. 过滤系统so
    if (!is_custom_so(info->dlpi_name)) {
        printf("[DEBUG] 跳过系统SO: %s\n", info->dlpi_name ? info->dlpi_name : "主程序");
        return 0;
    }

    // 2. 自定义so：获取基地址和ELF头
    uintptr_t base_addr = (uintptr_t)info->dlpi_addr;
    Elf_Ehdr *ehdr = get_elf_hdr_from_base(base_addr);
    if (!ehdr) {
        printf("[DEBUG] %s: 基地址无效/非ELF文件，跳过\n", info->dlpi_name);
        return 0;
    }
    printf("[DEBUG] 处理自定义SO: %s (基地址: 0x%lx, ELF头: 0x%lx)\n", 
           info->dlpi_name, base_addr, (uintptr_t)ehdr);

    // 3. 检查Section头有效性
    if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0 || ehdr->e_shstrndx == SHN_UNDEF) {
        printf("[DEBUG] %s: 无有效Section头，跳过\n", info->dlpi_name);
        return 0;
    }

    // 4. 获取Section头和字符串表（核心修正）
    Elf_Shdr *shdr = (Elf_Shdr*)(base_addr + ehdr->e_shoff);
    char *shstrtab = get_shstrtab_in_memory(base_addr, ehdr, shdr);
    if (!shstrtab) {
        printf("[DEBUG] %s: 无法获取字符串表，跳过\n", info->dlpi_name);
        return 0;
    }

    // 5. 遍历Section找目标段
    for (int j = 0; j < ehdr->e_shnum; j++) {
        Elf_Shdr *s = &shdr[j];
        // 跳过空段、非数据段、无地址段
        if (s->sh_type != SHT_PROGBITS || s->sh_size == 0 || s->sh_addr == 0) continue;

        // 检查Section名称
        if (s->sh_name >= shdr[ehdr->e_shstrndx].sh_size) continue;
        char *sec_name = shstrtab + s->sh_name;
        if (!sec_name) continue;

        // 打印所有Section名称（调试用）
        printf("[DEBUG] Section[%d]: 名称=%s, 地址=0x%lx, 大小=%zu\n",
               j, sec_name, (uintptr_t)(base_addr + s->sh_addr), s->sh_size);

        // 匹配目标section
        if (strcmp(sec_name, target_sec) != 0) continue;

        // 6. 计算函数指针地址和数量
        uintptr_t sec_addr = base_addr + s->sh_addr;
        size_t fn_count = s->sh_size / sizeof(void*);
        printf("[DEBUG] %s: 找到%s段 (地址: 0x%lx, 函数数: %zu)\n", 
               info->dlpi_name, target_sec, sec_addr, fn_count);

        // 7. 执行初始化/退出函数
        if (strcmp(target_sec, ".my_initcall") == 0) {
            user_initcall_t *fn_arr = (user_initcall_t*)sec_addr;
            for (size_t k = 0; k < fn_count; k++) {
                if (!fn_arr[k]) continue;
                printf("[%s] 执行初始化函数: %p\n", info->dlpi_name, fn_arr[k]);
                int ret = fn_arr[k]();
                if (ret != 0) {
                    iter_data->init_ret = ret;
                    fprintf(stderr, "[%s] 初始化失败，错误码: %d\n", info->dlpi_name, ret);
                    return 1;
                }
            }
        } else if (strcmp(target_sec, ".my_exitcall") == 0) {
            user_exitcall_t *fn_arr = (user_exitcall_t*)sec_addr;
            for (size_t k = 0; k < fn_count; k++) {
                if (!fn_arr[k]) continue;
                printf("[%s] 执行退出函数: %p\n", info->dlpi_name, fn_arr[k]);
                fn_arr[k]();
            }
        }
    }

    return 0;
}

// 导出接口
__attribute__((visibility("default")))
int module_manager_init(void) {
    printf("===== 模块管理器开始初始化所有模块 =====\n");
    iterate_data_t data = {
        .section_name = ".my_initcall",
        .init_ret = 0
    };
    dl_iterate_phdr(iterate_phdr_callback, &data);
    if (data.init_ret == 0) {
        printf("===== 所有模块初始化完成 =====\n");
    }
    return data.init_ret;
}

__attribute__((visibility("default")))
void module_manager_exit(void) {
    printf("\n===== 模块管理器开始退出所有模块 =====\n");
    iterate_data_t data = {
        .section_name = ".my_exitcall"
    };
    dl_iterate_phdr(iterate_phdr_callback, &data);
    printf("===== 所有模块退出完成 =====\n");
}
