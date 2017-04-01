#include <EEPROM.h>
#include <Ethernet.h>
//#include <Dns.h>
//#include <Dhcp.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>
#include <Base64.h>

#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 5
#define RELAY_PIN 3
#define REQ_BUF_SZ 100
#define OPTIONS_FILE_TEMP "temp.ini"
//#define OPTIONS_AUTOMANSTATUS_FILE "optstat.ini"
#define BUFFER_SIZE 1024
//#define ITERATIONS_MAX 1800 //10 - для тестування, для експлуатації треба буде збільшити це значення до 30хв, приблизно має бути 1800
const bool IF_DEBUG_SERIAL = 1;
OneWire oneWire1(ONE_WIRE_BUS_1);
OneWire oneWire2(ONE_WIRE_BUS_2);
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DeviceAddress Thermometer1, Thermometer2, Thermometer3;
float Temp1, Temp2, Temp3;
float sumTemp1, sumTemp2, sumTemp3;
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress arduinoIP(192, 168, 1, 5);
IPAddress routerIP(192, 168, 1, 1);
EthernetServer server(80);
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0; // index into HTTP_req buffer
File webFile;
byte relayStatus;
int iteration, iterationCount, log_history_len;
byte AutoMan; //ознака режиму роботи (0 - ручний, 1 - автоматичний режими) + доробити збереження статусу у файл
float tempLimit, hysteresis;

/*28FFBA16A1150327
28FF0683921501AF
28FF3682921501A4*/
void setup()
{
	Serial.begin(9600);
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, HIGH);
	StartEthernetServer();	
	StartSDCard();	
	LoadOptions();
	StartThermometerSensor();	
	tempLimit = 27.5;//тимчасово, ліміт для автоматичного управління реле
	iteration = 0;
	iterationCount = 100;
}

void loop()
{
	GetTemperatures();//отримування даних з датчиків температури	
	CheckRelayStatus();	//визначення стану реле, можливо запис у файл для початкового стану після запуску плати
	WebServer(); //ВЕБ сторінка
	GetStatisticsTemp();//статистика
	CheckTemp();	//управління реле в авт режимі відповідно значення гістерезису (ПОКИ +/-0,5)	
}

void WebServer()
{
	EthernetClient client = server.available();
	if (client) {
		if (IF_DEBUG_SERIAL) Serial.println(" new client");
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;	//ознака завершення http заголовку від користувача	
		while (client.connected()) 
		{
			if (client.available()) 
			{
				char c = client.read();		 //зчитування http заголовку від користувача по 1 байту	
				if (req_index < (REQ_BUF_SZ - 1)) 
				{
					HTTP_req[req_index] = c; // save HTTP request character
					req_index++;
				}
				if (c == '\n' && currentLineIsBlank) 
				{
					if (IF_DEBUG_SERIAL) Serial.println(HTTP_req);
					if (StrContains(HTTP_req, "GET / ") || StrContains(HTTP_req, "GET /index.htm")) 
					{
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connnection: close");
						client.println();
						webFile = SD.open("index.htm");
					}
					else if (StrContains(HTTP_req, "GET /options.htm")) 
					{
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connnection: close");
						client.println();
						webFile = SD.open("options.htm");
					}
					else if (StrContains(HTTP_req, "GET /style.css")) 
					{
						if (IF_DEBUG_SERIAL) Serial.println(HTTP_req);
						webFile = SD.open("style.css");
						if (webFile) 
						{
							client.println("HTTP/1.1 200 OK");
							client.println("Content-Type: text/css");
							client.println("Connnection: close");
							client.println();
						}
					}
					else if (StrContains(HTTP_req, "GET /test.jpg")) 
					{
						webFile = SD.open("test.jpg");
						if (webFile) 
						{
							client.println("HTTP/1.1 200 OK");
							client.println();
						}
					}
					else if (StrContains(HTTP_req, "ajax_temp")) {
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connection: keep-alive");
						client.println();

						client.print(Temp1, 3);
						client.print(":");
						client.print(Temp2, 3);
						client.print(":");
						client.print(Temp3, 3);
						client.print(":");						
						client.print(millis());	
						client.print(":");
						client.print(relayStatus);
						client.print(":");
						client.print(relayStatus);
						client.print(":");
						client.print(AutoMan);
					}
					else if (StrContains(HTTP_req, "ajax_options")) {
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connection: close");
						client.println();

						client.print(tempLimit);
						client.print(":");
						client.print(hysteresis);
						client.print(":");
						client.print("pwd");
						client.print(":");						
						client.print("debug");	
						client.print(":");
						client.print(iterationCount);
						client.print(":");
						client.print(log_history_len);						
					}
					else if (StrContains(HTTP_req, "?status=0")) //OFF relay
					{						
						if (AutoMan == 0)
						{
							digitalWrite(RELAY_PIN, HIGH);//!digitalRead(RELAY_PIN)								
							client.println("HTTP/1.1 200 OK");
							client.println("Content-Type: text/html");
							client.println("Connnection: close");
							client.println();
						}
					}
					else if (StrContains(HTTP_req, "?status=1")) //ON
					{		
						if (AutoMan == 0)
						{
							digitalWrite(RELAY_PIN, LOW);//!digitalRead(RELAY_PIN)								
							client.println("HTTP/1.1 200 OK");
							client.println("Content-Type: text/html");
							client.println("Connnection: close");
							client.println();
						}
					}
					else if (StrContains(HTTP_req, "relay?automan")) 
					{												
						AutoMan = !AutoMan;
						SaveOptions(0);						
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connnection: close");
						client.println();
					}
					else if (StrContains(HTTP_req, "options?")) 
					{														
						client.println("HTTP/1.1 200 OK");
						client.println("Content-Type: text/html");
						client.println("Connnection: close");
						client.println();
					}
					if (webFile) 
					{
						byte buff[BUFFER_SIZE];
						while (true)
						{
                            int len = webFile.read((char*)buff, BUFFER_SIZE);
                            if (!len)
                                break;
                            client.write(buff, len);
                        }
						webFile.close();						
					}
					req_index = 0;
					StrClear(HTTP_req, REQ_BUF_SZ);
					break;
				}

				if (c == '\n') {					
					currentLineIsBlank = true; //http заголовок зчитаний повністю, можна видавати користувачу
				}
				else if (c != '\r') {					
					currentLineIsBlank = false; //http заголовок ще не зчитаний повністю
				}
			}
		}		
		delay(1);		
		client.stop();
		//if (IF_DEBUG_SERIAL) Serial.println("client disconnected");
	}
}

