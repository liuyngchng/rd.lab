# 1. use mvn offline
```shell
 mvn clean compile  -s ~/user/.m2/settings.xml -Dencoding=UTF-8

```
settings.xml can be as following
```xml
<?xml version="1.0" encoding="UTF-8"?>
<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0 http://maven.apache.org/xsd/settings-1.0.0.xsd">

  <localRepository>~/user/.m2/repository</localRepository>
  <offline>true</offline>
  <mirrors>
    <mirror>
      <id>mirrorId</id>
      <mirrorOf>central</mirrorOf>
      <name>localMirror</name>
      <url>file:///home/$USER/.m2/repository</url>
    </mirror>
  </mirrors>
</settings>

```
若本地仓下的jar所在的文件夹（~/.m2/repository/aaa/bbb/ccc.jar）下存在文件"_remote.reposiories",
则需要删除， 否则会报
```error
Cannot access central (file://a/b/c) in offline mode
```
