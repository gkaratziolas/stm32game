/* Includes **********************************/
#include "stm32f4xx.h"

#define LCD_RST_PIN    (GPIO_Pin_3)
#define LCD_RST_PORT   (GPIOD)

#define LCD_PWM_PIN    (GPIO_Pin_13)
#define LCD_PWM_PORT   (GPIOD)

#define  LCD_BASE_DATA ((u32)(0x60000000|0x00100000))
#define  LCD_BASE_ADDR ((u32)(0x60000000|0x00000000))
#define  LCD_CMD       (*(vu16 *)LCD_BASE_ADDR)
#define  LCD_DATA      (*(vu16 *)LCD_BASE_DATA)

#define ABS(X)  ((X) > 0 ? (X) : -(X))

/* Local Functions ***************************/
void lcd_write_reg(uint8_t reg, uint16_t value);
uint16_t lcd_read_reg(uint8_t reg);
void lcd_write_ram_prepare(void);
void lcd_write_ram(uint16_t rgb_code);
void lcd_gpio_config(void);
void lcd_fsmc_config(void);

/* Functions *********************************/
void lcd_write_reg(uint8_t reg, uint16_t value) {
        // Write 16-bit Index, then Write Reg
        LCD_CMD  = reg;
        LCD_DATA = value;
}

uint16_t lcd_read_reg(uint8_t reg) {
        // Write 16-bit Index, then Read Reg
        LCD_CMD = reg;
        return (LCD_Data);
}

void lcd_write_ram_prepare(void) {
        LCD_CMD = SSD2119_RAM_DATA_REG;
}

void lcd_write_ram(uint16_t rgb_code) {
        // Write 16-bit GRAM Reg
        LCD_Data = rgbcode;
}

void lcd_gpio_config(void) {
        GPIO_InitTypeDef GPIO_InitStructure;

        /* Enable GPIOB, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB |
                               RCC_AHB1Periph_GPIOD |
                               RCC_AHB1Periph_GPIOE |
                               RCC_AHB1Periph_GPIOF, ENABLE);

        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOD, &GPIO_InitStructure);

        GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
                                      GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

        GPIO_Init(GPIOE, &GPIO_InitStructure);

        GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

        /* SRAM Address lines configuration LCD-DC */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_Init(GPIOE, &GPIO_InitStructure);  
        GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);          

        /* NE3 configuration */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);

        /* LCD RST configuration */
        GPIO_InitStructure.GPIO_Pin   = LCD_RST_PIN; 
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

        /* LCD pwm configuration */
        GPIO_InitStructure.GPIO_Pin   = LCD_PWM_PIN; 
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

        GPIO_Init(LCD_PWM_PORT, &GPIO_InitStructure);
        GPIO_SetBits(LCD_PWM_PORT, LCD_PWM_PIN);
}

void lcd_fsmc_config(void) {
        FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
        FSMC_NORSRAMTimingInitTypeDef  p;
   
        /* Enable FSMC clock */
        RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  
/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 1 ----------------------------------------*/
        /* FSMC_Bank1_NORSRAM4 configuration */
        p.FSMC_AddressSetupTime = 1;
        p.FSMC_AddressHoldTime = 0;
        p.FSMC_DataSetupTime = 9;
        p.FSMC_BusTurnAroundDuration = 0;
        p.FSMC_CLKDivision = 0;
        p.FSMC_DataLatency = 0;
        p.FSMC_AccessMode = FSMC_AccessMode_A;
        /* Color LCD configuration ------------------------------------
           LCD configured as follow:
              - Data/Address MUX = Disable
              - Memory Type = SRAM
              - Data Width = 16bit
              - Write Operation = Enable
              - Extended Mode = Enable
              - Asynchronous Wait = Disable */

        FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAM1;
        FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
        FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_SRAM;
        FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
        FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait      = FSMC_AsynchronousWait_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
        FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
        FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
        FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
        FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
        FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
        FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
        FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &p;

        FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   

        /* Enable FSMC NOR/SRAM Bank1 */
        FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}


