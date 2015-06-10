all: build

build:
	ino build -m uno

upload:
	sudo ino upload -p /dev/ttyACM2

serial:
	sudo picocom -b 9600 /dev/ttyACM2

clean:
	ino clean
