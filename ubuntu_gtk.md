# Ubuntu16.04下安装GTK+

# 1. 安装

## 1.1 安装gcc/g++/gdb/make 等基本编程工具  
```
sudo apt-get install build-essential  
```
## 1.2 安装 libgtk2.0-dev libglib2.0-dev 等开发相关的库文件  
```
sudo apt-get install gnome-core-devel   
```
## 1.3 用于在编译GTK程序时自动找出头文件及库文件位置  
```
sudo apt-get install pkg-config
```
## 1.4 安装 devhelp GTK文档查看程序
```
sudo apt-get install devhelp
```
## 1.5 安装 gtk/glib 的API参考手册及其它帮助文档
```
sudo apt-get install libglib2.0-doc libgtk2.0-doc
```
## 1.6 安装基于GTK的界面GTK是开发Gnome窗口的c/c++语言图形库 
```
sudo apt-get install glade libglade2-dev
```
或者
```
sudo apt-get install glade-gnome glade-common glade-doc
```
## 1.7 安装gtk2.0 或者 将gtk+2.0所需的所有文件统通下载安装完毕
```
sudo apt-get install libgtk2.0-dev
```
或者
```
sudo apt-get install libgtk2.0*
```
# 2. 查看GTK库版本
## 2.1 查看 2.x 版本
```
pkg-config --modversion gtk+-2.0
```
## 2.2 查看pkg-config的版本
```
pkg-config --version
```
## 2.3查看是否安装了gtk
```
$pkg-config --list-all grep gtk
```
