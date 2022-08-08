# doc
https://sikulix-2014.readthedocs.io/en/latest/toc.html
# keyboard cmd

##  使用底层函数模拟触发键盘

keyDown(Key.WIN)

##  按键

| 键名             | sikuliX中的命令                               |
| :--------------- | :-------------------------------------------- |
| Shift            | type(Key.SHIFT)                               |
| Ctrl             | type(Key.CTRL)                                |
| Alt键            | type(Key.ALT)                                 |
| Tab(制表符)      | type(Key.TAB)                                 |
| 大写锁定         | type(Key.CAPSLOCK)                            |
| 输入             | type(Key.ENTER)                               |
| 删除             | type(Key.DELETE)                              |
| 退格键           | type(Key.BACKSPACE)                           |
| 插入             | type(Key.INSERT)                              |
| Home             | type(Key.HOME)                                |
| End              | type(Key.END)                                 |
| ESC              | type(Key.ESC)                                 |
| 空格             | type(Key.SPACE)                               |
| 视窗             | type(Key.WIN)                                 |
| 换页             | type(Key.PAGE_UP)                             |
| 向下翻页         | type(Key.PAGE_DOWN)                           |
| 列印画面         | type(Key.PRINTSCREEN)                         |
| 数字键盘锁       | type(Key.NUM_LOCK)                            |
| 滚动锁           | type(Key.SCROLL_LOCK)                         |
| F1               | type(Key.F1)                                  |
| F2               | type(Key.F2)                                  |
| F3               | type(Key.F3)                                  |
| F4               | type(Key.F4)                                  |
| F5               | type(Key.F5)                                  |
| F6               | type(Key.F6)                                  |
| F7               | type(Key.F7)                                  |
| F8               | type(Key.F8)                                  |
| F9               | type(Key.F9)                                  |
| F10              | type(Key.F10)                                 |
| F11              | type(Key.F11)                                 |
| F12              | type(Key.F12)                                 |
| 光标键：向上     | type(Key.UP)                                  |
| 光标键：向下     | type(Key.DOWN)                                |
| 光标键：左       | type(Key.LEFT)                                |
| 光标键：向右     | type(Key.RIGHT)                               |
| 向上移动光标两次 | type(Key.UP * 2)                              |
| 向下移动光标两次 | type(Key.DOWN * 2)                            |
| 向左移动光标两次 | type(Key.LEFT * 2)                            |
| 向右移动光标两次 | type(Key.RIGTH * 2)                           |
| 按特定键         | KeyDown(Key.*) 需要使用Region()进行区域设置。 |
| 释放特定键       | KeyUp(Key.*) 需要使用Region()设置区域。       |

# 快捷键

| 常见操作                                                     | 快捷键                                             | sikuliX中的命令                                              |
| :----------------------------------------------------------- | :------------------------------------------------- | :----------------------------------------------------------- |
| 复制                                                         | Ctrl + C                                           | type("c" , Key.CTRL)                                         |
| 剪下                                                         | Ctrl + X                                           | type("x" , Key.CTRL)                                         |
| 贴上                                                         | Ctrl + V                                           | type("v" , Key.CTRL)                                         |
| 搜索                                                         | Ctrl + F                                           | type("f" , Key.CTRL)                                         |
| 打印                                                         | Ctrl + P                                           | type("p" , Key.CTRL)                                         |
| 撤消                                                         | Ctrl + Z                                           | type("z" , Key.CTRL)                                         |
| 重做                                                         | Ctrl + Y Ctrl + Shift + Z                          | type("y" , Key.CTRL); type("z" , Key.CTRL + Key.SHIFT)       |
| 关闭标签                                                     | Ctrl + F4                                          | type(Key.F4 , Key.CTRL)                                      |
| 还原标签                                                     | Ctrl + Shift + T                                   | type("t" , Key.CTRL + Key.SHIFT)                             |
| 浏览器缓存刷新(强制刷新)                                     | Ctrl + F5                                          | type(Key.F5 , Key.CTRL)                                      |
| 关闭窗口                                                     | Alt + F4                                           | type(Key.F4 , Key.ALT)                                       |
| 最小化所有窗口                                               | Windows + M                                        | type("m" , Key.WIN)                                          |
| 显示桌面                                                     | Windows + D                                        | type("d" , Key.WIN)                                          |
| 最小化窗口(当窗口为正常大小时)恢复为原始大小(当窗口为最大化时) | Windows + 光标键：向下,如果最大化,请执行上述两次。 | type(Key.DOWN , Key.WIN)                                     |
| 窗口最大化                                                   | Windows +光标键：向上                              | type(Key.UP , Key.WIN)                                       |
| 切换活动窗口                                                 | Windows + Tab; Windows + Shift + Tab               | type(Key.TAB , Key.WIN); type(Key.TAB , Key.WIN + Key.SHIFT) |
| 启动任务管理器                                               | Ctrl + Shift + ESC                                 | type(Key.ESC , Key.CTRL + Key.SHIFT)                         |
| 启动资源管理器                                               | Windows + e                                        | type("e" , Key.WIN)                                          |

