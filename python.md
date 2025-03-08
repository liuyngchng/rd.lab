# 1. python 文件 head

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



# 2. 虚拟环境

构建虚拟环境，每个环境下的包与其他环境隔离，方便在不同的环境下进行不同的任务，同时不会互相干扰。官方文档详见 https://docs.python.org/3.10/library/venv.html。这里推荐 `virtualenv `命令。

创建虚拟环境的缺点，就是会导致本地的package 更大。不同环境之间各自用自己的package，无法共用，这也是不可避免的。

## 2.1 创建

```sh
# 安装指令
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
pip freeze --all > requirements.txt
```



