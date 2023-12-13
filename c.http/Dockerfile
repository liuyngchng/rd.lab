#FROM centos 

FROM rd/ssl:1.0.0 
#FROM ubuntu:20.04 
#RUN bash -c 'apt-get update'
#RUN bash -c 'apt-get install openssl -y'

RUN bash -c 'pwd'
VOLUME /tmp
WORKDIR /opt
ADD ./_tcp  _tcp
ADD ./_tls  _tls
RUN bash -c 'mkdir tls'
WORKDIR /opt/config
ADD ./config/cfg.ini    cfg.ini
ADD ./config/ca.crt     ca.crt
ADD ./config/ca.key     ca.key
ENV LC_ALL "C.UTF-8"
ENV LANG "C.UTF-8"
#RUN bash -c 'rm /etc/localtime'
RUN bash -c 'ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime'
WORKDIR /opt
RUN bash -c 'pwd'
RUN bash -c 'ls -lh'
EXPOSE 8083
ENTRYPOINT ["./_tls"]
