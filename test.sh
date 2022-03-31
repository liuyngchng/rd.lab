#!/bin/bash
#***********************************************
#
#      Filename: poc.sh
#
#        Author: RichardLiu - liuyongcheng@163.com
#   Description: ---
#        Create: 2018-05-29 11:07:08
# Last Modified: 2018-05-29 11:07:08
#***********************************************
# Get current user info.
echo '######## start poc test config #######'
echo -e "\033[31mYOU NEED RUN COMMAND 'chmod 777 $0' AND EXECUTE FILE AS '$0\033[0m'"
read
echo -e 'If this is a IM server, all fhe following file is ready, OK?\n|\n - IM***.tar.gz\n - license'
read
echo -e 'If this is a IM Client, all fhe following file is ready, OK?\n|\n - jre.tar.gz\n - config.json\n - CreateUserMain.jar\n - RctMain.jar'
read
LOGIN_USER=`whoami`
if [ $LOGIN_USER != "root" ]; then
	echo 'current user is not root, you can have a try "sudo su".'
	exit -1
fi
echo "Is IM Server ? Please Input Y/N"
read IS_SERVER

SUPERVISORD_STATUS=`ps -ef | grep supervisord | wc -l`
S_CTL_CMD=`/opt/python2/bin/supervisorctl status | grep 'rcx-server.inst.*RUNNING' | wc -l`
IM_SERVER_DIR='/data/app/rcx-server.inst-0/configs'
LOG4J_FILE='log4j2.xml'
LOG_CHECK_CMD1=-1
LOG_CHECK_CMD2=-1
if [ -d $IM_SERVER_DIR ]; then
    LOG_CHECK_CMD1=`cat $IM_SERVER_DIR/$LOG4J_FILE | grep 'Logger.*level\s*=\s*"\s*info\s*"' -i | wc -l`
    LOG_CHECK_CMD2=`cat $IM_SERVER_DIR/$LOG4J_FILE | grep 'Logger.*level\s*=\s*"\s*debug\s*"' -i | wc -l`
fi
#Config IM Server setup
SETUP_DIR=''
IS_SERVER=''
PROMPT='Is IM Server? Please Input Y/N'
echo $PROMPT
read IS_SERVER
while [[ $IS_SERVER != 'Y' && $IS_SERVER != 'N' ]]; do
	echo $PROMPT
	read IS_SERVER
done
# Config IM Server log level if service started.
if [[ $IS_SERVER == "Y" && $SUPERVISORD_STATUS == 2 ]]; then
    if [[ -d $IM_SERVER_DIR && $LOG_CHECK_CMD1 == 0 && $LOG_CHECK_CMD2 == 0 ]]; then
        echo 'IM server log level is alreay set to ERROR.'
        exit 0
    elif [ $S_CTL_CMD == 1 ]; then
        echo 'IM Server started, start to change log level.'
        cp $IM_SERVER_DIR'/'$LOG4J_FILE $IM_SERVER_DIR'/'$LOG4J_FILE'.bck'
        echo "$IM_SERVER_DIR/$LOG4J_FILE backuped as $IM_SERVER_DIR/$LOG4J_FILE.bck"
        sed -i 's/level *= *" *info *"/level="ERROR"/i' $IM_SERVER_DIR'/'$LOG4J_FILE
        sed -i 's/level *= *" *debug *"/level="ERROR"/i' $IM_SERVER_DIR'/'$LOG4J_FILE
        /opt/python2/bin/supervisorctl restart rcx-server.inst-0
        /opt/python2/bin/supervisorctl status
        echo 'IM Server log level changed to ERROR and service restarted.'
        exit 0
    else
        echo 'Supervisord started but IM server not started, please check it, maybe need to execute "systemctl stop supervisord".'
        exit -1
    fi
fi

