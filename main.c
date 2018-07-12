/**
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    main.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   LCD Diaplay DEMO      
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "mech.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "lcd_graphics.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "     STM32F4xx      " 
#define MESSAGE2   " Joel is a dick bag " 
#define MESSAGE3   " Chog is cool as ice" 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
        volatile uint32_t dlycnt;

        /* 
         * At this stage the microcontroller clock setting is already configured, 
         * this is done through SystemInit() function which is called from startup
         * file (startup_stm32f4xx.s) before to branch to application main.
         * To reconfigure the default setting of SystemInit() function, refer to
         * system_stm32f4xx.c file
        */

        /* wait the power stable */
        for (dlycnt = 0; dlycnt < 10000000; dlycnt++);

        lcd_init();

        /* Display message on stm32f4_discovery LCD **********************************/
        /* Clear the LCD */ 
        lcd_fill(0x0001);

        int16_t colour = 0;
        int32_t y, x;
        for (y=0; y<LCD_PIXEL_HEIGHT; y++) {
                for (x=0; x<LCD_PIXEL_WIDTH; x++) {
                        colour = (0x20*(x + y)) / (LCD_PIXEL_HEIGHT + LCD_PIXEL_WIDTH)<<5;
                        lcd_draw_pixel(x, y, colour);
                }
        }

        //uint16_t data[16*16] = {0};
        int i = 0;

        uint16_t x_pos = 0;
        uint8_t animation = 0;
        while(1) {
                i++;
                animation = i%6;
                x_pos = i%LCD_PIXEL_WIDTH;
                GPIO_SetBits(GPIOE, GPIO_Pin_0);
                lcd_fill( 0xffff );
                lcd_draw_sprite( x_pos, 100, 32, 32, mech_data[animation]);
    
                lcd_draw_sprite(  10, 130, 32, 32, mech_data[animation]);
                lcd_draw_sprite( 230, 20,  32, 32, mech_data[animation]);
                lcd_draw_sprite( 220, 50,  32, 32, mech_data[animation]);
                for (dlycnt = 0; dlycnt < 50000; dlycnt++);
                GPIO_ResetBits(GPIOE, GPIO_Pin_0);
                for (dlycnt = 0; dlycnt < 500000; dlycnt++);
    //GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13);
        }

        while(1);

        x = 0;
        y = 0;
        int16_t dx = 3;
        int16_t dy = 2;
        while(1) {
                colour += lcd_read_pixel(x,y);
                lcd_draw_pixel(x, y, colour);
                x+=dx;
                y+=dy;
                if (x < 0) {
                        x  = 0;
                        dx = 3;
                } else if (x >= LCD_PIXEL_WIDTH) {
                        x = LCD_PIXEL_WIDTH-1;
                        dx = -3;
                }

                if (y < 0) {
                        y  = 0;
                        dy = 2;
                } else if (y >= LCD_PIXEL_HEIGHT) {
                        y = LCD_PIXEL_HEIGHT-1;
                        dy = -2;
                }
        }

        //lcd_draw_line(0, 0, 100, 100, 0xff);

        /* LCD RGB Test */
        //LCD_RGB_Test();
        while (1);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
        /* 
         * User can add his own implementation to report the file name and line number,
         * ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
         */
        while (1)
        {
        }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************** COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