void CheckTemp()
{
	if (AutoMan == 1)
	{
		if (Temp3 >= tempLimit + hysteresis) digitalWrite(RELAY_PIN, LOW);
		if (Temp3 <= tempLimit - hysteresis) digitalWrite(RELAY_PIN, HIGH);
	}
}

void GetTemperatures()
{
	sensors1.requestTemperatures(); //запит температури для всіх датчиків
	sensors2.requestTemperatures();
	
	Temp1 = sensors1.getTempC(Thermometer1);
	Temp2 = sensors1.getTempC(Thermometer2);
	Temp3 = sensors2.getTempC(Thermometer3);
}

void CheckRelayStatus()
{
	if (digitalRead(RELAY_PIN) == HIGH) relayStatus = 0;//OFF
	else if (digitalRead(RELAY_PIN) == LOW) relayStatus = 1;//ON	
}

void StartEthernetServer()
{
	Ethernet.begin(mac, arduinoIP, routerIP, routerIP);
	server.begin();// хз чи треба !!!
	if (IF_DEBUG_SERIAL) 
	{
		Serial.print("server is at ");
		Serial.println(Ethernet.localIP());	
	}
}

void StartThermometerSensor()
{
	sensors1.begin();
	sensors2.begin();
	/*if (IF_DEBUG_SERIAL) 
	{
		Serial.print("Locating devices...");
		Serial.print("Found "); 
		Serial.print(sensors1.getDeviceCount(), DEC);
		Serial.print(" + ");
		Serial.print(sensors2.getDeviceCount(), DEC);
		Serial.println(" devices.");
	}*/
	if (!sensors1.getAddress(Thermometer1, 0)) if (IF_DEBUG_SERIAL) Serial.println("Unable to find address for Device 0"); 
	if (!sensors1.getAddress(Thermometer2, 1)) if (IF_DEBUG_SERIAL) Serial.println("Unable to find address for Device 1");     
	if (!sensors2.getAddress(Thermometer3, 0)) if (IF_DEBUG_SERIAL) Serial.println("Unable to find address for Device 2"); 

	/*printAddress(Thermometer1);
	printAddress(Thermometer2);
	printAddress(Thermometer3);	*/

	sensors1.setResolution(Thermometer1, 12);
	sensors1.setResolution(Thermometer2, 12);
	sensors2.setResolution(Thermometer3, 12);
	/*if (IF_DEBUG_SERIAL) 
	{
		Serial.print("Device 0 Resolution: ");
		Serial.print(sensors1.getResolution(Thermometer1), DEC); 
		Serial.print(sensors1.getResolution(Thermometer2), DEC); 
		Serial.print(sensors2.getResolution(Thermometer3), DEC); 
		Serial.println();
	}*/
}

void StartSDCard()
{
	SD.begin(4);
}

