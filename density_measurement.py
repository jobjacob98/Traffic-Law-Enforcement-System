#
#
# Project Name: 	Traffic Law Enforcement System
# Author List: 		Ankitha Girish, Aiswarya Arun, Aishwarya LV, Job Jacob
# Filename: 		density_measurement.py
# Functions: 		(), void loop(), int data(int, int)
# Global Variables:	1) videos of type list
#                       2) density_objs of type list
#                   	3) densities of type list
#                   	4) vis_vert1 of type list
#                   	5) vis_vert2 of type list
#                   	6) vis of type list
#
#

import numpy as np
import cv2 as cv
import time
import serial
import threading

# A class for the Density Measurement. This class can be run parallelly with the help of threading module.
# This helps us to compare the densities of all the roads parallelly.
class DensityMeasurement(threading.Thread):
	#
	#
	# Function Name: 	__init__()
	# Input: 		1) vo --> to capture the video
	#			2) ft --> the flow theshold to compare with the obtained density
	#			3) name --> name of the video
	# Output: 		None
	# Logic:		This function is used to initialize the variables inside the class DensityMeasurement()
	#			(ie. it is a constructor of the class DensityMeasurement())
	# Example Call:		DensityMeasurement.__init__(cv.VideoCapture(video), 1.0, video)
	#
	#
	def __init__(self, vo, ft, name):
		threading.Thread.__init__(self)
		self.ft = ft
		self.vo = vo
		self.name = name
		self.flow_image = None
		self.density = None

	#
	#
	# Function Name: 	draw_flow()
	# Input: 		1) img --> the BGR to gray converted image
	# 			2) flow --> the list
	# Output: 		vis
	# Logic:		This function draws the flow and displays it on the monitor along with the captured image.
	#			The flow of pixels and its angle of flow are drawn using this function.
	# Example Call:		self.flow_image = self.draw_flow(gray, flow) from run function
	#
	#
	def draw_flow(self, img, flow, step=16):
		h, w = img.shape[:2]
		y, x = np.mgrid[step/2:h:step, step/2:w:step].reshape(2, -1).astype(int)
		fx, fy = flow[y, x].T
		lines = np.vstack([x, y, x+fx, y+fy]).T.reshape(-1, 2, 2)
		lines = np.int32(lines + 0.5)
		vis = cv.cvtColor(img, cv.COLOR_GRAY2BGR)
		cv.polylines(vis, lines, 0, (170, 232, 238))
		for (x1, y1), (_x2, _y2) in lines:
			cv.circle(vis, (x1, y1), 1, (170, 232, 238), -1)
		return vis

	#
	#
	# Function Name: 	run()
	# Input: 		self --> passing the class itself as a parameter
	# Output: 		None
	# Logic:		This function uses Farneback Optical Flow to find the density of each road meeting
	#			at the junction
	# Example Call:		DensityMeasurement.start()
	#
	#
	def run(self):
		cam = self.vo # creating VideoCapture object
		ret, prev = cam.read() # initial frame capture
		prevgray = cv.cvtColor(prev, cv.COLOR_BGR2GRAY) # converting previous captured image from BGR to gray
		while True:
			ret, img = cam.read() # capture frame-by-frame
			gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY) # our operations on the frame come here
			# Finding the flow using Farneback method. Gets a list of the flow as return value.
			flow = cv.calcOpticalFlowFarneback(prevgray, gray, 0.5, 3, 15, 3, 5, 1.2, 0)
			mag, ang = cv.cartToPolar(flow[..., 0], flow[..., 1]) # finding the magnitude and angle of the flow
			self.density = mag[np.where(mag > self.ft)].shape[0] # comparing the magnitude with the flow threshold
			prevgray = gray
			self.flow_image = self.draw_flow(gray, flow)

# This class acts as the controller which sends serial data to the Arduino board. This class also runs parallelly
# to help us change the traffic signals in real time.
class Controller(threading.Thread):
	#
	#
	# Function Name: 	__init__()
	# Input: 		densities of type list
	# Output: 		None
	# Logic:		This function is used to initialize the variables inside the class Controller()
	#			(ie. it is a constructor of the class Controller())
	# Example Call:		Controller.__init__(densities)
	#
	#
	def __init__(self, densities):
		threading.Thread.__init__(self)
		self.ser = serial.Serial('/dev/ttyS0', 9600) # initializing serial communication to /dev/ttyS0 port
		# initializing densities
		self.densig_1 = densities[0]
		self.densig_2 = densities[1]
		self.densig_3 = densities[2]
		self.densig_4 = densities[3]

	#
	#
	# Function Name: 	run()
	# Input: 		self --> passing the class itself as a parameter
	# Output: 		None
	# Logic:		This function compares all the densities obtained using optical flow and sends
	#			serial data to the Arduino based on the comparision
	# Example Call:		Controller.start()
	#
	#
	def run(self):
		while True:
			# comparing the densities and sending serial data to the arduino
			if self.densig_1 > (self.densig_2 and self.densig_3 and self.densig_4):
				self.ser.write('1')
			elif self.densig_2 > (self.densig_1 and self.densig_3 and self.densig_4):
				self.ser.write('2')
			elif self.densig_3 > (self.densig_1 and self.densig_2 and self.densig_4):
				self.ser.write('3')
			elif self.densig_4 >= (self.densig_1 and self.densig_2 and self.densig_3):
				self.ser.write('4')
    		ser.close()

if __name__ == "__main__":
	# creating a list of the videos
	videos = ["video1.avi","video2.mp4","video3.mp4","video4.mp4"]
	density_objs = []
	# creating an object for the class DensityMeasurement, passing the videos to the class DensityMeasurement
	# and starting the run function inside the class DensityMeasurement
	for video in videos:
		density_obj = DensityMeasurement(cv.VideoCapture(video), 1.0, video)
		density_obj.start()
		density_objs.append(density_obj) # appending each parallel density object to the list
	while True:
		time.sleep(1)
		# creating a list of the densities, creating an object for the class Controller, passing the
		# densities to the class Controller and starting the run function inside the class Controller
		densities = [density_objs[0].density, density_objs[1].density, density_objs[2].density, density_objs[3].density]
		controller_obj=Controller(densities)
		controller_obj.start()
		# concatenating all the flow images together for displaying
		vis_vert1 = np.concatenate((density_objs[0].flow_image, density_objs[1].flow_image), axis=0)
		vis_vert2 = np.concatenate((density_objs[2].flow_image, density_objs[3].flow_image), axis=0)
		vis = np.concatenate((vis_vert1, vis_vert2), axis=1)
		# showing the images to display
		cv.imshow('vis_vert', vis)
		ch_1 = cv.waitKey(5)
		if ch_1 == 27:
			break
	cv.destroyAllWindows()
