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

