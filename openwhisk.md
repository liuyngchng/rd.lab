# 1. golang setup
an architectureal view of apache openwhisk as [!this](https://thenewstack.io/behind-scenes-apache-openwhisk-serverless-platform/)
the document below can be foud [!doc](https://openwhisk.apache.org/documentation.html)
setup golang, Upgrade to Go 1.15
```
sudo apt-get install golang
```

setup godep, a go project dependency manage tool

```
sudo apt-get install go-dep
```
or `go get github.com/tools/godep`

# 2. install wsk Cli

OpenWhisk_CLI-1.1.0
```
mkdir openwhisk-cli
cd openwhisk-cli
wget https://github.com/apache/openwhisk-cli/releases/download/1.1.0/OpenWhisk_CLI-1.1.0-linux-amd64.tgz
sudo mv ./wsk /usr/local/bin/wsk
```

# 3. build openwhisk-wskdeploy 
# 3.1 get openwhisk-wskdeploy sourcecode from GitHub

```
mkdir go
cd go
export GOPATH=`pwd`
cd $GOPATH
go get github.com/apache/openwhisk-wskdeploy  # see known issues below if you get an error
go get github.com/tools/godep # get the dependency manager, a godep executable file produced in $gopath/bin
sudo cp bin/godep /usr/local/bin

```
# 3.2. build binary
```
cd src/github.com/apache/openwhisk-wskdeploy/
godep restore
go build -o wskdeploy
sudo cp wskdeploy /usr/local/bin
```
# 4. install openwhisk

```
git clone https://github.com/apache/openwhisk.git
cd openwhisk
 ./gradlew core:standalone:bootRun
```

to install npm
```
sudo apt-get install npm
```

# 5. troubleshooting
## 5.1 error downloagding dep
```
godep: error downloading dep (golang.org/x/sys/unix): unrecognized import path "golang.org/x/sys/unix"

```
to do 
```
mkdir $GOPATH/src/golang.org/x -p
cd $GOPATH/src/golang.org/x
git clone git@github.com:golang/text.git --depth=1
cd $GOPATH/src/
go install -x golang.org/x/text   //在$GOPATH/pkg目录下生成一个text.a的包文件
```
## 5.2 i/o timeout

```
go: github.com/apache/openwhisk-client-go@v0.0.0-20191018191012-ee5b8709787c: Get https://proxy.golang.org/github.com/apache/openwhisk-client-go/@v/v0.0.0-20191018191012-ee5b8709787c.mod: dial tcp 172.217.27.145:443: i/o timeout

```
to do  
Go1.12及以下：

Bash (Linux or macOS)

```
export GO111MODULE=on
export GOPROXY=https://goproxy.io
```
go1.13以上  
```
go env -w GO111MODULE=on
go env -w GOPROXY=https://goproxy.io,direct

```
