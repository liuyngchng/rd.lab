#!/bin/bash
# 参数--make的作用是制作依赖项包，就是把所有的依赖项拷贝到APPDEPALL目录
# 参数--check-and-use在部署是用，作用是在部署环境中检测哪些动态库缺失，并把缺失的动态库拷贝到APPDEP目录里

if [ "$#" = "0" ]; then
    echo "Error: please add option.";
    exit 1
fi
 
ACTION=make #[make|check-and-use]
 
#应用程序的安装目录
APPDIR=/data/workroom/industry/linux-x86-64/jni
#应用程序所有依赖项的存放目录
APPDEPALL=$APPDIR/dependencies/dep-all
#应用程序在实际部署时缺少的动态库要拷贝到目录，这个目录要在应用程序运行前加到LD_LIBRARY_PATH环境变量里
APPDEP=$APPDIR/dependencies/dep
 
while [ $# -gt 0 ]; do    # Until you run out of parameters...
    case "$1" in
        --make)
            ACTION=make
            ;;
        --check-and-use)
            ACTION=check-and-use
            ;;
        --help)
            echo "Options:"
            echo "  --make          [Make a dependency package]"
            echo "  --check-and-use [Check and use dependent packages]"
            echo "  --help          [show help]"
            exit
            ;;
        *)
            echo >&2 'copyDependencies.sh:' $"unrecognized option" "\`$1'"
            echo >&2 $"Try \`copyDependencies.sh --help' for more information."
            exit 1
            ;;
    esac
    shift   # Check next set of parameters.
done
 
if [ "$ACTION" = "make" ]; then
    #在这里可以根据自己的需要设置LD_LIBRARY_PATH,让ldd命令可以正确的找到动态库
    export LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
    #下面这个命令通过管道和ldd命令搜索出来应用程序以来的所有的动态库，并将他们拷贝到APPDEPALL目录
    find "$APPDIR/" -type f | grep -v "^$APPDEPALL/" | xargs -i file -F " // " "{}" | grep -E " // [ ]{0,}ELF" | awk -F " // " '{print $1}' | xargs -i ldd {} | awk -F " => " '{ if(NF==2) print $2}' | awk -F " [(]{1}0x[0-9a-f]{16}[)]{1}$" '{ if(NF==2) print $1}' | grep -v "^$APPDIR/" | xargs -i \cp -L -n "{}" "$APPDEPALL/"
else
 
    #ldconfig -p列出当前部署环境的动态库，如果没有则将动态库拷贝到APPDEP目录
    for filename in "$APPDEPALL/"* ; do
        basefilename=`basename "$filename"`
        ldre=`ldconfig -p | grep "/$basefilename\$"`
        if [ -z "$ldre" ]; then
            echo copy "$basefilename" to "$APPDEP/"
            \cp -L -n "$filename" "$APPDEP/"
        fi
    done
 
    # 有时虽然有同名的动态库，但是两个库的版本信息不一致导致程序不能启动，
    # 这是需要通过一下命令检测版本信息，如果缺少版本信息则将动态库拷贝到 APPDEP 目录
    # 以下命令参考了rpm的find-requires脚本，路径是/usr/lib/rpm/find-requires
    whereisobjdump=`whereis objdump`
    if [ ! "$whereisobjdump" = "objdump:" ]; then
        for checkobj in "$APPDIR/libINDUSTMGAPI.so" "$APPDIR/libswsjj.so" ; do
            reqverinfos=`objdump -p "$checkobj" | awk 'BEGIN { START=0; LIBNAME=""; }
            /^[ ]{0,}required from .*:$/ { START=1; }
            (START==1) && /required from / {
                sub(/:/, "", $3);
                LIBNAME=$3;
            }
            (START==1) && (LIBNAME!="") && ($4!="") {
                print LIBNAME" ----- "$4;
            }
            '`
            echo "$reqverinfos" | while read reqverinfo
            do
                LIBNAME=`echo $reqverinfo | awk -F " ----- " '{print $1}'`
                VERSION=`echo $reqverinfo | awk -F " ----- " '{print $2}'`
                if [ ! X"$LIBNAME" = X -a ! X"$VERSION" = X -a ! -f "$APPDEP/$LIBNAME" ]; then
                    ldre=`ldconfig -p | grep "/$LIBNAME\$" | awk -F " => " '{print $2}'`
                    findre=`strings "$ldre" | grep "^$VERSION\$"`
                    if [ -z "$findre" ]; then
                        echo  copy "$LIBNAME" to $APPDEP/
                        \cp -L -n "$APPDEPALL/$LIBNAME" "$APPDEP/"
                    fi
                fi
            done
        done
    else
        echo "objdump executable file not found, please check the dynamic library version information by yourself"
    fi
 
fi
