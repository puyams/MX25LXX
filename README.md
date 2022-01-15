# MX25LXX
SPI Flash Driver STM32HAL
Code For TEST


// SPI Flash Test -------------------------------------------------
        MX25L_SetStatus(0x00);
	uint8_t status = MX25L_GetStatus();
	printf("\r\nGet status:%d\r\n",status);
	printf("-----------------\r\n");
	uint8_t buffer[257];
	MX25L_Get_Identification((uint8_t *)buffer);
	printf("Identification read:\r\n");
	printf("Byte 0: %x\r\n", buffer[0]);
	printf("Byte 1: %x\r\n", buffer[1]);
	printf("Byte 2: %x\r\n", buffer[2]);
	printf("-----------------\r\n");
  
	printf("Clearing chip ... ");
	MX25L_ChipErase();
	printf("done\r\n");
	printf("-----------------\r\n");
	buffer[256] = 0;
	uint8_t ch = 0x21;
	uint8_t page = 0;
	while (1)
	{	
		memset(buffer,ch, 256);	
		printf("Writing page %d ... ", page);
		MX25L_Write(page * 256, buffer, 256);
		printf("done\r\n");
		printf("Data: %s\r\n", buffer);
		printf("Reading page %d ... ", page);
		MX25L_Read(page * 256, buffer, 256);
		buffer[256] = 0;
		printf("done\r\n");
		printf("Data: %s\r\n", buffer);
		printf("\r\n\r\n");
		ch ++;
		if(ch > 0x7E) ch = 0x21;
		page ++;
		if(page > 4095) page = 0;
		HAL_Delay(1000);
	}
