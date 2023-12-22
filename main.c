/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9225.h"
#include "stdio.h"
#include "stdint.h"
#include <string.h>
#include "RTC.h"
#include "button.h"
#include "DHT.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;
/* USER CODE BEGIN PV */
uint8_t second_present, min_present, hour_present, date_present, month_present, year_present;
uint8_t second_temp, min_temp, hour_temp, date_temp, month_temp, year_temp;
uint8_t date_max;
char str1[100];
char str2[100];
Button_TypeDef;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
DateTime_t datetime;
Button_TypeDef buttonmode;
Button_TypeDef buttonup;
Button_TypeDef buttondown;
typedef enum
{
	BINHTHUONG = 0,
	CHINHPHUT = 1,
	CHINHGIO = 2,
	CHINHNGAY = 3,
}Mode;

Mode mode; // khai báo là biến "mode" có kiểu dữ liệu "Mode"

//////////////////////////////////////////////////////////////CHƯƠNG TRÌNH PHỤC VỤ NGẮT//////////////////////////////////////////////////////////////
void btn_pressing_callback()
{ if (ButtonX->GPIO_Pin = GPIO_PIN_0)
	switch (mode)
	{
	case BINHTHUONG: // mode = 0
  datetime.date = date_temp; //gán thời gian vừa cài đặt xong cho biến official (biến mà RTC sẽ tự động cập nhật)
		mode = CHINHPHUT;  // gán mode 0 thành 1
		break; // đang ở case BINHTHUONG thì chương trình sẽ gán cho biến mode (dòng 84), sau đó break ra khỏi switch, rồi khi này chương trình sẽ không thực hiện lệnh gì trong khi đợi điều kiện của 1 trong 3 cái if ở ngắt là đúng;
    // Vậy lúc này, nếu bấm button_mode một lần nữa thì chương trình sẽ xét switch rồi nhảy vô case CHINHGIO vì biến "mode" đã được gán = 2 ở dòng 88; Else nhấn button_up một lần thì chương trình
    // sẽ nhảy vào chạy hàm btn_adjust(up) hoặc nếu nhấn button_down thì chương trình sẽ nhảy vào chạy hàm btn_adjust(down).
	case CHINHPHUT: // mode = 1
		mode = CHINHGIO; // gán mode = 2
		break;
	case CHINHGIO: // mode = 2
  datetime.min = min_temp; //gán thời gian vừa cài đặt xong cho biến official (biến mà RTC sẽ tự động cập nhật)
		mode = CHINHNGAY; // gán mode = 3
		break;
	case CHINHNGAY:      // mode = 3
  datetime.hour = hour_temp; //gán thời gian vừa cài đặt xong cho biến official (biến mà RTC sẽ tự động cập nhật)
		mode = BINHTHUONG; // gán mode = 0
		break;
	}
  ////////////////BUTTON_UP///////////////
	if (ButtonX->GPIO_Pin = GPIO_PIN_1)
	{
		btn_adjust(up);
	}
  ///////////////BUTTON_DOWN////////////
	if (ButtonX->GPIO_Pin = GPIO_PIN_2)
		{
	  btn_adjust(down);
		}
}

