#! -*- coding:utf-8 -*-
import socket
import threading

import socket

# 创建一个socket:
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 建立连接:
s.connect(('192.168.199.172', 8888))

def tcplink(sock):

    recvthread = threading.Thread(target=tcprecv,args=(sock,))
    recvthread.start()
    while True:
        data = raw_input('input what you send! ')
        if data != 'exit':
            sock.send(data)
        else:
            break
def tcprecv(sock):
    while True:
        data = sock.recv(1024)
        if not data:
            break
        print("recv data:\t"+str(data)) 
        
while True:
    t1 = threading.Thread(target=tcplink,args=(s,))
    t1.start()