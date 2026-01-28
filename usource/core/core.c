#include "core.h"
#include <stddef.h>
#include "cJSON.h"
#include "autoconf.h"
#include "wt_tools_class.h"
#include "wt_tools_driver.h"
#include "export.h"
// ====================== 全局变量 ======================
pthread_mutex_t hw_module_lock = PTHREAD_MUTEX_INITIALIZER;
// ====================== Class 接口实现（修复链表初始化） ======================
int hw_class_init(hw_class_t *cls, const char *name)
{
    if (cls == NULL || name == NULL)
    {
        HW_ERR("hw_class_init: invalid param (cls=%p, name=%s)\n", cls, name);
        return -1;
    }

    // 初始化基础字段
    memset(cls, 0, sizeof(hw_class_t));
    cls->name = strdup(name);
    if (cls->name == NULL) {
        HW_ERR("hw_class_init: strdup failed (errno=%d)\n", errno);
        return -1;
    }

    // 修复：使用函数初始化链表（替代原宏）
    hw_list_head_init(&cls->drv_list);
    hw_list_head_init(&cls->dev_list);

    // 初始化锁
    if (pthread_mutex_init(&cls->lock, NULL) != 0)
    {
        HW_ERR("hw_class_init: mutex init failed (errno=%d)\n", errno);
        free(cls->name);
        return -1;
    }

    cls->dev_count = 0;
    cls->drv_count = 0;
    HW_INFO("Class %s(%p) init success\n", name,cls);
    return 0;
}

void hw_class_deinit(hw_class_t *cls) {
    if (cls == NULL) return;

    pthread_mutex_lock(&cls->lock);

    // 注销所有驱动
    hw_driver_unregister_all_from_class(cls);
    // 解绑删除所有设备
    hw_device_driver_unbind_all(cls);

    pthread_mutex_destroy(&cls->lock);

    HW_INFO("Class %s deinit success\n", cls->name);
}

int hw_class_get_drv_count(hw_class_t *cls)
{
    return (cls != NULL) ? cls->drv_count : 0;
}

int hw_class_get_dev_count(hw_class_t *cls)
{
    return (cls != NULL) ? cls->dev_count : 0;
}

// ====================== Driver 接口实现 ======================
int hw_driver_register(hw_class_t *cls, hw_driver_t *drv) {
    if (cls == NULL || drv == NULL || drv->name == NULL) {
        HW_ERR("hw_driver_register: invalid param (cls=%p, drv=%p)\n", cls, drv);
        return -1;
    }

    pthread_mutex_lock(&cls->lock);

    // 检查是否已注册
    hw_driver_t *tmp_drv;
    hw_list_for_each_entry(tmp_drv, &cls->drv_list, node) {
        if (strcmp(tmp_drv->name, drv->name) == 0) {
            HW_ERR("hw_driver_register: driver %s already exist in class %s\n",
                   drv->name, cls->name);
            pthread_mutex_unlock(&cls->lock);
            return -1;
        }
    }

    hw_list_head_init(&drv->node);
    hw_list_add_tail(&drv->node, &cls->drv_list);
    cls->drv_count++;

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Driver %s register to class %s success (total drv: %d)\n",
            drv->name, cls->name, cls->drv_count);
    return 0;
}

void hw_driver_unregister(hw_class_t *cls, hw_driver_t *drv) {
    if (cls == NULL || drv == NULL) return;

    pthread_mutex_lock(&cls->lock);

    // 从链表删除
    hw_list_del(&drv->node);
    cls->drv_count--;

    // 解绑该驱动绑定的所有设备
    hw_device_t *dev;
    hw_list_for_each_entry(dev, &cls->dev_list, node) {
        if (dev->drv == drv) {
            if (dev->drv && dev->drv->remove) {
                dev->drv->remove(dev);
            }
            dev->drv = NULL;
        }
    }

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Driver %s unregister from class %s success\n", drv->name, cls->name);
}
void hw_driver_unregister_all_from_class(hw_class_t *cls) {
    if (cls == NULL) return;

    pthread_mutex_lock(&cls->lock);

    hw_driver_t *drv, *tmp;
    hw_list_for_each_entry_safe(drv, tmp, &cls->drv_list, node) {
        hw_driver_unregister(cls, drv);
    }

    cls->drv_count = 0;
    pthread_mutex_unlock(&cls->lock);
}