//                    Tạo hàm sẽ chạy khi nhấn button_up
	void btn_adjust(up)
	{
	switch(mode)
		{
		case CHINHPHUT:
			// viet code de cong 1 phut va khi den 60 thi quay lai
            min_temp = datetime.min; // gán giá trị của "biến official" (RTC đang dùng để cập nhật) cho biến temporary.
        while(1){
            fill_rectangle(110, 100, 130, 110, COLOR_WHITE); // blink tại thông số "minute"
            if (ButtonX->GPIO_Pin = GPIO_PIN_1)
            {
            min_temp ++;
            if (min_temp = 60)
            {
                min_temp = 0;
            }
		    draw_string(40, 100, COLOR_BLACK, 1, str1);
            }
        }
			 break;
		case CHINHGIO:
			// viet code de cong 1 gio va khi den 60 thi quay lai
            hour_temp = datetime.hour; // gán giá trị của "biến official" (RTC đang dùng để cập nhật) cho biến temporary.
        while(1){
            fill_rectangle(110, 100, 130, 110, COLOR_WHITE); // blink tại thông số "hour"
            if (ButtonX->GPIO_Pin = GPIO_PIN_1)
            {
            hour_temp ++;
            if (hour_temp = 24)
            {
                hour_temp = 0;
            }
		    draw_string(40, 100, COLOR_BLACK, 1, str1);
            }
        }
			 break;
		case CHINHNGAY:
			// viet code de cong 1 ngay va biet duoc so ngay trong thang de quay lai
            date_temp = datetime.date; // gán giá trị của "biến official" (RTC đang dùng để cập nhật) cho biến temporary.
        while(1){
            fill_rectangle(85, 115, 105, 135, COLOR_WHITE); // Blink tại thông số "date"
            if (ButtonX->GPIO_Pin = GPIO_PIN_1)
            {
            date_temp ++;
            switch (datetime.month)   // xét xem tháng hiện tại có tổng bao nhiêu ngày
            {
            case 1: case 3: case 5:case 7: case 8: case 10: case 12:
                date_max = 31 + 1;
                break;
            case 4: case 6: case 9: case 11:
                date_max = 30 + 1;
            case 2:
                if ((datetime.year - 2014) % 4 == 0)
            {
                date_max = 29 + 1;
            }
            else
            {
                date_max = 28 + 1;
            }
                break;
            default:
                break;
            }
            if (date_temp == date_max)
            {
                date_temp = 1;
            }
		    draw_string(40, 120, COLOR_BLACK, 1, str2); // in cả string 2 (nhưng chỉ có thông số date thay đổi).
            }
        }
			 break;
		}
	}
  //                     Tạo hàm sẽ chạy khi nhấn button_down
	void btn_adjust(down)
	{
    	switch(mode)
		{
		case CHINHPHUT:
			// viet code de tru 1 phut va khi den 0 thi quay lai
            min_temp = datetime.min; // gán giá trị của "biến official" (RTC đang dùng để cập nhật) cho biến temporary.
        while(1){
            fill_rectangle(110, 100, 130, 110, COLOR_WHITE); // blink tại thông số "minute"
            if (ButtonX->GPIO_Pin = GPIO_PIN_1)
            {
            min_temp --;
            if (min_temp = -1)
            {
                min_temp = 59;
            }
		    draw_string(40, 100, COLOR_BLACK, 1, str1);
            }
        }
			 break;
		case CHINHGIO:
			// viet code de tru 1 gio va khi den 0 thi quay lai
            hour_temp = datetime.hour; // gán giá trị của "biến official" (RTC đang dùng để cập nhật) cho biến temporary.
        while(1){
            fill_rectangle(110, 100, 130, 110, COLOR_WHITE); // blink tại thông số "hour"
            if (ButtonX->GPIO_PIN = GPIO_PIN_1)
            {
            hour_temp --;
            if (hour_temp = -1)
            {
                hour_temp = 23;
            }
		    draw_string(40, 100, COLOR_BLACK, 1, str1);
            }
        }
			 break;
		case CHINHNGAY:
			// viet code de tru 1 ngay va biet duoc so ngay trong thang de quay lai
            date_temp = datetime.date; // gán giá trị của "biến official" (RTC đang dùng để cập nhật) cho biến temporary.
        while(1){
            fill_rectangle(85, 115, 105, 135, COLOR_WHITE); // Blink tại thông số "date"
            if (ButtonX->GPIO_PIN = GPIO_PIN_1)
            {
            date_temp --;
            switch (datetime.month)   // xét xem tháng hiện tại có tổng bao nhiêu ngày
            {
            case 1: case 3: case 5:case 7: case 8: case 10: case 12:
                date_max = 31 + 1;
                break;
            case 4: case 6: case 9: case 11:
                date_max = 30 + 1;
            case 2:
                if ((datetime.year - 2014) % 4 == 0) // nếu năm hiện tại là năm nhuận thì số tổng số ngày của tháng 2 ở năm hiện tại là 29, còn không thì là 28.
            {
                date_max = 29 + 1;
            }
            else
            {
                date_max = 28 + 1;
            }
                break;
            default:
                break;
            }
            if (date_temp == 0)
            {
                date_temp = date_max - 1;
            }
		    draw_string(40, 120, COLOR_BLACK, 1, str2); // in cả string 2 (nhưng chỉ có thông số date thay đổi).
            }
        }
			 break;
		}
	}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
 /////////////////////////////////////////////////////////////////////////////CHƯƠNG TRÌNH CHÍNH/////////////////////////////////////////////////////////////////////
