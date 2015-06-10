all: build

build:
	ino build -m uno

upload:
	sudo ino upload -p /dev/ttyACM0

serial:
	sudo picocom -b 9600 /dev/ttyACM0

clean:
	ino clean
