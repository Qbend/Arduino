char StrContains(char *str, char *sfind)
{
	char found = 0;
	char index = 0;
	char len;
	len = strlen(str);
	if (strlen(sfind) > len) {
		return 0;
	}
	while (index < len) {
		if (str[index] == sfind[found]) {
			found++;
			if (strlen(sfind) == found) {
				return 1;
			}
		}
		else {
			found = 0;
		}
		index++;
	}
	return 0;
}
void StrClear(char *str, char length)
{
	for (int i = 0; i < length; i++) {
		str[i] = 0;
	}
}
void printAddress(DeviceAddress deviceAddress)
{
	if (IF_DEBUG_SERIAL)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			if (deviceAddress[i] < 16) Serial.print("0");
			Serial.print(deviceAddress[i], HEX);
		}
		Serial.println();
	}
}

