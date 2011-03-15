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
	
	'''	Pass One	'''
	print "Pass One"
	for y in range(0, img.height, 2):
		for x in range(0, img.width, 2):
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
				
	'''	Pass Two   '''
	print "Pass Two"
	for y in range(0, img.height, 2):
		for x in range(0, img.width, 2):
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
					
	print "coloring regions"
	blobImg = cv.CreateImage((self.width, self.height), cv.IPL_DEPTH_8U, 3)
	for key in region2pix.keys():
		avgColor = self.getAvgColor(region2pix[key], img)
		region2color[key] = avgColor
		for i in range(len(region2pix[key])):
			blobImg[region2pix[key][i][0], region2pix[key][i][1]] = avgColor
				
	return blobImg, region2color, region2pix
