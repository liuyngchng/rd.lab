FROM centos 
FROM ubuntu 
VOLUME /tmp
WORKDIR /opt
ADD ./_srvr  srvr
ADD ./cfg.ini  cfg.ini
ENV LC_ALL "C.UTF-8"
ENV LANG "C.UTF-8"
#RUN bash -c 'rm /etc/localtime'
RUN bash -c 'ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime'
WORKDIR /opt
RUN bash -c 'pwd'
RUN bash -c 'ls -lh'
EXPOSE 8083
ENTRYPOINT ["./srvr"]
