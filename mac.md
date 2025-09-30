# 1. mount ext4 file system
## 1.1 查看分区
```sh
diskutil list
mkdir ubuntu
```
## 1.2 install fuse-ext2  
see `https://github.com/alperakcan/fuse-ext2`  
## 1.3 mout  
```sh
sudo mount -t fuse-ext2 /dev/disk0s3 ./ubuntu/
```
## 1.4 umount
```sh
sudo umount /dev/disk0s3
```
