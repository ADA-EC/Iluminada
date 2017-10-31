#include "defines.h"
#include "main.h"
#include "core_cm4.h"
#include "stm32f4xx_conf.h"
#include "mp3dec.h"
#include "Audio.h"
#include <string.h>
#include "tm_stm32f4_fatfs.h"

// Macros
#define f_tell(fp)		((fp)->fptr)
#define BUTTON			(GPIOA->IDR & GPIO_Pin_0)

// Variables
volatile uint32_t		time_var1, time_var2;
USB_OTG_CORE_HANDLE		USB_OTG_Core;
USBH_HOST				USB_Host;
RCC_ClocksTypeDef		RCC_Clocks;
volatile int			enum_done = 0;

// MP3 Variables
#define FILE_READ_BUFFER_SIZE 8192
MP3FrameInfo			mp3FrameInfo;
HMP3Decoder				hMP3Decoder;
FIL						file;
char					file_read_buffer[FILE_READ_BUFFER_SIZE];
volatile int			bytes_left;
char					*read_ptr;

// Private function prototypes
static void AudioCallback(void *context,int buffer);
static uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist,
		uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize);
static void play_mp3(char* filename);
static FRESULT play_directory (const char* path, unsigned char seek);

/*
 * Main function. Called when startup code is done with
 * copying memory and setting up clocks.
 */
int main(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// SysTick interrupt each 1ms
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	// GPIOD Peripheral clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Initialize USB Host Library
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);

	//Fatfs object
	FATFS FatFs;
	//File object
	FIL fil;
	//Free and total space
	uint32_t total, free;

	//Initialize system
	SystemInit();
	//Initialize delays
	TM_DELAY_Init();
	//Initialize LEDs
	//TM_DISCO_LedInit();

	//Mount drive
	if (f_mount(&FatFs, "", 1) == FR_OK) {
		//Mounted OK, turn on RED LED
		//TM_DISCO_LedOn(LED_RED);

		//Try to open file
		if (f_open(&fil, "1stfile.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
			//File opened, turn off RED and turn on GREEN led
			//TM_DISCO_LedOn(LED_GREEN);
			//TM_DISCO_LedOff(LED_RED);

			//If we put more than 0 characters (everything OK)
			if (f_puts("First string in my file\n", &fil) > 0) {
				if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {
					//Data for drive size are valid
				}

				//Turn on both leds
				//TM_DISCO_LedOn(LED_GREEN | LED_RED);
			}

			//Close file, don't forget this!
			f_close(&fil);
		}

		//Unmount drive, don't forget this!
		f_mount(0, "", 1);
	}

	while (1) {

	}
}

/*
 * Delay a number of systick cycles (1ms)
 */
void Delay(volatile uint32_t nTime) {
	time_var1 = nTime;
	while(time_var1){};
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {

}


/*
 * Called by the SysTick interrupt
 */
void TimingDelay_Decrement(void) {
	if (time_var1) {
		time_var1--;
	}
	time_var2++;
}