# 运算符

| 运算           | sikuliX中运算符 |
| :------------- | :-------------- |
| 加成           | +               |
| 减法           | –               |
| 乘法           | *               |
| 除             | /               |
| 余数计算       | ％              |
| 除数的整数部分 | //              |
| A的b次方       | A ** B          |
| A的负数        | -A              |
| 逻辑或         | or              |
| 逻辑与         | and             |
| 否认           | not             |
| a大于b         | a > b           |
| a小于b         | a < b           |
| a小于或等于b   | a <= b          |
| a大于或等于b   | a >= b          |
| a等于b         | a == b          |
| a不等于b       | a != b          |

# 鼠标动作

常见鼠标操作如下所示。

| 操作         | cmd                            |
| ------------ | ------------------------------ |
| 鼠标左键单击 | click(Location(123, 456))      |
| 鼠标左键双击 | doubleClick()                  |
| 鼠标右键单击 | rightClick(Location(123, 456)) |
| 鼠标悬停     | hover()                        |
| 鼠标拖拽     | dragDrop()                     |
| 按下鼠标左键 | mouseDown(Button.LEFT)         |
| 抬起鼠标按键 | mouseUp(Button.LEFT)           |



```python
image = ("1565939082284.png")
t1 = find(image)
hover("1565939082284.png")				#鼠标悬停
mouseDown(Button.RIGHT)					#按住鼠标右键
mouseMove(t1.left(122))					#往左拖动
#dragDrop(t,Location(t.x, t.y - 10))  	#向上拖动10的位置
wait(1) 
mouseUp()								#抬起鼠标右键
image = ("1565939082284.png")
t2 = find(image)
hover("1565939082284.png")				#鼠标悬停
mouseDown(Button.RIGHT)					#按住鼠标右键
mouseMove(t2.above(398))				#往上拖动
#dragDrop(t,Location(t.x, t.y - 10))  	#向上拖动10的位置
wait(1)
mouseUp()								#抬起鼠标右键
```

# 其他

```shell
waitVanish(img)						# 等待img 消失
wait(img)									# 等待 img 出现
type("txt")								# 在光标焦点处输入 txt
type(img, "txt")					# 在 img 出现的位置，输入文本 txt
# 在 img 出现的位置，设置一定的偏移量，输入文本 txt
type(Pattern(img).targetOffset(-100, -1), "txt"); 
paste("txt")							# 在光标焦点处粘贴 txt
paste(img, "txt")					# 单击指定的图片 img， 再输入文本 txt

r=Region(img)
r.click()									# 在指定的区域 r 中点击

```



# 拷贝粘贴文本

```python
r=Region(184,324,1233,479)
param=r.find(Pattern("1659332267414.png").targetOffset(-71,-6))
wait(1)
hover(param)
wait(1)
mouseDown(Button.LEFT)
wait(1)
mouseMove(500, 0)
wait(1)
mouseUp(Button.LEFT)
type("c", Key.CTRL)
wait(1)
type("t", Key.CTRL+Key.ALT)
wait(2)
type("v", Key.CTRL+Key.SHIFT)
type(Key.ENTER)
wait(2)
type(Key
```

# 日志

```python
Debug.on(3)
Debug.setUserLogFile("/home/rd/workspace/sikulix/log")		# 设置日志文件的绝对路径
Debug.user("text with %s", "test")							# 打印日志，使用 C语言风格占位符， or said Java String.format()
```

# appData

修改源代码 Sikulix1.API.org.sikuli.support.Commons.getAppDataPath()

