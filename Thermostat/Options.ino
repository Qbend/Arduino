//������ 0 ����� ������ ���������� (����/���)
//������ 1 ��� ��� ���������� (��������� ���� 20����)
//������ 2 ��������� ��� ���������� (������� 0,5)
//������ 3 ������� �������� ��� ������ ���������� ����������
//������ 4 ������� ���������� ����������, ����� � �������
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
		log_history_len = 200;//����� � �������
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


