#!/bin/bash

script_dir=$(dirname "$(realpath "$0")")

# 切换到当前脚本所在的目录
cd "$script_dir" || exit

# 遍历当前文件夹下的所有 .s 文件
for file_name in *.s; do
    # 检查文件是否存在
    if [ ! -f "$file_name" ]; then
        echo "文件 $file_name 不存在"
        continue  # 继续下一个文件
    fi

    test_name=$(basename "$file_name" .s)
    echo -e "\n\n file name $file_name"
    t=out/$test_name

    mkdir -p "$t"

    # 执行文件
    chmod +x "$file_name"  # 添加执行权限
    ../cmake-build-debug/as_final  -o "$t"/res "$file_name"
    arm-linux-gnueabihf-gcc -static "$t"/res -o "$t"/fi_res
    qemu-arm "$t"/fi_res
    echo $?
done