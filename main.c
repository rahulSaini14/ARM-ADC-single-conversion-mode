#include "stm32f4xx_hal.h"             

ADC_HandleTypeDef adc;
uint32_t val;
void config(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef def;
	def.Pin = GPIO_PIN_0;
	def.Mode = GPIO_MODE_ANALOG;
	def.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&def);
}

void adc_config(){
	__HAL_RCC_ADC1_CLK_ENABLE();
	adc.Instance = ADC1;
	adc.Init.Resolution = ADC_RESOLUTION_8B;
	adc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	HAL_ADC_Init(&adc);
	
	ADC_ChannelConfTypeDef adc_ch;
	adc_ch.Rank = 1;
	adc_ch.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	adc_ch.Channel = ADC_CHANNEL_0;
	HAL_ADC_ConfigChannel(&adc,&adc_ch);
}

void sysTickConfig(){
		HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
		HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
		HAL_NVIC_EnableIRQ(SysTick_IRQn);
}
void sysTickHandler(){
		HAL_IncTick();
}
void delay(uint32_t k){
		for(uint32_t i=0;i<4000*k;i++);
}


int main(){
	HAL_Init();
	config();
	adc_config();
	sysTickConfig();
	while(1){
			HAL_ADC_Start(&adc);
			if(HAL_ADC_PollForConversion(&adc,5) == HAL_OK){
					val = HAL_ADC_GetValue(&adc);
			}
			HAL_ADC_Stop(&adc);
			delay(100);
	}
}
