#include <project.h>
#define HDC1080ADRR 0x40

ClosedCube_HDC1080 hdc1080;
float HDC_Temp;
int HDC_Humi;

void HDC1080_Init()
{
    hdc1080.begin(HDC1080ADRR);

	Serial.print("Manufacturer ID=0x");
	Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	Serial.print("Device ID=0x");
	Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
}

void HDC_GetData()
{
	HDC_Temp = hdc1080.readTemperature();
	HDC_Humi = hdc1080.readHumidity();
	SystemTemp = (int)(HDC_Temp + 0.5);
	Serial.println("Temp: " + String(HDC_Temp)+ "*C");
	Serial.println("Humidity: " + String(HDC_Humi)+ "%");
	Serial.println("System Temp: " + String(SystemTemp)+ "*C");
}
