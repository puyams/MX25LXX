
#include "main.h"
#include "MX25L.h"
//------------------------------------------------------------------------
extern SPI_HandleTypeDef    _MX25LXX_SPI;

//------------------------------------------------------------------------

#define MX25L_SLAVESELECT   HAL_GPIO_WritePin(_MX25LXX_CS_GPIO, _MX25LXX_CS_PIN, GPIO_PIN_RESET)
#define MX25L_SLAVEDESELECT HAL_GPIO_WritePin(_MX25LXX_CS_GPIO, _MX25LXX_CS_PIN, GPIO_PIN_SET)

#define COMMAND_WREN        0x06
#define COMMAND_WRDI        0x04
#define COMMAND_RDSR        0x05
#define COMMAND_WRSR        0x01
#define COMMAND_READ        0x03
#define COMMAND_FREAD       0x0B
#define COMMAND_WRITE       0x02
#define COMMAND_RDID        0x9F
#define COMMAND_SE          0x20
#define COMMAND_BE          0x52
#define COMMAND_CE          0x60


      
#define STATUS_WIP          0x01 
#define STATUS_WEL          0x02 
#define STATUS_BP0          0x04 
#define STATUS_BP1          0x08 
#define STATUS_BP2          0x10 
#define STATUS_BP3          0x20 
#define STATUS_RES          0x40 
#define STATUS_SWRD         0x80 
      
#define DUMMY               0xFF
//------------------------------------------------------------------------
void MX25L_SPI_Init(void)
{
	// Initial by HAL
  
}
//------------------------------------------------------------------------
uint8_t MX25L_SPI_Transfer(uint8_t data)
{
	uint8_t ret;
	HAL_SPI_TransmitReceive(&_MX25LXX_SPI, &data, &ret, 1, 100);
	return ret;
}
//------------------------------------------------------------------------
void MX25L_Get_Identification(uint8_t data[3])
{
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_RDID);
	data[0] = MX25L_SPI_Transfer(DUMMY);
	data[1] = MX25L_SPI_Transfer(DUMMY);
	data[2] = MX25L_SPI_Transfer(DUMMY);
	MX25L_SLAVEDESELECT;
}
//------------------------------------------------------------------------
uint8_t MX25L_GetStatus(void)
{
	uint8_t status;
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_RDSR);
	status = MX25L_SPI_Transfer(DUMMY);
	MX25L_SLAVEDESELECT;
	return status;
}
//------------------------------------------------------------------------
void MX25L_SetWriteEnable(bool enable)
{
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(enable ? COMMAND_WREN : COMMAND_WRDI);
	MX25L_SLAVEDESELECT;
}
//------------------------------------------------------------------------
void MX25L_SetStatus(uint8_t status)
{
	MX25L_SetWriteEnable(true);
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_WRSR);
	MX25L_SPI_Transfer(status);
	MX25L_SLAVEDESELECT;
}
//------------------------------------------------------------------------
void MX25L_SectorErase(uint32_t address)
{
	MX25L_SetWriteEnable(true);
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_SE);
	MX25L_SetAddress(address);
	MX25L_SLAVEDESELECT;
	while(MX25L_GetStatus() & STATUS_WIP);
}
//------------------------------------------------------------------------
void MX25L_BlockErase(uint32_t address)
{
	MX25L_SetWriteEnable(true);
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_BE);
	MX25L_SetAddress(address);
	MX25L_SLAVEDESELECT;
	while(MX25L_GetStatus() & STATUS_WIP);
}
//------------------------------------------------------------------------
void MX25L_ChipErase(void)
{
	MX25L_SetWriteEnable(true);
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_CE);
	MX25L_SLAVEDESELECT;
	while(MX25L_GetStatus() & STATUS_WIP);
}
//------------------------------------------------------------------------
void MX25L_SetAddress(uint32_t address)
{
	MX25L_SPI_Transfer( (uint8_t) (address >> 16) );
	MX25L_SPI_Transfer( (uint8_t) (address >> 8) );
	MX25L_SPI_Transfer( (uint8_t) address );
}
//------------------------------------------------------------------------
void MX25L_Write(uint32_t address, const uint8_t * data, uint16_t length)
{
	MX25L_SetWriteEnable(true);
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_WRITE);
	MX25L_SetAddress(address);
	for(uint16_t i = 0;i < length;i ++)
	{
		MX25L_SPI_Transfer(*(data + i));
	}
	MX25L_SLAVEDESELECT;
	while(MX25L_GetStatus() & STATUS_WIP);
}
//------------------------------------------------------------------------
void MX25L_Read(uint32_t address, uint8_t * data, uint16_t length)
{
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_READ);
	MX25L_SetAddress(address);
	for(uint16_t i = 0;i < length;i ++)
	{
		*(data + i) = MX25L_SPI_Transfer(DUMMY);
	}
	MX25L_SLAVEDESELECT;
	while(MX25L_GetStatus() & STATUS_WIP);
}
//------------------------------------------------------------------------
void MX25L_FastRead(uint32_t address, uint8_t * data, uint16_t length)
{
	MX25L_SLAVESELECT;
	MX25L_SPI_Transfer(COMMAND_READ);
	MX25L_SetAddress(address);
	MX25L_SPI_Transfer(DUMMY);
	for(uint16_t i = 0;i < length;i ++)
	{
		*(data + i) = MX25L_SPI_Transfer(DUMMY);
	}
	MX25L_SLAVEDESELECT;
	while(MX25L_GetStatus() & STATUS_WIP);
}
//------------------------------------------------------------------------
void ProtectBlock(bool bp0, bool bp1, bool bp2, bool bp3)
{
	uint8_t status;
  status = MX25L_GetStatus();
	if(bp0)
    status |= STATUS_BP0;
	if(bp1)
    status |= STATUS_BP1;
	if(bp2)
    status |= STATUS_BP2;
	if(bp3)
    status |= STATUS_BP3;

	MX25L_SetStatus(status);

}

//------------------------------------------------------------------------