# Unzip IM tar ball.
if [ $IS_SERVER == "Y" ]; then
	echo 'This is IM server.'
	TAR_BALL=`ls | grep "IM.*\.gz$"`
	if [ -z "$TAR_BALL" ];then
		echo 'No IM tar ball found, please check it again.'
		exit -1
	fi
	LICENSE=`ls | grep '^license$'`
	if [[ -z "$LICENSE" || ! -f $LICENSE ]];then
		echo 'No license file foud, please check if again.'
		exit -1
	fi
	echo -e "IM tar ball is: \033[31m$TAR_BALL\033[0m"
	echo -e "License file is: \033[31m$LICENSE\033[0m"
	echo 'start to extract file from '$TAR_BALL
	tar -zxf $TAR_BALL
	FILES=`ls | grep 'IM.*'`
	for FILE in $FILES; do
		if [ -d $FILE ]; then
			SETUP_DIR=`pwd`"/$FILE"
			echo "IM Server setup dir is: $SETUP_DIR"
			cp $LICENSE $SETUP_DIR'/packages/rcx/'
			echo 'license file '`pwd`"/$LICENSE installed to $SETUP_DIR/packages/rcx/"
		fi
	done
else
	echo 'This is IM client.'
fi

# Get OS, CPU, RAM info.
NEED_REBOOT=0
NEED_RELOGIN=0
OS=`cat /etc/system-release`
CORE_NUM=`cat /proc/cpuinfo | grep processor | wc -l`
echo 'OS is '$OS
echo 'CPU CORE NUM is '$CORE_NUM
cat /proc/meminfo | grep MemTotal

# Set ephemeral port range.
NET_SET_CMD='net.ipv4.ip_local_port_range = 1024 65535'
NET_SET_FILE='/etc/sysctl.conf'
NET_CHECK=`grep "$NET_SET_CMD" $NET_SET_FILE | wc -l`
# echo '$NET_CHECK is '$NET_CHECK
if [ $NET_CHECK -gt 0 ]; then
	echo 'Ephemeral port range is already 1024~65535, it is OK.'
else
	cp  $NET_SET_FILE $NET_SET_FILE'.bck'
	echo $NET_SET_CMD >> $NET_SET_FILE
	NUM=`sysctl -p | grep "$NET_SET_CMD" | wc -l`
	echo 'SET net.ipv4.ip_local_port_range RESULT IS '$NUM', DONE'
fi

# Set default language encoding.
LANG_FILE='/etc/locale.conf'
LANG_SET_CMD='LANG="en_US.UTF-8"'
LANG_CHECK=`grep "$LANG_SET_CMD" $LANG_FILE | wc -l`
if [ $LANG_CHECK -gt 0 ]; then 
	echo 'Default system language is already set, it is OK.'
else
	cp $LANG_FILE $LANG_FILE'.bck'
	echo $LANG_SET_CMD > $LANG_FILE
	source $LANG_FILE
	echo 'SET OS LANGUAGE, DONE'
fi

# Set file handle can be opened per process.
F_LMT_FILE='/etc/security/limits.conf'
F_CK_RST=`grep 'root soft nproc 655535' $F_LMT_FILE | wc -l `
if [ $F_CK_RST -gt 0 ]; then
	echo 'open file handle limit is already set, it is OK.'
else
	cp $F_LMT_FILE $F_LMT_FILE'.bck'
	echo -e 'soft nofile 65535\nhard nofile 65535\n* soft nproc 655535\n* hard nproc 655535\n* soft nofile 655535\n* hard nofile 655535\nroot soft nofile 655535\nroot hard nofile 655535\nroot soft nproc 655535\nroot hard nproc 655535' >> $F_LMT_FILE
	NEED_RELOGIN=1
	echo 'SET PROCESS FILE HANDLE DONE, you need re-login.'
fi
echo 'open files is '
echo `ulimit -a | grep 'open files'`

# Close Selinux core.
SEL_CONFIG=`cat /etc/selinux/config | grep 'SELINUX=disabled' | wc -l`
if [ $SEL_CONFIG -gt 0 ]; then	
	echo 'SELINUX is already disabled, it is OK.'
else
	echo "SELINUX NEED TO BE SET AS DISABLED"
	cp /etc/selinux/config /etc/selinux/config.bck
	sed -i 's/SELINUX=enforcing/SELINUX=disabled/' /etc/selinux/config
	sed -i 's/SELINUX=permissive/SELINUX=disabled/' /etc/selinux/config
	NEED_REBOOT=1
	echo 'SELINUX SET TO DISABLED, system need to be reboot.'
