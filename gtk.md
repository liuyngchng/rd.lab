# setup

官方网站 https://www.gtk.org/docs/installations/linux/。

In order to install GTK for GNU/Linux and Unix systems, you will need to get the GLib, GObject-Introspection, Pango, Gdk-Pixbuf, ATK and GTK packages to build GTK. To read more about these packages, please refer to the [Architecture](https://www.gtk.org/docs/architecture/).

## Ubuntu

```sh
# for ubuntu 22.04LTS
sudo apt search libgtk2.0

# setup lib
sudo apt-get install libgtk2.0-dev

# setup doc
sudo apt-get install libgtk2.0-doc

# setup  pkg-config cmd
sudo apt install pkgconf

# setup GLib library
sudo apt-get install libglib2.0-dev

# setup libconfig , parsing/manipulation of structured config files
sudo apt-get install libconfig-dev
```

解决 “Failed to load module "canberra-gtk-module"”， 执行

```sh
sudo apt-get install libcanberra-gtk-module
```

查找某个模块所在的位置

```sh
locate libgtk2.0-dev
```

查看gtk版本号

```sh
# 查看 GTK 的详细版本号
pkg-config --modversion gtk+-2.0

# 查看安装的 GTK 及其相关库的信息
pkg-config --list-all | grep gtk
```

## Mac OS


使用的系统为 Mac OS 10.14.6，

https://www.cnblogs.com/suugee/p/15853135.html

采用源码安装方式。 整个GTK+源码依赖如下所示。第三极依赖可能还有遗漏
=======
​		使用的系统为 Mac OS 10.14.6。

### brew install

```
brew update --force --quiet
brew install pkg-config

```



### 源码安装

整个GTK+源码依赖如下所示。全部依赖包括 docbook, docbook-xsl, libyaml, openssl@3, m4, autoconf, libssh2, cmake, libgit2@1.7, mpdecimal, readline, sqlite, xz, libffi, python@3.12, z3, lz4, zstd, ninja, pcre2, swig, llvm, rust, ruby, asciidoctor, automake, gnu-getopt, xmlto, dbus, gettext, bison, meson, python-setuptools, glib, util-macros, xorgproto, libxau, libxdmcp, xcb-proto, libxcb, xtrans, libx11, libxext, libxfixes, libxi, libxtst, libpng, freetype, fontconfig, libxrender, lzo, pixman, cairo, gobject-introspection, at-spi2-core, nasm, jpeg-turbo, libtiff, docutils, gdk-pixbuf, hicolor-icon-theme, fribidi, graphite2, icu4c, harfbuzz and pango
```
GTK+
	+ glib
			- gettext
			- libiconv
	- atk
	+ pango
	+ cairo
			- pixman
	+ gdk-pixbuf
			- libjpeg
			- libtiff
			- libpng
```

在 gtk官网 https://download.gnome.org/sources/ 下载源码，GTK+选择了 gtk+-2.24.33版本， 最终目录如下

```sh
drwxr-xr-x@  41 richard  staff   1.3K Jul 26 10:22 atk-1.29.2
-rw-r--r--@   1 richard  staff   978K Jul 26 09:31 atk-1.29.2.tar.gz
drwxr-xr-x@  67 richard  staff   2.1K Jul 26 10:54 cairo-1.17.6
-rw-r--r--@   1 richard  staff    33M Jul 26 09:33 cairo-1.17.6.tar.xz
drwxr-xr-x@  40 richard  staff   1.3K Jul 26 14:19 gdk-pixbuf-2.21.7
-rw-r--r--@   1 richard  staff   2.1M Jul 26 09:33 gdk-pixbuf-2.21.7.tar.gz
drwxr-xr-x@  36 richard  staff   1.1K Jul 26 09:44 gettext-0.22.5
-rw-r--r--@   1 richard  staff    26M Jul 26 09:37 gettext-0.22.5.tar.gz
drwxr-xr-x@ 104 richard  staff   3.3K Jul 26 10:13 glib-2.28.0
-rw-r--r--@   1 richard  staff   9.3M Jul 26 09:31 glib-2.28.0.tar.gz
drwxr-xr-x@  83 richard  staff   2.6K Jul 26 14:48 gtk+-2.24.33
-rw-r--r--@   1 richard  staff    12M Jul 26 09:29 gtk+-2.24.33.tar.xz
drwxr-xr-x@ 328 richard  staff    10K Jul 26 13:51 jpeg-9f
-rw-r--r--@   1 richard  staff   1.0M Jul 26 13:49 jpegsrc.v9f.tar.gz
drwxr-xr-x@  46 richard  staff   1.4K Jul 26 10:07 libiconv-1.16
-rw-r--r--@   1 richard  staff   4.9M Jul 26 10:06 libiconv-1.16.tar.gz
drwxr-xr-x@ 134 richard  staff   4.2K Jul 26 14:05 libpng-1.6.43
-rw-r--r--@   1 richard  staff   1.5M Jul 26 14:03 libpng-1.6.43.tar.gz
drwxr-xr-x@  80 richard  staff   2.5K Jul 26 14:43 pango-1.24.0
-rw-r--r--@   1 richard  staff   2.1M Jul 26 14:42 pango-1.24.0.tar.gz
-rw-r--r--@   1 richard  staff   1.1M Jul 26 14:41 pango-1.30.0.tar.xz
drwxr-xr-x@  39 richard  staff   1.2K Jul 26 10:51 pixman-0.38.0
-rw-r--r--@   1 richard  staff   870K Jul 26 10:50 pixman-0.38.0.tar.gz
drwxr-xr-x@  33 richard  staff   1.0K Jul 26 11:13 tiff-4.6.0t
-rw-r--r--@   1 richard  staff   3.7M Jul 26 11:11 tiff-4.6.0t.tar.gz
```

首先安装 gettext，这个是glib依赖的，进入页面下载。

```sh
tar -zxf gettext-0.22.5.tar.gz
cd gettext-0.22.5
./configure
sudo make install
```

安装 libiconv， 这个也是 glib依赖的， 详见 http://savannah.gnu.org/projects/libiconv/

```sh
tar -zxf libiconv-1.16.tar.gz
cd libiconv-1.16
./configure
sudo make install
```

安装 glib

```sh
tar -zxf glib-2.28.0.tar.gz
cd glib-2.28.0
./configure
sudo make install
```



安装atk

```sh
cd atk-1.29.2
./configure
sudo make install
```

安装pango , 实际使用版本1.24.0

```sh
tar -zxf pango-1.20.5.tar.gz
cd pango-1.20.5
./configure
sudo make install
```

安装 cairo, 依赖的 requires pixman-1 >= 0.36.0， 详细见 https://www.pixman.org

```sh
tar -zxf pixman-0.38.0.tar.gz
cd pixman-0.38.0
./configure 
sudo make install
```

安装 cairo

 ```sh
 tar -xf cairo-1.17.6.tar.xz
 cd cairo-1.17.6
 ./configure
 sudo make install
 ```

安装 gdk-pixbuf 所依赖的 TIFF loader libtiff， 详见 http://www.libtiff.org/downloads/，或者在  gdk-pixbuf 中执行

```sh
./configure --without-libtiff --without-libjpeg
```

其他暂时不要的库都可以通过--without-libunwanted 的方式暂时不安装。

```sh
tar -zxf tiff-4.6.0t.tar.gz
cd tiff-4.6.0t
./configure
sudo make install
```

安装  gdk-pixbuf 所依赖的 libjpeg， 详见 https://libjpeg.sourceforge.net， http://www.ijg.org， 

```sh
tar -zxf jpegsrc.v9f.tar.gz
cd jpeg-9f
./configure
sudo make install
```

 安装 gdk-pixbuf 所依赖的 libpng， 详见 http://www.libpng.org

```sh
tar -zxf libpng-1.6.43.tar.gz
cd libpng-1.6.43
./configure
sudo make install
```



安装 gdk-pixbuf

```sh
tar -xf gdk-pixbuf-2.21.7.tar.gz
cd gdk-pixbuf-2.21.7
./configure
sudo make install
```

最后一步，安装 GTK+， 

```sh
tar -xf gtk+-2.24.33.tar.xz
cd gtk+-2.24.33
```



## Windows 

#  查看文档

GTK 源代码 https://gitlab.gnome.org/GNOME/gtk。

GTK 在线文档  ， GTK3： https://docs.gtk.org/gtk3/, GTK4：https://docs.gtk.org/gtk4/, 更低版本的文档官网无法看到。 目前Ubuntu 22.04LTS可安装的最高版本为gtk2.0。

```sh
# 执行完安装文档语句 
sudo apt-get install libgtk2.0-doc
```

在浏览器中打开

```html
file:///usr/share/doc/libgtk2.0-doc/tutorial/index.html
```

# API

窗口大小默认是可调整的， 若想设置窗口大小不可调整，可通过以下函数实现

```c
gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
```

