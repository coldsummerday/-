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

TIME_OUT = 10


class Ros(object):
    def __init__(self):
        rospy.init_node('tcp_server',anonymous=True)
        self.led_message_talker = rospy.Publisher("led",std_msgs.msg.Int32,queue_size=30)
        self.buzzer_message_talker = rospy.Publisher("buzzer",std_msgs.msg.Int32,queue_size=30)
        self.light_data = 0

    ##灯光数据返回
    def light_callback(self,data):
        self.light_data = data.data
    
    def init_listenner(self):
        rospy.Subscriber("light_block",std_msgs.msg.Int32,self.light_callback)
        rospy.spin()
    
    def led_publish(self,data):
        self.led_message_talker.publish(data)
    
    def buzzer_publish(self,data):
        self.buzzer_message_talker.publish(data)

    def get_light_data(self):
        return self.light_data
def callback(data):
    print('light:'+str(data.data))

def listenner():
    rospy.Subscriber("light_block",std_msgs.msg.Int32,callback)
    rospy.spin()



class Flag(object):
    def __init__(self):
        self.buzzer_flag = False
        self.boxopen_flag = False
        #python2没有枚举类,所以用int值表示led状态,1:闪,代表到时没吃药,2,灭,代表药盒没药,3,黄:药品进盒等待 4:红,准备到吃药时间
        self.led_state = 0
        ##标记tcp是否断开,否则将会一直推送
        self.tcp_flag = False
        self.send_flag = False
    
    def reset(self):
        self.boxopen_flag = False
        self.buzzer_flag = False
        self.led_state = 0
        self.tcp_flag = False
        self.send_flag = False


class Serial(object):
    def __init__(self,serial_port,baudrate):
        self.serial_handle = serial.Serial(port =serial_port,baudrate=baudrate)
        self.buffer = ''
        
    
    def read(self):
        data = ''
        while True:
            a=1

def sockinit():
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)    
    s.bind(('192.168.191.2',8888))
    s.listen(1)
    return s
def tcplink(sock,addr,ros_handle):
    global state_class
    while True:
        try:
            data = sock.recv(1024)
        except :
            break
        if data == 'exit' or not data:
            state_class.tcp_flag = False
            state_class.reset()
            break
        if data == 't':
            tdata = sock.recv(1024)
            t2 = threading.Thread(target=threadingtest,args=(ros_handle,float(tdata),sock,))
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
    nowtime = time.time()
    while nowtime <eat_time - 5:
        time.sleep(1)
        nowtime = time.time()
    ros_handle.led_publish(4)
    state_class.led_state = 4
    nowtime = time.time()
    while nowtime < eat_time:
        time.sleep(1)
        nowtime = time.time()
    ros_handle.led_publish(1)
    while not state_class.boxopen_flag:
        ros_handle.buzzer_publish(1)
        time.sleep(1)
        nowtime = time.time()
        if (nowtime-eat_time) >TIME_OUT and not state_class.send_flag:
            until_time ='t-' + time.strftime("%Y-%m-%d-%H-%M-%S", time.localtime())
            sock.send(str(until_time))
            state_class.send_flag = True
        
    ros_handle.led_publish(2)
    state_class.led_state = 2
    
def delayToLed(time,data,ros_handle):
    nowtime = time.time()
    while nowtime<time:
        time.sleep(1)
        nowtime = time.time()
    ros_handle.led_publish(data)

def temperature_thread(sock,serial_handle):
    global state_class
    while True:
        try:
            print('send temp!')
            sock.send('c-'+19*'s')
        except:
            print("connect close!")
            break
        time.sleep(5)
        if not state_class.tcp_flag:
            print("connect close!")
            break 
        
if __name__=="__main__":
    ros_handle = Ros()
    state_class = Flag()
    server_socket = sockinit()
    while True:
        s,addr =  server_socket.accept()
        state_class.tcp_flag = True
        t2 = threading.Thread(target=temperature_thread,args=(s,1,))
        t1 = threading.Thread(target=tcplink,args=(s,addr,ros_handle,))
        t1.setDaemon(True)
        t1.start()
        t2.setDaemon(True)
        t2.start()
    
        
        

