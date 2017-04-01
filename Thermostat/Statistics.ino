void printMidTemperature()
{
	if (IF_DEBUG_SERIAL) 
	{
		if (iteration = iterationCount)
		{
			Serial.println(sumTemp1, 5);
			Serial.println(sumTemp2, 5);
			Serial.println(sumTemp3, 5);
		}
	}
}
void GetStatisticsTemp()
{
		if (iteration >= iterationCount)
	{
		//printMidTemperature();
		sumTemp1 /= iteration;
		sumTemp2 /= iteration;
		sumTemp3 /= iteration;
		//printMidTemperature();
		writeMidTemperature();
		iteration = 0;
		sumTemp1 = 0;
		sumTemp2 = 0;
		sumTemp3 = 0;
	}
	
	sumTemp1 += Temp1;
	sumTemp2 += Temp2;
	sumTemp3 += Temp3;
	iteration ++;
}
void writeMidTemperature()
{
	// Считываем данные (напряжение) с нулевого аналогового разъёма.
    //int temp = analogRead(tempPin);
    // Увеличиваем значение времени (номер строки) на единицу. Если вы будете записывать данные раз в минуту, но хотите чтобы время записывалось в секундах - прибавляйте не единицу, а 60.
	unsigned int time = millis();
	File MidTemp;
	MidTemp = SD.open("MidTemp.csv", FILE_WRITE);
	/*if (SD.exists("MidTemp.csv")) 
	{
		Serial.println("MidTemp.csv exists.");
	}
	else 
	{
		Serial.println("MidTemp.csv doesn't exist.");
	}*/
	if (MidTemp) 
	{
		//String str = time + ";" + String(sumTemp1) + ";" + String(sumTemp2) + ";" + String (sumTemp3);
		MidTemp.print(time);
		MidTemp.print(";");
		MidTemp.print(sumTemp1);
		MidTemp.print(";");
		MidTemp.print(sumTemp2);
		MidTemp.print(";");
		MidTemp.println(sumTemp3);
		MidTemp.close();
	} 
	else 
	{
		Serial.println("error opening data.csv");
	}
}
