# 实验4 解析TCP段和FTP数据报
## 编译
```bash
gcc ftp-dump.c -o ftp-dump -lpcap
```
## 运行
```bash
sudo ./packet_peek
```
## 使用
* 按q（小写）退出
* 自动选择设备，有需要自行修改
* 文件默认保存为./log.csv
* 作为试验，程序只处理TCP端口21的数据包