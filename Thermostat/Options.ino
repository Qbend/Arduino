//Адреса 0 режим роботи термостата (авто/руч)
//Адреса 1 ліміт для термостата (початкова темп 20град)
//Адреса 2 гістерезис для термостата (базовий 0,5)
//Адреса 3 кількість ітерацій для періоду інтеграції статистики
//Адреса 4 глибина збереження статистики, розмір в кбайтах
void SaveOptions(byte adr)
{
	switch (adr)
	{
	case 0:
		EEPROM.write(0, AutoMan);
		break;
	case 1:
		EEPROM.write(1, tempLimit);
		break;
	case 2:
		EEPROM.write(2, hysteresis);
		break;
	case 3:
		EEPROM.write(2, iterationCount);
		break;
	case 4:
		EEPROM.write(2, log_history_len);
		break;
	default:
		break;
	}
}

// Load options
void LoadOptions() 
{
	AutoMan = EEPROM.read(0);
	tempLimit = EEPROM.read(1);
	hysteresis = EEPROM.read(2);
	iterationCount = EEPROM.read(3);
	log_history_len = EEPROM.read(4);
	if (AutoMan == 255) 
	{
		AutoMan = 0;
		EEPROM.write(0, AutoMan);
	}
	if (tempLimit == 255) 
	{
		tempLimit = 20;
		EEPROM.write(1, tempLimit);
	}
	if (hysteresis == 255) 
	{
		hysteresis = 0.5;
		EEPROM.write(2, hysteresis);
	}
	if (iterationCount == 255) 
	{
		iterationCount = 1800;
		EEPROM.write(2, iterationCount);
	}
	if (log_history_len == 255) 
	{
		log_history_len = 200;//розмір в кбайтах
		EEPROM.write(2, log_history_len);
	}
	if (IF_DEBUG_SERIAL)
	{
		Serial.print("AutoMan ");
		Serial.println(AutoMan);
		Serial.print("tempLimit ");
		Serial.println(tempLimit);	
		Serial.print("hysteresis ");
		Serial.println(hysteresis);	
	}
}


