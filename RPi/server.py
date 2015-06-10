# SERVER APP

from RF24 import *
import RPi.GPIO as GPIO

PAYLOAD_SIZE = 4
CHANNEL = 0
CE_PIN = 8
CS_PIN = 7

SENSOR_0_ID = 0
SENSOR_0_PIN = 25

sensors = []

radio = RF24(RPI_BPLUS_GPIO_J8_22, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ)
pipes = [0xF0F0F0F0E1, 0xF0F0F0F0D2]

radio.begin()
radio.enableDynamicPayloads()
radio.setRetries(5,15)

radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1, pipes[1])

class Sensor:
	def __init__(self, ID, pin):
		self.ID = ID
		self.pin = pin

	def setup(self):
		GPIO.setmode(self.pin, GPIO.IN)

	def readValue(self):
		return GPIO.input(self.pin)

sensors.append(Sensor(SENSOR_0_ID, SENSOR_0_PIN))

def listen():
	radio.startListening()

	while (not radio.available()):
		continue

	return radio.read(PAYLOAD_SIZE)

def send(payload):
	radio.stopListening()
	radio.write(payload)

for sensor in sensors:
	sensor.setup()

while True:
	ret = radio.listen()

	for sensor in sensors:
		if sensor.ID == int(ret):
			send(sensor.readValue())