// ====================== Device 接口实现 ======================
int hw_device_add(hw_class_t *cls, hw_device_t *dev) {
    if (cls == NULL || dev == NULL || dev->name == NULL) {
        HW_ERR("hw_device_add: invalid param (cls=%p, dev=%p)\n", cls, dev);
        return -1;
    }

    pthread_mutex_lock(&cls->lock);

    // 检查设备是否已存在
    hw_device_t *tmp_dev;
    hw_list_for_each_entry(tmp_dev, &cls->dev_list, node) {
        if (strcmp(tmp_dev->name, dev->name) == 0) {
            HW_ERR("hw_device_add: device %s already exist in class %s\n",
                   dev->name, cls->name);
            pthread_mutex_unlock(&cls->lock);
            return -1;
        }
    }

    // 初始化设备节点，尾插法添加
    dev->cls = cls;
    dev->drv = NULL;
    hw_list_head_init(&dev->node);
    hw_list_add_tail(&dev->node, &cls->dev_list);
    cls->dev_count++;

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Device(%p) %s add to class %s success (total dev: %d)\n",
            dev, dev->name, cls->name, cls->dev_count);
    return 0;
}
void hw_device_remove(hw_class_t *cls, hw_device_t *dev) {
    if (cls == NULL || dev == NULL) return;

    pthread_mutex_lock(&cls->lock);

    // 解绑驱动
    if (dev->drv) {
        if (dev->drv && dev->drv->remove)
        {
            dev->drv->remove(dev);
        }
        dev->drv = NULL;
    }

    // 从链表删除
    hw_list_del(&dev->node);
    cls->dev_count--;

    pthread_mutex_unlock(&cls->lock);

    HW_INFO("Device %s remove from class %s success\n", dev->name, cls->name);
}
int hw_device_driver_match(hw_class_t *cls)
{
    if (cls == NULL)
    {
        HW_ERR("hw_device_driver_match: invalid class (cls=%p)\n", cls);
        return -1;
    }

    pthread_mutex_lock(&cls->lock);

    int match_count = 0;
    hw_device_t *dev;
    hw_driver_t *drv;

    // 遍历所有设备，匹配兼容的驱动
    hw_list_for_each_entry(dev, &cls->dev_list, node)
    {
        if (dev->compatible == NULL)
        {
            HW_WARN("Device %s has no compatible field, skip match\n", dev->name);
            continue;
        }

        // 遍历所有驱动，匹配compatible
        hw_list_for_each_entry(drv, &cls->drv_list, node)
        {

            if (dev->compatible && strcmp(dev->compatible, drv->name) == 0)
            {
                dev->drv = drv;
                match_count++;

                if (drv && drv->probe)
                {
                    int ret = drv->probe(dev);
                    if (ret != 0)
                    {
                        HW_ERR("Device %s probe failed (driver: %s, ret=%d)\n",
                               dev->name, drv->name, ret);
                        dev->drv = NULL;
                        match_count--;
                    } else
                    {
                        HW_INFO("Device %s match driver %s success (compatible: %s)\n",
                                dev->name, drv->name, dev->compatible);
                    }
                }
                break; // 匹配到第一个驱动即退出
            }
        }

        if (!dev->drv) {
            HW_WARN("Device %s (compatible: %s) no matched driver in class %s\n",
                    dev->name, dev->compatible, cls->name);
        }
    }

    pthread_mutex_unlock(&cls->lock);

    if (match_count == 0) {
        HW_ERR("No device-driver match in class %s\n", cls->name);
        return -1;
    }

    HW_INFO("Class %s device-driver match success (matched: %d/%d)\n",
            cls->name, match_count, cls->dev_count);
    return 0;
}
void hw_device_driver_unbind_all(hw_class_t *cls) {
    if (cls == NULL) return;

    pthread_mutex_lock(&cls->lock);

    hw_device_t *dev, *tmp;
    hw_list_for_each_entry_safe(dev,tmp, &cls->dev_list, node)
    {
        if (dev->drv && dev->drv->remove)
        {
            dev->drv->remove(dev);
            dev->drv = NULL;
            HW_INFO("Device %s unbind driver success\n", dev->name);
        }
        hw_device_remove(cls,dev);
        if(dev)
        {
            free(dev);
        }
    }
    pthread_mutex_unlock(&cls->lock);
}
/**
 * @brief 读取文件内容到缓冲区
 * @param file_path JSON文件路径
 * @param file_len 输出参数，文件长度
 * @return 成功返回缓冲区指针，失败返回NULL
 */
