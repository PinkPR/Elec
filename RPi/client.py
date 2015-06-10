# CLIENT APP

from RF24 import *
import RPi.GPIO as GPIO

PAYLOAD_SIZE = 4
CHANNEL = 0
CE_PIN = 8
CS_PIN = 7

SENSOR_0_ID = 0

sensorActions = []

radio = RF24(RPI_BPLUS_GPIO_J8_22, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)
pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

radio.begin()
radio.enableDynamicPayloads()
radio.setRetries(5,15)

radio.openWritingPipe(pipes[1])
radio.openReadingPipe(1, pipes[0])

class SensorAction:
	def __init__(self, ID, callback):
		self.ID = ID
		self.callback = callback

def ledCallback(sensorValue):
	if (sensorValue == 0):
		print "OFF"
	else:
		print "ON"

sensorActions.append(SensorAction(0, ledCallback))

def listen():
	radio.startListening()

	while (not radio.available()):
		continue

	return radio.read(PAYLOAD_SIZE)

def send(payload):
	radio.stopListening()
	radio.write(payload)

while True:
	for sensorAction in sensorActions:
		send(sensorAction.ID)
		ret = listen()
		sensorAction.callback(ret)
