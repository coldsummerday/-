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

class TcpController(object):
    def __init__(self,ip,port):
        self.socket_handle = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.socket_handle.bind((ip,port))
        self.socket_handle.listen(5)
    
    def connect(self):
        while True:
           accept_sock,addr = self.socket_handle.accept()
           connect_thread = threading.Thread(target=self.deal_with_message,args=(accept_sock,addr))
           connect_thread.start()
    def deal_with_message(self,sock,addr):
        while True:
            a=2


class Serial(object):
    def __init__(self,serial_port,baudrate):
        self.serial_handle = serial.Serial(port =serial_port,baudrate=baudrate)
        self.buffer = ''
        
    
    def read(self):
        data = ''
        while True:
            a=1

if __name__=="__main__":
    ros_handle = Ros()
    print('2')
    #ros_handle.init_listenner()
    listenner()
    print('init')
    while True:
        time.sleep(2)
        data = random.randint(0,5)
        ros_handle.led_publish(data)
	print('led')
	ros_handle.buzzer_publish(1)
	print('buzzer')
	time.sleep(1)    
            
        
        