char* read_file_to_buf(const char *file_path, long *file_len) {
    if (!file_path || !file_len) {
        printf("错误：文件路径或长度指针为空\n");
        return NULL;
    }

    // 打开文件（二进制模式避免换行符问题）
    FILE *fp = fopen(file_path, "rb");
    if (!fp) {
        printf("错误：打开文件失败 %s\n", file_path);
        return NULL;
    }

    // 获取文件长度
    fseek(fp, 0, SEEK_END);
    *file_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (*file_len <= 0) {
        printf("错误：文件为空 %s\n", file_path);
        fclose(fp);
        return NULL;
    }

    // 分配缓冲区（+1 预留字符串结束符）
    char *buf = (char*)malloc(*file_len + 1);
    if (!buf) {
        printf("错误：内存分配失败\n");
        fclose(fp);
        return NULL;
    }

    // 读取文件内容
    size_t read_len = fread(buf, 1, *file_len, fp);
    if (read_len != *file_len) {
        printf("错误：读取文件不完整，预期%ld字节，实际%zu字节\n", *file_len, read_len);
        free(buf);
        fclose(fp);
        return NULL;
    }
    buf[*file_len] = '\0';  // 字符串结束符

    fclose(fp);
    return buf;
}
/**
 * @brief 判断是否为类Key（以class_开头）
 */
int is_class_key(const char *key) {
    return key && strncmp(key, "class_", strlen("class_")) == 0;
}

/**
 * @brief 判断是否为设备Key（以device_开头）
 */
int is_device_key(const char *key) {
    return key && strncmp(key, "device_", strlen("device_")) == 0;
}

static struct hw_class* find_class(char *name)
{
    for (int i = 0; generate_class_list[i] != NULL; i++)
    {
	if(!strcmp(name,generate_class_list[i]->name))
	{
		return generate_class_list[i];
	}
    }
    return NULL;
}

static struct hw_driver* find_driver(char *name)
{
    for (int i = 0; generate_driver_list[i] != NULL; i++)
    {
	if(!strcmp(name,generate_driver_list[i]->name))
	{
		return generate_driver_list[i];
	}
    }
    return NULL;
}
static int parse_device_node(cJSON *root_node,hw_device_t *dev)
{
    hw_driver_t *drv = NULL;
    cJSON *compatible = cJSON_GetObjectItem(root_node, "compatible");
    if(!compatible)
    {
        HW_ERR("%s compatile not find\n",dev->name);
        return -1;
    }
    dev->compatible = strdup(compatible->valuestring);
    drv = find_driver(compatible->valuestring);
    if(!drv)
    {
        HW_WARN("%s not match driver\n",compatible->valuestring);
        return -1;
    }
    hw_driver_register(dev->cls, drv);
    return 0;
}
static int parse_device(cJSON *root_node,struct hw_class *cls)
{
    struct hw_device *dev = NULL;
    cJSON *child = NULL;
    child = root_node->child;
    while(child != NULL)
    {
        if (!is_device_key(child->string)) 
	{
		child = child->next;
		continue;
	}
        dev =(struct hw_device *) malloc(sizeof(struct hw_device));
        if(!dev)
        {
            printf("dev malloc error\n");
            return -1;
        }
	memset(dev, 0, sizeof(hw_device_t));
        dev->name = strdup(child->string);
        if (dev->name == NULL) {
            HW_ERR("%s: strdup failed (errno=%d)\n",__FUNCTION__, errno);
            return -1;
        }
        dev->root = child;
        hw_device_add(cls,dev);
        parse_device_node(child, dev);
	child = child->next;
    }
    return 0;
}
static int parse_all_class(cJSON *root_node)
{
    struct hw_class *cls = NULL;

    cJSON *child = NULL;
    child = root_node->child;

    while(child != NULL)
    {
    	if (!is_class_key(child->string)) 
	{
		child = child->next;
		continue;
	}
	cls = find_class(child->string);
	if(cls == NULL)
	{
		printf("%s not find\n",child->string);
                child = child->next;
		continue;
	}
	printf("%s find\n",child->string);
        hw_class_init(cls,cls->name);
        parse_device(child,cls);
        hw_device_driver_match(cls);
	child = child->next;

    }

    return 0;
}

int _bsp_device_get(struct hw_class *cls,struct hw_device **out_dev , int id)
{
    int index = 0;
    struct hw_device *dev = NULL;
    hw_list_for_each_entry(dev, &cls->dev_list, node)
    {
        if(index == id)
        {
            *out_dev = dev;
            return 0;
        }
        index++;
    }
    HW_ERR("not find device\n");
    return -1;
}

int bsp_init()
{
    long file_len = 0;
    char *json_buf = read_file_to_buf(CONFIG_JSON_PATH, &file_len);
    if (!json_buf)
    {
        return -1;
    }

    // 3. 解析JSON字符串
    cJSON *root_node = cJSON_Parse(json_buf);
    free(json_buf);  // 解析完成后释放文件缓冲区
    if (!root_node)
    {
        printf("错误：JSON解析失败 %s\n", cJSON_GetErrorPtr());
        return -1;
    }
    parse_all_class(root_node);
    cJSON_Delete(root_node);
    return 0;
}

int bsp_deinit()
{
    for (int i = 0; generate_class_list[i] != NULL; i++)
    {
        hw_class_deinit(generate_class_list[i]);
    }
    return 0;
}
