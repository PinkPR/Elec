#include <SPI.h> // Pour la gestion du port SPI
#include <Mirf.h> // Pour la gestion de la communication
#include <nRF24L01.h> // Pour les définitions des registres du nRF24L01
#include <MirfHardwareSpiDriver.h>
#include <Arduino.h>

#define PAYLOAD_SIZE		4
#define CHANNEL				0
#define CE_PIN				8
#define CS_PIN				7

#define RADDR				"NRF01"
#define TADDR				"NRF02"

#define SENSOR_CNT			1

#define SENSOR_0_ID			0
#define SENSOR_0_PIN		3

struct sensor
{
	int id;
	int pin;
};

struct sensor sensors[SENSOR_CNT];

void setupSensors()
{
	sensors[0].id	= SENSOR_0_ID;
	sensors[0].pin	= SENSOR_0_PIN;

	for (int i = 0; i < SENSOR_CNT; i++)
		pinMode(sensors[i].pin, INPUT);
}

void setupRF24()
{
	Mirf.cePin		= CE_PIN;
	Mirf.csnPin		= CS_PIN;
	Mirf.spi		= &MirfHardwareSpi;
	Mirf.init();
	Mirf.channel	= CHANNEL;
	Mirf.payload	= PAYLOAD_SIZE;
	Mirf.config();
	Mirf.setTADDR((uint8_t *) TADDR);
	Mirf.setRADDR((uint8_t *) RADDR);
}

void listen(byte *payload)
{
	while (!Mirf.dataReady());

	Mirf.getData(payload);
}

void send(byte *payload)
{
	Mirf.send(payload);

	while (Mirf.isSending());
}

int readSensor(int sensorId)
{
	for (int i = 0; i < SENSOR_CNT; i++)
	{
		if (sensors[i].id == sensorId)
			return digitalRead(sensors[i].pin);
	}

	return 123;
}

void setup()
{
	Serial.begin(9600);
	setupSensors();
	setupRF24();
}

void loop()
{
	byte payload[PAYLOAD_SIZE];
	int *payload_int = (int *) payload;

	listen(payload);

	if (payload[0] >= SENSOR_CNT)
	{
		payload[0] = (byte) 'E';
		send(payload);
	}
	else
	{
		payload_int[0] = readSensor(payload[0]);
		send(payload);
	}
}
