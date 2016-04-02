# 屙屎外部编辑器替换工具

## 原理

经测试，osu!编辑器的『在记事本中打开』这一项调用的命令是『notepad.exe 文件名』，因此与游戏可执行文件同级的notepad.exe会优先于系统记事本被调用，因此只要让游戏目录下的notepad.exe执行我们想要的命令就可以了。

## 使用本工具

### 编译

需要本机拥有MinGW编译环境，并且包含Windows API组件。

```
mingw32-make
```

### 使用

1. 复制customrunner.exe、customrunner.txt到游戏安装目录
2. 重命名customrunner.exe为notepad.exe
3. 编辑customrunner.txt，输入你的目标编辑器命令，如`"C:\Program Files (x86)\Notepad++\Notepad++.exe"`

PS. 轻量级的、只有单个文件的编辑器，你可以直接把它拉来改名为notepad.exe即可