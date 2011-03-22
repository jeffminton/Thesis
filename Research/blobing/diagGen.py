#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#       untitled.py
#       
#       Copyright 2011 Jeffrey Minton <ffej@blinking-book>
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


img5x5 = [[[120, 111, 82],[118, 109, 80],[117, 108, 79],[50, 70, 79],[51, 71, 81]],
[[121, 112, 83],[120, 111, 82],[51, 70, 80],[50, 72, 81],[49, 69, 82]],
[[121, 112, 83],[50, 111, 201],[50, 109, 200],[51, 115, 204],[67, 200, 100]],
[[200, 50, 90],[225, 104, 67],[50, 115, 202],[73, 112, 201],[72, 100, 203]],
[[201, 70, 100],[100, 234, 80],[103, 231, 79],[99, 229, 78],[98, 228, 77]]]

import cv
import time
from unionfind import *

class blobit():
	def __init__(self):
		0
		#~ self.width = 320
		#~ self.height = 240
		#~ self.imgCnt = 0
		#~ #img = cv.LoadImage("0.bmp")
		#~ cv.NamedWindow("orig")
		#~ cv.NamedWindow("img")
		#~ cv.NamedWindow("blobimg")
		#~ self.cap = cv.CaptureFromCAM(0)
		#~ print "setting props"
		#~ cv.SetCaptureProperty(self.cap, cv.CV_CAP_PROP_FRAME_WIDTH, self.width)
		#~ cv.SetCaptureProperty(self.cap, cv.CV_CAP_PROP_FRAME_HEIGHT, self.height)

	def gen5x5(self):
		r = img5x5[:][:][2]
		g = img5x5[:][:][1]
		b = img5x5[:][:][0]
		
		for y in range(5):
			for x in range(5):
				b = img5x5[x][y][2]
				img5x5[x][y][2] = img5x5[x][y][0]
				img5x5[x][y][0] = b
					
		imgIn = cv.LoadImage("segOrig.jpg")
		imgIn = imgIn[200:205, 200:205]
		
		for y in range(imgIn.height):
			for x in range(imgIn.width):
				#~ print "y: ", y
				#~ print "x: ", x
				#~ print(img5x5[x][y])
				imgIn[x, y] = img5x5[x][y]
		return imgIn
	
	def whereObject(self):
		orig = cv.QueryFrame(self.cap)
		orig = cv.QueryFrame(self.cap)
		
		centerThresh = ((self.width / 2) - 50, (self.width / 2) + 50)
		center = self.width / 2
		
		img = cv.CreateImage((self.width, self.height), cv.IPL_DEPTH_8U, 3)
		cv.Smooth(orig, img, cv.CV_GAUSSIAN, 5)
		
		blobImg, region2color, region2pix = self.findBlob(img, 9)
		
		color = (75, 0, 0)
		region = self.findColor(color, blobImg, region2color, region2pix)
		
		if(region != None):
			print "best color: ", region2color[region]
			
			regionMid = self.midMass(region2pix, region)
			print "Middle of Region: ", regionMid
			
			black = False
			for pix in region2pix[region]:
				if(black == True):
					blobImg[pix[0], pix[1]] = (0, 0, 0)
					black = False
				else:
					blobImg[pix[0], pix[1]] = (255, 255, 255)
					black = True
			
			blobImg[regionMid[0], regionMid[1]] = (0, 0, 255)
			
			#~ cv.ShowImage("orig", orig)
			#~ cv.ShowImage("img", img)	
			cv.ShowImage("blobimg", blobImg)
			
			cv.SaveImage("blob" + str(self.imgCnt) + ".bmp", blobImg)
			cv.SaveImage("orig" + str(self.imgCnt) + ".bmp", orig)
			cv.SaveImage("img" + str(self.imgCnt) + ".bmp", img)
			self.imgCnt += 1
			
			#too far left
			if(regionMid[1] < centerThresh[0]):
				return 'l'
			#too far right
			elif(regionMid[1] > centerThresh[1]):
				return 'r'
			#left of center
			elif(regionMid[1] > centerThresh[0] and regionMid < center):
				return 'lc'
			#right of center
			elif(regionMid[1] < centerThresh[1] and regionMid > center):
				return 'rc'
			
		cv.SaveImage("blob" + str(self.imgCnt) + ".bmp", blobImg)
		cv.SaveImage("orig" + str(self.imgCnt) + ".bmp", orig)
		cv.SaveImage("img" + str(self.imgCnt) + ".bmp", img)
		self.imgCnt += 1
		
		return None
		

	def midMass(self, region2pix, region):
		maxX = region2pix[region][0][1]
		minX = region2pix[region][0][1]
		maxY = region2pix[region][0][0]
		minY = region2pix[region][0][0]
		
		for coord in region2pix[region]:
			if(coord[1] > maxX):
				maxX = coord[1]
			if(coord[1] < minX):
				minX = coord[1]
			if(coord[0] > maxY):
				maxY = coord[0]
			if(coord[0] < minY):
				minY = coord[0]
				
		midX = (minX + maxX) / 2
		midY = (minY + maxY) / 2
		
		return (midY, midX)

	def findColor(self, color, blobImg, region2color, region2pix):
		bestKey = None
		bestColor = None
		color = {"blue" : color[2], "green" : color[1], "red" : color[0]}
		prim = None
		sec = []
		
		#get the primary and secondary colors
		for key in color.keys():
			if(prim == None):
				prim = key
			elif(color[key] > color[prim]):
				prim = key
		for key in color.keys():
			if(key != prim):
				sec.append(key)
				
		for key in region2color.keys():
			regColor = region2color[key]
			regColor = {"blue" : regColor[0], "green" : regColor[1], "red" : regColor[2]}
			offset = regColor[prim] - color[prim]
			newRegColor = {"blue" : regColor["blue"] - offset, "green" : regColor["green"] - offset, "red" : regColor["red"] - offset}
			
			if(newRegColor[prim] >= color[prim] and newRegColor[sec[0]] <= color[sec[0]] and newRegColor[sec[1]] <= color[sec[1]]):
				if(bestKey == None):
					bestKey = key
				elif(len(region2pix[key]) > len(region2pix[bestKey])):
					bestKey = key
					
		return bestKey

	def findBlob(self, img, threshold):
		#pixMap = [(0, -1), (-1, 0), (0, 1), (1, 0)]
		#pixMap = [(0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1)]
		#pixMap = [(0, -1), (-1, 0)]
		uf = UnionFind()
		west = (0, -1)
		north = (-1, 0)
		region2pix = {}
		region2color = {}
		pix2region = {}
		regionCnt = 0
		found = False
		matchWest = False
		matchNorth = False
		mask = []
		
		'''	Pass One	'''
		print "Pass One"
		for y in range(img.height):
			for x in range(img.width):
		#~ for y in range(img.height):
			#~ for x in range(img.width):
				matchWest = False
				matchNorth = False
				pixCheck = [y, x]
				#get the address of the pixel to compare to
				pixWest = self.getPixOffset(pixCheck, west, img)
				pixNorth = self.getPixOffset(pixCheck, north, img)
				
				#check if the average color distance is less than threshold
				if(self.checkPixels(pixCheck, pixWest, img, threshold) == True):
					if(pix2region.has_key(str(pixWest))):	
						#set pixCheck to the same region as pixWest
						matchWest = True
						pix2region[str(pixCheck)] = pix2region[str(pixWest)]
				
				if(self.checkPixels(pixCheck, pixNorth, img, threshold) == True):
					if(pix2region.has_key(str(pixNorth))):
						matchNorth = True
						if(matchWest == True):
							#if the region of pixCheck is not == to region of pixNorth
							if(pix2region[str(pixNorth)] != pix2region[str(pixCheck)]):
								pix2region[str(pixCheck)] = self.minReg(pix2region[str(pixNorth)], pix2region[str(pixWest)])
								uf.union(str(pix2region[str(pixNorth)]), str(pix2region[str(pixWest)]))
						elif(matchWest == False):
							pix2region[str(pixCheck)] = pix2region[str(pixNorth)]
				
				if(matchWest == False and matchNorth == False):
					uf.insert_objects(str(regionCnt))
					pix2region[str(pixCheck)] = regionCnt
					regionCnt += 1
					
		'''	MASK   '''
		print "MASK"
		for y in range(img.height):
			mask.append([])
			for x in range(img.width):
				pixCheck = [y, x]
				mask[y].append(pix2region[str(pixCheck)])
			
				
		print "First Pass: ", mask
					
		'''	Pass Two   '''
		print "Pass Two"
		for y in range(img.height):
			for x in range(img.width):
		#~ for y in range(img.height):
			#~ for x in range(img.width):
				pixCheck = [y, x]
				#get the address of the pixel to compare to
				regInt = pix2region[str(pixCheck)]
				parReg = uf.find(str(regInt))
				pix2region[str(pixCheck)] = int(parReg)
				if(not region2pix.has_key(parReg)):
					region2pix[parReg] = []
				region2pix[parReg].append(pixCheck)
				
		mask = []
				
		'''	MASK   '''
		print "MASK"
		for y in range(img.height):
			mask.append([])
			for x in range(img.width):
				pixCheck = [y, x]
				mask[y].append(pix2region[str(pixCheck)])
				
		print "Second Pass: ", mask
						
		print "coloring regions"
		blobImg = cv.CreateImage((img.width, img.height), cv.IPL_DEPTH_8U, 3)
		for key in region2pix.keys():
			avgColor = self.getAvgColor(region2pix[key], img)
			region2color[key] = avgColor
			for i in range(len(region2pix[key])):
				blobImg[region2pix[key][i][0], region2pix[key][i][1]] = avgColor
					
		return blobImg, region2color, region2pix
				
				
	def getAvgColor(self, pixList, img):
		blue = 0
		green = 0
		red = 0
		
		for i in range(len(pixList)):
			rgb = img[pixList[i][0], pixList[i][1]]
			blue += rgb[0]
			green += rgb[1]
			red += rgb[2]
			
		blueAvg = blue / len(pixList)
		greenAvg = green / len(pixList)
		redAvg = red / len(pixList)
		
		return (int(blueAvg), int(greenAvg), int(redAvg))
					
	def getPixOffset(self, pixIn, pixComp, img):
		pixCompReal = []
		pixCompReal.append(pixIn[0] + pixComp[0])
		pixCompReal.append(pixIn[1] + pixComp[1])
			
		if(pixCompReal[0] < 0 or pixCompReal[0] >= img.height \
			or pixCompReal[1] < 0 or pixCompReal[1] >= img.width):
			pixCompReal[0] = pixIn[0]
			pixCompReal[1] = pixIn[1] 
		#print pixCompReal
		return pixCompReal

	def checkPixels(self, pixIn, pixComp, img, threshold):
		pixInVal = img[pixIn[0], pixIn[1]]
		pixCompVal = img[pixComp[0], pixComp[1]]
		
		blueDiff = abs(pixInVal[0] - pixCompVal[0])
		greenDiff = abs(pixInVal[1] - pixCompVal[1])
		redDiff = abs(pixInVal[2] - pixCompVal[2])
		
		if(blueDiff < threshold and redDiff < threshold and greenDiff < threshold):
			return True
		else:
			return False


	def minReg(self, reg1, reg2):
		if(reg1 <= reg2):
			return reg1
		else:
			return reg2
			
			
	def inList(self, key, list):
		for i in range(len(list)):
			if(key == list[i]):
				return True
		return False
		

def main():
	
	myBlob = blobit()
	
	img = myBlob.gen5x5()
	
	blobImg, region2color, region2pix = myBlob.findBlob(img, 15)
	
	return 0

if __name__ == '__main__':
	main()

