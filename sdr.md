# source 

```
http://t.zoukankan.com/milton-p-8580639.html
```

# ubuntu

```sh
# 软件安装 Ubuntu16.04

# 在Ubuntu下, 需要使用 Gqrx 这个软件, http://gqrx.dk/  参考安装说明 http://gqrx.dk/download/install-ubuntu以及http://gqrx.dk/doc/practical-tricks-and-tips .

# 首先移除存在的相关软件
sudo apt-get purge --auto-remove gqrx
sudo apt-get purge --auto-remove gqrx-sdr
sudo apt-get purge --auto-remove libgnuradio*

# 然后检查并删除相关的软件源 gqrx/releases 和 gqrx/snapshots PPA,  添加相关的软件源
sudo add-apt-repository -y ppa:bladerf/bladerf
sudo add-apt-repository -y ppa:myriadrf/drivers
sudo add-apt-repository -y ppa:myriadrf/gnuradio
sudo add-apt-repository -y ppa:gqrx/gqrx-sdr
sudo apt-get update

#然后就可以安装了
sudo apt-get install gqrx-sdr
#启动gqrx之前, 需要插入rtl2832 usb, 并且系统能正确识别, 可以通过dmesg命令查看.
gqrx &
# 软件的运行界面
```

![img](https://images2018.cnblogs.com/blog/650273/201803/650273-20180316134350857-1094321963.png)