int main(void)
{
  /* USER CODE BEGIN 1 */
int mode = 0; // Cho chương trình chạy ở MODE 0 trước tiên khi mới bắt đầu.
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
button_init(&buttonmode, GPIOA, GPIO_PIN_0);
button_init(&buttonup, GPIOA, GPIO_PIN_1);
button_init(&buttondown, GPIOA, GPIO_PIN_2);
  lcd_init();
  fill_rectangle(0, 0, WIDTH, HEIGHT, COLOR_WHITE);
  draw_string(0, 50, COLOR_GREENYELLOW, 5, "HELLO");
  HAL_Delay(2500);
  fill_rectangle(0, 0, WIDTH, HEIGHT, COLOR_WHITE);

    // Cài đặt thời gian ban đầu
  datetime.second = 15;
  datetime.min = 44;
  datetime.hour = 12;
  datetime.day = 8;
  datetime.date = 58;
  datetime.month = 12;
  datetime.year = 23;
  RTC_WriteTime(&datetime);

  draw_string(20, 50, COLOR_GOLD, 5, "QMDP");
  HAL_Delay(2500);
  fill_rectangle(0, 0, 176, 220, COLOR_WHITE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	button_handle(&buttonmode);
	button_handle(&buttonup);
	button_handle(&buttondown);

    		 RTC_ReadTime(&datetime);
		  second_temp = datetime.second;
		  min_temp = datetime.min;
		  hour_temp = datetime.hour;
		  date_temp = datetime.date;
		  month_temp = datetime.month;
		  year_temp = datetime.year;
		  sprintf(str1, "Time %.2d:%.2d:%.2d", datetime.hour, datetime.min, datetime.second);

// biến có đuôi _present là biến được chọn làm mốc. Nếu biến _temp khác biến _present thì có nghĩa là thời gian đã có sự thay đổi;
// Ngay lúc này, thông số đó sẽ được cập nhật lên màn hình
		  if(second_temp != second_present)
		  {
			  fill_rectangle(140, 100, 160, 110, COLOR_WHITE);
			  second_present = second_temp;
		  }
		  draw_string(40, 100, COLOR_BLACK, 1, str1);

		  if(min_temp != min_present)
		  	  {
		  		  fill_rectangle(110, 100, 130, 110, COLOR_WHITE);
		  		  min_present = min_temp;
		  	  }
		  draw_string(40, 100, COLOR_BLACK, 1, str1);

		  if(hour_temp != hour_present)
		 	  	  {
		 	  		  fill_rectangle(85, 100, 105, 110, COLOR_WHITE);
		 	  		  hour_present = hour_temp;
		 	  	  }
		  draw_string(40, 100, COLOR_BLACK, 1, str1);

		  sprintf(str2,"Date %.2d/%.2d/%.2d",datetime.date,datetime.month,datetime.year );
//	// để sprintf ở đây cho nó chính xác, date, month, year lâu lắm
//		  //để sprintf ở đây để convert, lệnh in lên LCD nằm trong thư viện của LCD
		  if(date_temp != date_present)
		  	 	  	  {
		  	 	  		  fill_rectangle(85, 115, 105, 135, COLOR_WHITE);
		  	 	  		  date_present = date_temp;
		  	 	  	  }
		  	  draw_string(40, 120, COLOR_BLACK, 1, str2);

		  	if(month_temp != month_present)
		  		 	  	  {
		  		 	  		  fill_rectangle(110, 115, 130, 135, COLOR_WHITE);
		  		 	  		  month_present = month_temp;
		  		 	  	  }
		  		  draw_string(40, 120, COLOR_BLACK, 1, str2);

		  		if(year_temp != year_present)
		  			 	  	  {
		  			 	  		  fill_rectangle(135, 115, 160, 135, COLOR_WHITE);
		  			 	  		  year_present = year_temp;
		  			 	  	  }
		  			  draw_string(40, 120, COLOR_BLACK, 1, str2);
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
