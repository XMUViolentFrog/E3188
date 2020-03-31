## 实验2 RS232C 通信
### 如何运行
执行 `demo.sh` 脚本或手动运行
### 手动运行
0. 在Linux环境编译
1. 创建pipe
```bash
mkfifo /tmp/vpipe1 /tmp/vpipe2
```
2. 运行程序
分别在两个窗口中执行
```bash
serial-talker /tmp/vpipe1 /tmp/vpipe2
```
```bash
serial-talker /tmp/vpipe2 /tmp/vpipe1
```
### 使用说明
* 程序的第1、2个参数分别用于指示发送、接受端
* 一次发生的字符串长不超过255
* 输入没有退格
* 按`Esc`退出
* 可以使用两个串口连接在虚拟机中使用
