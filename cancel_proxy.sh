#!/bin/sh
# 只对当前的shell 有效，
# 如果是console，需要在console 中执行

unset proxyserveraddr
unset proxyserverport
unset HTTP_PROXY
unset HTTPS_PROXY
unset FTP_PROXY
unset SOCKS_PROXY
unset NO_PROXY
unset http_proxy
unset https_proxy
unset ftp_proxy
unset socks_proxy
unset no_proxy
unset ALL_PROXY
unset all_proxy
gsettings reset org.gnome.system.proxy ignore-hosts
echo -n ""|sudo tee /etc/apt/apt.conf
