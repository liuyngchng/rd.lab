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

#  查看文档

```sh
# 执行完安装文档语句 
sudo apt-get install libgtk2.0-doc
```

在浏览器中打开

```html
file:///usr/share/doc/libgtk2.0-doc/tutorial/index.html
```

