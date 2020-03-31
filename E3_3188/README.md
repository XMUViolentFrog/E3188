# 实验3 侦听以太网和IP报文
## 编译
```bash
gcc packet_peek.c -o packet_peek -lpcap
```
## 运行
```bash
sudo ./packet_peek
```
## 使用
* 按q（小写）退出
* 自动选择设备，有需要自行修改
* 文件默认保存为./log.csv
* 控制台默认以1Mbps为上限进行提示