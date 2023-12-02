# cmd

## 查看端口占用

```powershell
# 获取进程ID
netstat -ano |findstr 9222
# 根据进程 ID 查看进程信息
tasklist | findstr "1912"
```



## chrome启动debug

```powershell
start "" "C:\Program Files\Google\Chrome\Application\chrome.exe" --user-data-dir="C:\tagui\src\chrome\tagui_user_profile" --remote-debugging-port=9222 about:blank --window-size=1366,842


"C:\Program Files\Google\Chrome\Application\chrome.exe" --user-data-dir="C:\tagui\src\chrome\tagui_user_profile" --remote-debugging-port=9222 about:blank --window-size=1366,842
```

## 查询命令使用方式

```powershell
cmd /?
```

## 查询命令列表

```
help
```

# OpenSSH

## 启用

对于windows10， 设置->应用->管理可选功能->OpenSSH 服务器， OpenSSH客户端

## 修改默认shell

将默认的shell 从cmd 修改为 powershell

Windows 初始默认 Shell 是 cmd，Windows 还包括了 PowerShell 和 Bash。

先确认 OpenSSH 安装文件夹位于系统路径上，对于 windows，默认安装位置是：`C:Windows\System32\openssh`。

cmd 中 `path`和 PowerShell 中 `$env:path`可以显示当前路径。

在注册表中，将 PowerShell 的完整路径添加到 `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\OpenSSH`中的 `DefaultShell`字符串值中。

先确保注册表中存在 `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\OpenSSH`这个路径，然后可以在 PowerShell 中用以下命令将默认 shell 设为 PowerShell：

```powershell
New-ItemProperty -Path "HKLM:\SOFTWARE\OpenSSH" -Name DefaultShell -Value "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -PropertyType String -Force
```

