
#include <SPI.h>
#include <SD.h>
File file;
char incomingByte = 0;
String temp;
void setup()
{	
	Serial.begin(9600);
	pinMode(13, OUTPUT);
	SD.begin(4);
	boolean rez;
	temp = "";
	rez = SD.exists("index.htm");
	ReadFile();
}

void loop()
{
	//WriteFile();
}

void WriteFile()
{
	if (Serial.available() > 0) 
	{
		SD.remove("index.htm");
		file = SD.open("index.htm", FILE_WRITE);
		digitalWrite(13, HIGH);	
	}
	while (Serial.available() > 0)
	{  									
		temp = Serial.readString();
		file.print(temp);
		delay(1);	//ץח הכÿ קמדמ		
	}
	digitalWrite(13, LOW);
	file.flush();
	file.close();
}

void ReadFile()
{
	file = SD.open("index.htm", FILE_READ);
	while (file.available()) 
	{
		Serial.write(file.read());
	}
	file.close();
}