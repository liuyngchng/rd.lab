# 1. Windows 下环境的配置

下载 python 3.12.3, https://www.python.org/downloads/release/python-3123/ , [Windows installer (64-bit)](https://www.python.org/ftp/python/3.12.3/python-3.12.3-amd64.exe)



# 2. python 文件 head

（1）shebang 行。文件第一行，指定该文件应该使用的解释器。

```python
#!/usr/bin/env python3
```

（2）设置文件编码。告诉python 解释器应采用UTF-8编码格式读取文件，增加文件的跨操作系统的适应性。

```python
# -*- coding: utf-8 -*-
```



# 2. 重要的命令

（1）重新强制安装损坏的包

```sh
# 强制重新安装，会忽略目前已安装的对应组件，直接覆盖
pip install --force-reinstall module
# 卸载组件
pip uninstall module
# 卸载1.txt里的所有组件
pip uninstall -r .1.txt
# 安装requirements.txt 中的组件清单
pip install -r requirements.txt
```

（2）安装指定版本的组件

```sh
pip install model_name==version_numer
pip install numpy==1.26.2
```

（3）查看某个组件版本号

```sh
pip show numpy
```
（4）Python Image Library(PIL)
PIL 也叫Pillow
```sh
pip install pillow
```

 (5) requirements.txt
 自动生成项目依赖的包及其版本

 ```sh
 pip install pipreqs
 pipreqs . --force
 ```
# 2. 虚拟环境

构建虚拟环境，每个环境下的包与其他环境隔离，方便在不同的环境下进行不同的任务，同时不会互相干扰。官方文档详见 https://docs.python.org/3.10/library/venv.html。这里推荐 `virtualenv `命令。

创建虚拟环境的缺点，就是会导致本地的package 更大。不同环境之间各自用自己的package，无法共用，这也是不可避免的。

## 2.1 创建

```sh
# 安装指令
sudo apt-get install virtualenv
pip install virtualenv
# 也可以使用 venv, Ubuntu 系统
sudo apt install python3.10-venv

# 创建虚拟环境 llm_py_env, 会在当前目录下创建一个名称为 llm_py_env 的新目录
virtualenv llm_py_env
# 用这个命令也可以
python -m virtualenv llm_py_env
python -m venv llm_py_env
pwd
/home/rd/workspace
rd@rd-tpd:~/workspace
# 启用虚拟环境，会看到在用户名称前多了一个 “(llm_py_env) ”
source ./llm_py_env/bin/activate
pwd
/home/rd/workspace
(llm_py_env) rd@rd-tpd:~/workspace/$
# 启用虚拟环境之后，再通过pip 安装包时，新安装的包就会存储在虚拟环境的目录下



```

## 2.2 退出

退出虚拟环境后，如果不再需求，可以直接删除。

```sh
# 退出虚拟环境
deactivate llm_py_env
# 删除虚拟环境， 首先进入创建的虚拟环境目录的父目录，然后执行删除
cd parent_dir
rm -fr llm_py_env
```



## 2.3 打包

进入虚拟环境后，形成组件清单

```sh
# 在虚拟环境下打包
source ./llm_py_env/bin/activate

# 不推荐
#pip freeze --all > requirements.txt

# List installed packages
pip list --format=freeze > requirements.txt 

# download all package in a dir
pip download -r requirements.txt -d ./ubuntu22_04_lts_python_3_10_12_offline_package
pip install *.wheel
```

# 3. jupyter

a light UI dev and op system for data process.

```sh
# setup
pip install jupyter notebook
# run
jupyter notebook
```

# 4. 镜像源

## 4.1 使用指定的镜像源

```sh
# for temporary case
pip install some_package -i https://pypi.tuna.tsinghua.edu.cn/simple
# for permanent
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple

pip3 install some_package -i http://your_private_domain/root/pypi/+simple --trusted-host your_private_domain
```

## 4.2 离线安装

```sh
# 进入到储存whl package的 目录下
cd my_whl_dir
# 在有网络的环境下事先下载你的whl包,例如,下载 torch相关的whl， 必须确保离线和有线环境下的python pip版本号完全一致
pip download torch

#离线安装
pip install torch --no-cache-dir --no-index --find-links=/a/b/c/my_whl_dir
```



## 4.3 搭建本地镜像源

todo：尚未验证

```sh
方法2：使用local-pypi-server
local-pypi-server是一个轻量级的本地pypi服务器，它可以帮助你管理本地Python包。

安装local-pypi-server：

pip install local-pypi-server

启动服务器：

local-pypi-server --port 8000 --repo /path/to/your/packages
这里--repo参数指定了你的包存储位置。你可以将需要发布的包放在这个目录下。

方法3：使用devpi
devpi是一个更高级的pypi服务器，支持更多的功能和配置选项。

安装devpi-server和devpi-client：

pip install devpi-server devpi-client

启动devpi-server：

devpi-server --host 127.0.0.1 --port 3141 --start

使用devpi-client创建用户和索引：

devpi use http://127.0.0.1:3141/root/pypi
devpi user -cX --password=mypass root
devpi login root --password=mypass
devpi index -c

上传包：

devpi upload /path/to/your/package/dist/*.whl

```

# 5. IDE

pycharm 历史版本

https://www.jetbrains.com.cn/pycharm/download/other.html

# 6. Oracle

```
pip install cx_Oracle
pip install oracledb
```

cx_Oracle 和 oracledb 的区别：

1. **迭代关系**
   oracledb 是 Oracle 官方推出的新版（原 cx_Oracle 的重命名升级版），推荐使用新库。
2. **安装依赖**
   oracledb 默认使用纯 Python 实现的 "Thin模式"，**无需安装 Oracle 客户端**；cx_Oracle 需依赖客户端。
3. **性能优化**
   oracledb 的 Thin 模式性能更好，且支持原生 JSON 等新特性，同时保留兼容旧版的 "Thick模式"（需客户端）。
4. **API 兼容性**
   oracledb 完全兼容 cx_Oracle 的 API，通常只需修改 `import` 语句即可迁移。

**建议**：新项目直接用 `oracledb`，旧项目可逐步迁移。
