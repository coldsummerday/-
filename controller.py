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

class Ros(object):
    def __init__(self):
        rospy.init_node('tcp_server',anonymous=True)
        self.led_message_talker = rospy.Publisher("led",std_msgs.msg.Int32,queue_size=30)
        self.buzzer_message_talker = rospy.Publisher("buzzer",std_msgs.msg.Int32,queue_size=30)
        self.light_data = 0

    ##灯光数据返回
    def light_callback(self,data):
        self.light_data = data.data
	    print('light_block'+str(data.data))
    
    def init_listenner(self):
        rospy.Subscriber("light_block",std_msgs.msg.Int32,self.light_callback)
        rospy.spin()
    
    def light_block_listenner(self):
	    rospy.Subscriber("light_block",std_msgs.msg.Int32,self.light_block_callback)
        rospy.spin()
    def light_block_callback(self,data):
	    print('light_block'+str(data.data))
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
    s.bind(('192.168.191.2',8888))
    s.listen(1)
    return s

def tcplink(sock,addr,ros_handle):
    while True:
        data = sock.recv(1024)
        if data == 'exit' or not data:
            break
        if data == 't':
            tdata = sock.recv(1024)
            ros_handle.led_publish(int(tdata))
        if data == 'b':
            ros_handle.buzzer_publish(1)
            

if __name__=="__main__":
    ros_handle = Ros()
    server_socket = sockinit()
    while True:
        s,addr =  server_socket.accept()
        t1 = threading.Thread(target=tcplink,args=(s,addr,ros_handle,))
        t1.start()
    
        
        
