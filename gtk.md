# setup

官方网站 https://www.gtk.org/docs/installations/linux/。

In order to install GTK for GNU/Linux and Unix systems, you will need to get the GLib, GObject-Introspection, Pango, Gdk-Pixbuf, ATK and GTK packages to build GTK. To read more about these packages, please refer to the [Architecture](https://www.gtk.org/docs/architecture/).

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

