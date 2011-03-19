#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       untitled.py
#       
#       Copyright 2010 Jeffrey Minton <ffej@blinking-book>
#       
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#       
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.

import blob
import create
import time
import cv

def main():
	
	blobFind = blob.blobit()
	robot = create.Create(3)
	robot.toFullMode()
	
	obj = blobFind.whereObject()
	while(obj == None):
		robot.setWheelVelocities(10, -10)
		time.sleep(.7)
		robot.go(0)
		obj = blobFind.whereObject()
		
	for i in range(20):
		if(obj == 'r' or obj == 'l'):
			objOld = obj
			centered = False
			while(centered == False):
				turn(objOld, robot)
				obj = blobFind.whereObject()
				if(obj == objOld + 'c'):
					centered = True
		if(obj == 'rc' or obj == 'lc'):
			robot.go(10)
			time.sleep(.7)
			robot.go(0)
		obj = blobFind.whereObject()
		
	finished = False
		
	while(finished == False):
		k = cv.WaitKey(5)
		if k % 0x100 == 27:
			finished = True

	return 0
	

def turn(dir, robot):
	if(dir == 'r'):
		robot.setWheelVelocities(10, -10)
		time.sleep(.3)
		robot.go(0)
	elif(dir == 'l'):
		robot.setWheelVelocities(-10, 10)
		time.sleep(.3)
		robot.go(0)


def oppDir(dir):
	if(dir == 'r'):
		return 'l'
	elif(dir == 'l'):
		return 'r'
	elif(dir == 'lc'):
		return 'rc'
	elif(dir == 'rc'):
		return 'lc'
	
	return 'x'
if __name__ == '__main__':
	main()
