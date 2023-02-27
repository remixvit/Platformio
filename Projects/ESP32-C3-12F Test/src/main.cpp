#include <Arduino.h>
#include <Wire.h>
#include "ClosedCube_HDC1080.h"
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

ClosedCube_HDC1080 hdc1080;

void setup() 
{
  Serial.begin(115200);
  Serial.println(" ");
  Serial.println("Hello");

  hdc1080.begin(0x40);

  Serial.print("Manufacturer ID=0x");
	Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	Serial.print("Device ID=0x");
	Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device

  hdc1080.setResolution(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_14BIT);

  Serial.print("T=");
	Serial.print(hdc1080.readTemperature());
	Serial.print("C, RH=");
	Serial.print(hdc1080.readHumidity());
	Serial.println("%");
}

void loop() {
  // put your main code here, to run repeatedly:
}