#! -*- coding:utf-8 -*-
import socket
import threading

import socket
import time
# 创建一个socket:
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 建立连接:
s.connect(('192.168.199.172', 8888))
tcp_flag = True
def tcplink(sock):

    recvthread = threading.Thread(target=tcprecv,args=(sock,))
    recvthread.start()
    while True:
        data = raw_input('input what you send! ')
        if data != 'exit':
            sock.send(data)
            if data == 't':
                sock.send(str(time.time()+30.0))
        else:
            tcp_flag = False
            break
def tcprecv(sock):
    while True:
        data = sock.recv(1024)
        if not data:
            break
        print("recv data:\t"+str(data)) 
        
while tcp_flag:
    t1 = threading.Thread(target=tcplink,args=(s,))
    t1.daemon()
    t1.start()
    t2 = threading.Thread(target=tcprecv,args=(s,))
    t2.daemon()
    t2.start()
    