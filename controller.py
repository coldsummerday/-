#!/usr/bin/env python
#!-*-coding:utf-8 -*-

import rospy
import std_msgs
import threading
import os
import socket
from time import sleep
import sys
import serial
import time
import random
import time


#超时多久发消息给手机端
TIME_OUT = 10


class Ros(object):
    def __init__(self,state_class):
        ##初始化ROS的两个发布消息的节点,分别向下发布LED跟蜂鸣信号
        rospy.init_node('tcp_server',anonymous=True)
        self.led_message_talker = rospy.Publisher("led",std_msgs.msg.Int32,queue_size=30)
        self.buzzer_message_talker = rospy.Publisher("buzzer",std_msgs.msg.Int32,queue_size=30)
        self.light_data = 0
	self.state_class = state_class

    ##灯光数据返回
    def light_callback(self,data):
        ##每次有光阻断信息时改变状态信息
	    print(self.state_class.box_change,self.state_class.boxopen_flag)
	    self.state_class.box_state = not self.state_class.box_state
	    self.state_class.box_change += 1
	    if self.state_class.box_change >=2:
	        self.state_class.boxopen_flag = not self.state_class.boxopen_flag
                self.state_class.box_change = 0
	    
    def init_listener(self):
        rospy.Subscriber("light_block",std_msgs.msg.Int32,self.light_callback)
        
    
    #向arduino发控制led灯的信息
    def led_publish(self,data):
        self.led_message_talker.publish(data)
    
    #发蜂鸣信号
    def buzzer_publish(self,data):
        self.buzzer_message_talker.publish(data)

    def get_light_data(self):
        return self.light_data

class Flag(object):
    def __init__(self):
        self.buzzer_flag = False
        self.boxopen_flag = False
        #python2没有枚举类,所以用int值表示led状态,1:闪,代表到时没吃药,2,灭,代表药盒没药,3,黄:药品进盒等待 4:红,准备到吃药时间
        self.led_state = 0
        ##标记tcp是否断开,否则将会一直推送
        self.tcp_flag = False
        self.send_flag = False
        ##改变两次代表一次开盒子过程
	    self.box_change = 0
        ##false代表盒子关闭
	    self.box_state = False
    
    def reset(self):
        self.boxopen_flag = False
        self.buzzer_flag = False
        self.led_state = 0
        self.tcp_flag = False
        self.send_flag = False


class Serial_read(object):
    ##串口读取类,读取串口中的温度传感器信息
    def __init__(self,serial_port,baudrate):
        self.serial_handle = serial.Serial(port =serial_port,baudrate=baudrate)
        self.buffer = ''
        
    
    def read(self):
        data = ''
        while True:
	   
	    data = self.serial_handle.readline()
            if  data:		
		if data[0] != 't':
		    continue
		temperature = data.split('     ')[1][:-1]
		self.buffer = temperature



def sockinit():
    #tcp通信初始化
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)    
    s.bind(('192.168.192.3',8888))
    s.listen(1)
    return s
def tcplink(sock,addr,ros_handle):
    global state_class
    while True:
        try:
            data = sock.recv(1024)
	    print(data)
        except :
            break
        if data == 'exit' or not data:
            state_class.tcp_flag = False
            state_class.reset()
            break
        if data == 't':
            tdata = sock.recv(1024)
	        print(str(tdata)+str(type(tdata)))
	        tdata = float(long(tdata)/1000.0)
            ##开启一次计时吃药提醒函数
            t2 = threading.Thread(target=threadingtest,args=(ros_handle,tdata,sock,))
            t2.start()
        if data == 'b':
            ros_handle.buzzer_publish(1)
            state_class.boxopen_flag = True

def threadingtest(ros_handle,eat_time,sock):
    global state_class
    state_class.boxopen_flag = False
    state_class.send_flag = False
    ros_handle.led_publish(3)
    state_class.led_state = 3
    if state_class.box_state:
	    sock.send('e' + 20 *'-')
    nowtime = time.time()
    ##吃药前5秒 ,闪红灯
    while nowtime <eat_time - 5:
        time.sleep(1)
        nowtime = time.time()
    ros_handle.led_publish(4)
    state_class.led_state = 4
    nowtime = time.time()
    while nowtime < eat_time:
        time.sleep(1)
        nowtime = time.time()
    #到了吃药时间,没开盒子就发蜂鸣信号
    ros_handle.led_publish(1)
    send_flag = False
    while not state_class.boxopen_flag:
        ros_handle.buzzer_publish(1)
        time.sleep(1)
        nowtime = time.time()

        ##超过延时时间就给手机推送消息
        if (nowtime-eat_time) >TIME_OUT:
            until_time ='t-' + time.strftime("%Y-%m-%d-%H-%M-%S", time.localtime())
            if not send_flag:
	        sock.send(str(until_time))
		send_flag = True
            
    ros_handle.led_publish(2)
    state_class.led_state = 2
    return
def temperature_thread(sock,serial_handle):
    ##温度读取并定时发送温度消息
    global state_class
    while True:
        try:
	        senddata='c-'+str(serial_handle.buffer)+'-'+13*'s'
	        sock.send(senddata)
        except Exception as e:
            break
        time.sleep(5)
        if not state_class.tcp_flag:
	        state_class.boxopen_flag = False
            print("connect close!")
            break 
        
if __name__=="__main__":
    state_class = Flag()
    ros_handle = Ros(state_class)
    ros_handle.init_listener()
    server_socket = sockinit()
    serial_handle = Serial_read('/dev/ttyUSB0',9600)
    ##开启一个线程专门更新串口中温度信息
    serial_thread = threading.Thread(target=serial_handle.read,args=())
    serial_thread.setDaemon(True)
    serial_thread.start()
    
    print('init wait for connect')
    while True:
        s,addr =  server_socket.accept()
	    print(addr,'connecting')
        state_class.tcp_flag = True
        ##一个线程处理与客户端的交互
        ##另一个线程处理定时发送温度消息
        t2 = threading.Thread(target=temperature_thread,args=(s,serial_handle,))
        t1 = threading.Thread(target=tcplink,args=(s,addr,ros_handle,))
        t1.setDaemon(True)
        t1.start()
        t2.setDaemon(True)
        t2.start()
   
        
        

