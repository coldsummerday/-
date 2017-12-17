#!/usr/bin/env python
#!-*-coding:utf-8 -*-

import rospy
import std_msgs
import threading

def callback(data):
    print("recv data:"+str(data.data))

def listenner():
    rospy.init_node('listenner',anonymous = True)
    rospy.Subscriber("light",std_msgs.msg.Int32,callback)
    rospy.spin()

if __name_=="__main__":
    listenner()