Electronic Project GISTRE 2016
==============================

Subject : make 2 Raspberry Pi communicating through NRF24 and design a protocole
to remotely get sensor values.

As we faced some problems with our Raspberries, we decided to extend the project
to Arduino boards, in order to be able to show our work during a defense.

Requirements
===========
For Raspberry : RF24 library

For Arduino : Mirf Library

Client App
==========

The client app is asking for sensor values, and then do some actions depending on
these values.

The app was designed to associate a Sensor ID with a callback action.
The client ask the server for a value by sending the Sensor ID of the concerned
sensor.
The callback function is then called with the sensor value as an argument, and
it decides what is to be done.

Server App
==========

The server app is waiting for client requests, then get the concerned sensor's
value, and finally send it back to the client.
Sensor IDs must logically be the same for the client and the server, or an
Error value will be sent to the client.
