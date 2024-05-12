#!/bin/bash

# 启动服务器并放到后台运行
./server &
# 获取服务器进程的 PID
SERVER_PID=$!
CUR_PID=$$
echo "last pid:$SERVER_PID"
echo "current pid:$CUR_PID"

# 等待一秒钟，让服务器有足够的时间启动
sleep 1

# 启动客户端，连接到 localhost
./client localhost

# 等待客户端程序结束
wait $!

# 当客户端结束后，也关闭服务器进程
kill $SERVER_PID