fi

# Stop system firewall.
FW_STATUS=`systemctl status firewalld`
FW_S_CHECK=`echo $FW_STATUS | grep 'Active:\\s*inactive' | wc -l`
if [ $FW_S_CHECK -gt 0 ]; then	
	echo 'Firewall is already closed, it is OK.'
else
	systemctl stop firewalld
	systemctl disable firewalld
	echo "FIREWALL CLOSED."
fi
# Install lrzsz
#echo 'check if lrzsz pre-installed.'
C_RZSZ=`rpm -qa | grep lrzsz | wc -l`
if [ $C_RZSZ -gt 0 ]; then
	echo 'lrzsz is already installed.'
else
	echo 'start to install lrzsz.'
	yum -y install lrzsz
	echo 'lrzsz DONE'
fi
# Install netstat
#echo 'check if netstat pre-installed.'
C_NETSTAT=`rpm -qa | grep net-tools | wc -l`
if [ $C_NETSTAT -gt 0 ]; then
	echo 'netstat is already installed.'
else
	echo 'start to install netstat.'
	yum -y install net-tools
	echo 'netstat DONE.'
fi
echo 'check if telnet pre-installed.'
# Install telnet
C_TELNET=`rpm -qa | grep telnet | wc -l`
if [ $C_TELNET -gt 0 ]; then
	echo 'telnet is already installed.'
else
	echo 'start to install telnet.'
	yum install xinetd telnet telnet-server -y
	echo 'telnet DONE.'
fi
# Install scp command
echo 'check if scp pre-installed.'
C_SCP=`rpm -qa | grep  openssh-clients | wc -l`
if [ $C_SCP -gt 0 ]; then
	echo 'scp is already installed.'
else
	echo 'start to install scp.'
	yum install openssh-clients -y
	echo 'scp DONE'
fi
# Stop supervisord if it exist on client.
if [ $IS_SERVER != "Y" ]; then
    echo 'supervisord need to be stopped on IM client.'
    systemctl stop supervisord
    echo 'supervisord stopped on IM client.'
fi

# Check if user need to relogin or system need to be reboot.
if [ $NEED_REBOOT -gt 0 ]; then
	echo 'SYSTEM NEED TO BE REBOOT.'
elif [ $NEED_RELOGIN -gt 0 ]; then
	echo 'CURRENT USER NEED TO LOG OUT AND LOG IN AGAIN.'
fi

# Start to setup IM Server.
if [ $SUPERVISORD_STATUS == 2 ]; then
	echo 'supervisord started, do not run setup script.'
elif [ $IS_SERVER == 'Y' ];then
	echo 'IM Server not started, start execute install script.'
	cd $SETUP_DIR
        ./run.sh
	cd ..
fi

# Setup JRE
if [ $IS_SERVER != "Y" ]; then
    echo 'start setup JRE for IM client.'
    JRE_TAR_BALL=`ls | grep "JRE.*\.tar\.gz$" -i`
	if [ -z "$JRE_TAR_BALL" ];then
		echo 'No JRE tar ball found, please check it again.'
		exit -1
	fi
	echo -e "JRE tar ball is: \033[31m$JRE_TAR_BALL\033[0m"
	JAVA_DIR=`pwd`'/jre'
	PROFILE='/etc/profile'
    PROFILE_CHECK=`grep "JAVA_HOME=$JAVA_DIR" $PROFILE | wc -l`
    if [ $PROFILE_CHECK -gt 0 ]; then
        echo 'jre is already set, it is OK.'
    else
        echo 'start to extract file from '$JRE_TAR_BALL
	    tar -zxf $JRE_TAR_BALL
        cp $PROFILE $PROFILE'.bck'
        echo -e "export JAVA_HOME=$JAVA_DIR\nexport PATH=\$JAVA_HOME/bin:\$PATH\nexport CLASSPATH=.:\$JAVA_HOME/lib/dt.jar:\$JAVA_HOME/lib/tools.jar" >> $PROFILE
        source /etc/profile
        java -version
    fi
    echo -n 'Now java cmd locate on: '
    echo `which java`
    echo 'JRE setup finished.'

fi
echo '######## poc test config finished. #######'
