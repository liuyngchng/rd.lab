# use mvn offline
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

#  pom config

```xml
<project>
	<dependencyManagement></dependencyManagement>
	<repositories>
        <repository>
            <id>public</id>
            <name>private nexus</name>
<!--            <url>https://maven.aliyun.com/repository/public</url>-->
            <url>http://mvnrepository.com</url>
            <releases>
                <enabled>true</enabled>
            </releases>
        </repository>
    </repositories>
</project>
```

# mvn setting

```sh
which mvn
/usr/bin/mvn
```

in file /etc/maven/settings.xml

```xml
<?xml version="1.0" encoding="UTF-8"?>
<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0 http://maven.apache.org/xsd/settings-1.0.0.xsd">
	<localRepository>/home/rd/.m2/repository</localRepository>
    <mirrors>
        <mirror>
            <id>alimaven</id>
            <name>aliyun maven</name>
            <!--<url>http://127.0.0.1:8081/repository/maven-public/</url> --> 
            <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
            <mirrorOf>central</mirrorOf>
        </mirror>
    </mirrors>
</settings>
```

# 单独构建子模块

对于父子工程，由于项目下子模块很多，如果直接用parent下的pom文件打包（`mvn clean package`），会将所有子模块打包，耗时较长。如果只想打包某一个子模块，可单独进行构建，执行如下命令

```sh
mvn clean package -pl child-module-name -am
```

例如，目前parent `pom.xml` 中有以下几个 `module`

```xml
<modules>
    <module>a</module>
    <module>b</module>
    <module>c</module>
    <module>d</module>
</modules>
```

其中 c 模块依赖 b 模块， 那么在根 pom.xml 所在目录下使用命令 

```sh
mvn clean package -pl c -am
```

将会单独构建模块c , 同时构建了 模块 c 依赖的模块 b。