void lcd_init(void) {
        /* Configure the LCD Control pins */
        lcd_gpio_config();
        
        /* Configure the FSMC Parallel interface */
        lcd_fsmc_config();

        _delay_(5); 
        
        /* Reset LCD */
        GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);    
        _delay_(10);  
        GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);

        /*
        SSD2119Init(void)
        */
        /* Enter sleep mode (if we are not already there).*/
        lcd_write_reg(SSD2119_SLEEP_MODE_1_REG, 0x0001);

        /* Set initial power parameters. */
        lcd_write_reg(SSD2119_PWR_CTRL_5_REG, 0x00B2);
        lcd_write_reg(SSD2119_VCOM_OTP_1_REG, 0x0006);
  
        /* Start the oscillator.*/
        lcd_write_reg(SSD2119_OSC_START_REG, 0x0001);

        /* Set pixel format and basic display orientation (scanning direction).*/
        lcd_write_reg(SSD2119_OUTPUT_CTRL_REG,       0x30EF);
        lcd_write_reg(SSD2119_LCD_DRIVE_AC_CTRL_REG, 0x0600);

        /* Exit sleep mode.*/
        lcd_write_reg(SSD2119_SLEEP_MODE_1_REG, 0x0000);
        _delay_(5);
          
        /* Configure pixel color format and MCU interface parameters.*/
        lcd_write_reg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);

        /* Set analog parameters */
        lcd_write_reg(SSD2119_SLEEP_MODE_2_REG, 0x0999);
        lcd_write_reg(SSD2119_ANALOG_SET_REG,   0x3800);

        /* Enable the display */
        lcd_write_reg(SSD2119_DISPLAY_CTRL_REG, 0x0033);

        /* Set VCIX2 voltage to 6.1V.*/
        lcd_write_reg(SSD2119_PWR_CTRL_2_REG, 0x0005);

        /* Configure gamma correction.*/
        lcd_write_reg(SSD2119_GAMMA_CTRL_1_REG,  0x0000);
        lcd_write_reg(SSD2119_GAMMA_CTRL_2_REG,  0x0303);
        lcd_write_reg(SSD2119_GAMMA_CTRL_3_REG,  0x0407);
        lcd_write_reg(SSD2119_GAMMA_CTRL_4_REG,  0x0301);
        lcd_write_reg(SSD2119_GAMMA_CTRL_5_REG,  0x0301);
        lcd_write_reg(SSD2119_GAMMA_CTRL_6_REG,  0x0403);
        lcd_write_reg(SSD2119_GAMMA_CTRL_7_REG,  0x0707);
        lcd_write_reg(SSD2119_GAMMA_CTRL_8_REG,  0x0400);
        lcd_write_reg(SSD2119_GAMMA_CTRL_9_REG,  0x0a00);
        lcd_write_reg(SSD2119_GAMMA_CTRL_10_REG, 0x1000);

        /* Configure Vlcd63 and VCOMl */
        lcd_write_reg(SSD2119_PWR_CTRL_3_REG, 0x000A);
        lcd_write_reg(SSD2119_PWR_CTRL_4_REG, 0x2E00);

        /* Set the display size and ensure that the GRAM window is set to allow
           access to the full display buffer.*/
        lcd_write_reg(SSD2119_V_RAM_POS_REG, (LCD_PIXEL_HEIGHT-1) << 8);
        lcd_write_reg(SSD2119_H_RAM_START_REG, 0x0000);
        lcd_write_reg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);

        lcd_write_reg(SSD2119_X_RAM_ADDR_REG, 0x00);
        lcd_write_reg(SSD2119_Y_RAM_ADDR_REG, 0x00);
  
        /* clear the lcd  */
        unsigned long ulCount;
        lcd_write_reg(SSD2119_RAM_DATA_REG, 0x0000);
        for(ulCount = 0; ulCount < (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); ulCount++)
        {
                lcd_write_ram(0x0000);
        }
}

void lcd_set_cursor(uint16_t x_pos, uint16_t y_pos) {
        /* Set the X address of the display cursor.*/
        lcd_write_reg(SSD2119_X_RAM_ADDR_REG, x_pos);

        /* Set the Y address of the display cursor.*/
        lcd_write_reg(SSD2119_Y_RAM_ADDR_REG, y_pos);
}

void lcd_fill(uint16_t colour) {
        uint32_t index = 0;
        
        // Zero the cursor and draw at every position
        lcd_set_cursor(0x00, 0x00); 
        lcd_write_ram_prepare();
        for(index = 0; index < LCD_PIXEL_HEIGHT*LCD_PIXEL_WIDTH; index++)
        {
                LCD_DATA = colour;
        }  
}

void lcd_draw_pixel(int16_t x, int16_t y, uint16_t colour) {
        // If x or y are outside the accepted range, return
        if( (x<0 || x>LCD_PIXEL_WIDTH-1) || (y<0 || y>LCD_PIXEL_HEIGHT-1) ) {
                return; 
        }
        lcd_set_cursor(x, y);
        lcd_write_ram_prepare();
        lcd_write_ram(colour);
}
        
void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
        int16_t dx=x1-x0;
        int16_t dy=y1-y0;
        int16_t x=x0;
        int16_t y=y0;
         
        int16_t p = 2*dy - dx;
         
        while(x<x1) {
                if(p >= 0) {
                        lcd_draw_pixel(x, y, colour);
                        y = y++;
                        p = p + 2*dy - 2*dx;
                } else {
                        lcd_draw_pixel(x, y, colour);
                        p = p + 2*dy;
                }
                x=x+1;
        }
}