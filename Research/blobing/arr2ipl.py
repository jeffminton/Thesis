img5x5 = [[[120, 111, 82],[118, 109, 80],[117, 108, 79],[50, 70, 79],[51, 71, 81]],
[[121, 112, 83],[120, 111, 82],[51, 70, 80],[50, 72, 81],[49, 69, 82]],
[[121, 112, 83],[50, 111, 201],[50, 109, 200],[51, 115, 204],[67, 200, 100]],
[[200, 50, 90],[225, 104, 67],[50, 115, 202],[73, 112, 201],[72, 100, 203]],
[[201, 70, 100],[100, 234, 80],[103, 231, 79],[99, 229, 78],[98, 228, 77]]]

import cv

def gen5x5():
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
