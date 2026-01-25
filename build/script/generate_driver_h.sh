#!/bin/bash
set -e

# ====================== 配置项 ======================
CONFIG_FILE="./.config"
OUTPUT_HEADER="./wt_tools_driver.h"
CLASS_STRUCT_TYPE="struct hw_driver"

# ====================== 用法说明 ======================
usage() {
    echo "用法: $0 [选项]"
    echo "  功能：从.config提取CONFIG_DRIVER_*=y，自动生成全局类实例+映射数组"
    echo "  选项："
    echo "    -c <path>  指定.config文件路径（默认：./.config）"
    echo "    -o <path>  指定输出头文件路径（默认：./wt_tools_driver.h）"
    echo "    -t <type>  指定类结构体类型（默认：struct hw_driver）"
    echo "    -h         显示帮助信息"
    exit 1
}

# ====================== 解析命令行参数 ======================
while getopts "c:o:t:h" opt; do
    case $opt in
        c) CONFIG_FILE="$OPTARG" ;;
        o) OUTPUT_HEADER="$OPTARG" ;;
        t) CLASS_STRUCT_TYPE="$OPTARG" ;;
        h) usage ;;
        *) echo "无效参数: -$OPTARG"; usage ;;
    esac
done

# 检查.config文件是否存在
if [ ! -f "$CONFIG_FILE" ]; then
    echo "❌ 错误：.config文件不存在 -> $CONFIG_FILE"
    exit 1
fi

# ====================== 核心修复：用进程替换代替管道，避免子shell变量丢失 ======================
# 1. 先提取所有匹配的CONFIG_CLASS_*=y行（允许行首/行尾空格）
MATCH_LINES=$(grep -E '^[[:space:]]*CONFIG_DRIVER_[A-Z0-9_]+[[:space:]]*=[[:space:]]*y[[:space:]]*$' "$CONFIG_FILE")

if [ -z "$MATCH_LINES" ]; then
    echo "⚠️ 警告：.config中未找到CONFIG_DRIVER_*=y格式的配置项"
    CLASS_INSTANCES=""
    CLASS_ENTRIES=""
else
    echo "🔍 匹配到的CONFIG_DRIVER配置项："
    echo "$MATCH_LINES" | sed 's/^/   /'

    # 2. 生成全局类实例（用进程替换< <()，避免子shell）
    CLASS_INSTANCES=""
    while read -r line; do
        # 提取CONFIG_CLASS_后的关键字（如SWITCH）
        class_key=$(echo "$line" | sed -E 's/^[[:space:]]*CONFIG_DRIVER_([A-Z0-9_]+)[[:space:]]*=[[:space:]]*y[[:space:]]*$/\1/' | tr 'A-Z' 'a-z')
        # 生成实例行
        INSTANCE_LINE="${CLASS_STRUCT_TYPE} driver_${class_key};  // 对应CONFIG_CLASS_$(echo "$class_key" | tr 'a-z' 'A-Z')=y"
        CLASS_INSTANCES="extern ${CLASS_INSTANCES}${INSTANCE_LINE}"$'\n'
    done < <(echo "$MATCH_LINES")

    # 3. 生成数组项
    CLASS_ENTRIES=""
    while read -r line; do
        class_key=$(echo "$line" | sed -E 's/^[[:space:]]*CONFIG_DRIVER_([A-Z0-9_]+)[[:space:]]*=[[:space:]]*y[[:space:]]*$/\1/' | tr 'A-Z' 'a-z')
        # 生成数组项行
        ENTRY_LINE="    &driver_${class_key},"
        CLASS_ENTRIES="${CLASS_ENTRIES}${ENTRY_LINE}"$'\n'
    done < <(echo "$MATCH_LINES")
fi

# 去掉末尾多余的换行
CLASS_INSTANCES=$(echo -e "$CLASS_INSTANCES" | sed -e '/^$/d')
CLASS_ENTRIES=$(echo -e "$CLASS_ENTRIES" | sed -e '/^$/d')

# ====================== 生成最终头文件（拼接方式）=====================
# 确保输出目录存在
mkdir -p "$(dirname "$OUTPUT_HEADER")"

# 第一步：写入固定头部
cat > "$OUTPUT_HEADER" << EOF
//
// Automatically generated file; DO NOT EDIT.
// WT eth tool Build Configuration

#ifndef __WT_TOOLS_DRIVER_H
#define __WT_TOOLS_DRIVER_H
//#include "core.h"

// 1. 自动生成的全局类实例（从.config提取）
EOF

# 第二步：写入动态生成的全局实例
if [ -n "$CLASS_INSTANCES" ]; then
    echo -e "$CLASS_INSTANCES" >> "$OUTPUT_HEADER"
fi

# 第三步：写入结构体定义和数组头部
cat >> "$OUTPUT_HEADER" << EOF


// 3. 自动生成的类实例数组（末尾带终止符）
static struct class *generate_driver_list[] = {
EOF

# 第四步：写入动态生成的数组项
if [ -n "$CLASS_ENTRIES" ]; then
    echo -e "$CLASS_ENTRIES" >> "$OUTPUT_HEADER"
fi

# 第五步：写入数组终止符和固定尾部
cat >> "$OUTPUT_HEADER" << EOF
    NULL // 遍历终止标记
};

#endif
EOF

# ====================== 输出成功信息 ======================
echo -e "\n✅ 处理完成！\n"
echo "📄 输入配置文件：$CONFIG_FILE"
echo "📄 输出头文件：$OUTPUT_HEADER"
echo -e "\n🔧 生成内容汇总："
if [ -z "$CLASS_INSTANCES" ]; then
    echo "   无CONFIG_DRIVER_*=y配置项，未生成任何类实例"
else
    echo -e "   生成的全局类实例："
    echo "$CLASS_INSTANCES" | sed 's/^/     /'
    echo -e "\n   生成的类实例数组项："
    echo "$CLASS_ENTRIES" | sed 's/^/     /'
fi

exit 0
