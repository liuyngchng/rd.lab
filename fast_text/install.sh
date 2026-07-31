#!/bin/bash
# fastText 环境安装脚本
# 需要 sudo 权限

set -e

echo "=== 安装 fastText CLI ==="
sudo apt install -y fasttext

echo ""
echo "=== 验证安装 ==="
fasttext --help 2>&1 | head -5

echo ""
echo "=== 安装 Python 依赖 ==="
source ~/workspace/llm_py_env/bin/activate
pip install -q --no-input "numpy<2" 2>&1 | tail -1

echo ""
echo "=== 完成 ==="
echo "fastText 版本: $(fasttext --version 2>&1 || true)"
