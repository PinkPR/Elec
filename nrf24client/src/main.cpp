#include <SPI.h> // Pour la gestion du port SPI
#include <Mirf.h> // Pour la gestion de la communication
#include <nRF24L01.h> // Pour les définitions des registres du nRF24L01
#include <MirfHardwareSpiDriver.h>
#include <Arduino.h>

#define PAYLOAD_SIZE		4
#define CHANNEL				0
#define CE_PIN				8
#define CS_PIN				7

#define RADDR				"NRF02"
#define TADDR				"NRF01"

#define SENSOR_CNT		1

#define SENSOR_0_ID					0
#define SENSOR_0_CALLBACK		&ledCallback

#define LED_PIN			3

struct sensorAction
{
	int id;
	void (*callback)(int sensorValue);
};

struct sensorAction sensorActions[SENSOR_CNT];

void ledCallback(int sensorValue)
{
	if (sensorValue)
		Serial.println("ON");
	else
		Serial.println("OFF");
}

void setupSensorActions()
{
	sensorActions[0].id					= SENSOR_0_ID;
	sensorActions[0].callback		= SENSOR_0_CALLBACK;

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
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

void setup()
{
	Serial.begin(9600);

	setupSensorActions();
	setupRF24();
}

void loop()
{
	byte payload[PAYLOAD_SIZE];
	int *payload_int = (int *) payload;

	for (int i = 0; i < SENSOR_CNT; i++)
	{
		payload[0] = i;

		send(payload);
		listen(payload);
		Serial.println("Value read :")
		Serial.println(payload_int[0]);

		sensorActions[i].callback(payload_int[0]);
		delay(1000);
	}
}
