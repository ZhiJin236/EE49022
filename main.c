void LCD_Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   
    /* FSMC */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
   
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
                                  | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG
                                  | RCC_APB2Periph_GPIOF , ENABLE);
   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
    /*LCD*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;               
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    /* FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
   
                /*
                 * PD4-FSMC_NOE   :LCD-RD
                 * PD5-FSMC_NWE   :LCD-WR
                 * PG12-FSMC_NE4  :LCD-CS
                 * PE2-FSMC_A23   :LCD-DC
                */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStructure);  
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

}

void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;
   
   
    p.FSMC_AddressSetupTime = 0x02;         
    p.FSMC_AddressHoldTime = 0x00;         
    p.FSMC_DataSetupTime = 0x05;                
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;
    p.FSMC_AccessMode = FSMC_AccessMode_B;        
   
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
//        FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;       
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;  
   
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
   
    /*  FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

void RA8875_WriteReg(uint8_t RegAddr, uint8_t RegValue)
{
  LCD_RA8875_REG (RegAddr);
  LCD_RA8875_WRITE_DATA(RegValue);
}

void RA8875_Config(void)
{
                LCD_RA8875_REG(0x88);
                LCD_Delay(10);               
                LCD_RA8875_WRITE_DATA(12);        /* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

          LCD_Delay(10000);

                LCD_RA8875_REG(0x89);
                LCD_Delay(10);                
                LCD_RA8875_WRITE_DATA(2);       
                /* 
                  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
                          = 25M * (12 + 1) / ((0 + 1) * (2 ^ 2))
                                  = 81.25MHz
                */
	
            LCD_Delay(10000);
   
    RA8875_WriteReg(0x10, (1 <<3 ) | (1 << 1));        
  
                /* REG[04h] Pixel Clock Setting Register (PCS) */
	
            RA8875_WriteReg(0x04, 0x81);
            LCD_Delay(10000);
      
                /* OTD9960 & OTA7001 */
                RA8875_WriteReg(0x14, 0x63);
                RA8875_WriteReg(0x15, 0x00);
                RA8875_WriteReg(0x16, 0x03);
                RA8875_WriteReg(0x17, 0x03);
                RA8875_WriteReg(0x18, 0x0B);
                RA8875_WriteReg(0x19, 0xDF);
                RA8875_WriteReg(0x1A, 0x01);
                RA8875_WriteReg(0x1B, 0x1F);
                RA8875_WriteReg(0x1C, 0x00);
                RA8875_WriteReg(0x1D, 0x16);
                RA8875_WriteReg(0x1E, 0x00);
                RA8875_WriteReg(0x1F, 0x01);
   
    RA8875_WriteReg(0xC7, 0x01);        /* DISP = 1 */

    /* LCD Display on */
    RA8875_WriteReg(0x01, 0x80);

    /*         REG[40h] Memory Write Control Register 0 (MWCR0)*/
    RA8875_WriteReg(0x40, 0x00);        
        //RA8875_WriteReg(0x20, 0x04);       

    /*         REG[41h] Memory Write Control Register1 (MWCR1)*/
    RA8875_WriteReg(0x41, 0x01);        
}

void LCD_Rst(void)
{                       
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);         
    LCD_Delay(0xAFFff);                                           
    GPIO_SetBits(GPIOB, GPIO_Pin_1);                          
    LCD_Delay(0xAFFff);        
}

void LCD_Init(void)
{
        LCD_GPIO_Config();
        LCD_FSMC_Config();
       
        LCD_Rst();
  
    RA8875_Config();  
}

static void I2C_Delay(void)
{
        uint8_t i;
        for (i = 0; i < 10; i++);
}

static void Delay(__IO uint32_t nCount)         
{
        for(; nCount != 0; nCount--);
}

void I2C_Start(void)
{
        I2C_SDA_1();
        I2C_SCL_1();
        I2C_Delay();
        I2C_SDA_0();
        I2C_Delay();
        I2C_SCL_0();
        I2C_Delay();
}

void I2C_Stop(void)
{
        I2C_SDA_0();
        I2C_SCL_1();
        I2C_Delay();
        I2C_SDA_1();
}

static uint8_t I2C_WaitAck(void)
{
        uint8_t re;

        I2C_SDA_1();       
        I2C_Delay();
        I2C_SCL_1();       
        I2C_Delay();
        if (I2C_SDA_READ())        
        {
                re = 1;
        }
        else
        {
                re = 0;
        }
        I2C_SCL_0();
        I2C_Delay();
        return re;
}

static void I2C_NAck(void)
{
        I2C_SDA_1();        /* CPU??SDA = 1 */
        I2C_Delay();
        I2C_SCL_1();        
        I2C_Delay();
        I2C_SCL_0();
        I2C_Delay();       
}

static void I2C_SendByte(uint8_t ucByte)
{
        uint8_t i;

        for (i = 0; i < 8; i++)
        {               
                if (ucByte & 0x80)
                {
                        I2C_SDA_1();
                }
                else
                {
                        I2C_SDA_0();
                }
                I2C_Delay();
                I2C_SCL_1();
                I2C_Delay();       
                I2C_SCL_0();
                if (i == 7)
                {
                         I2C_SDA_1(); 
                }
                ucByte <<= 1;        
                I2C_Delay();
        }
}