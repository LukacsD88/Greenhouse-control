/*
*
* Folia humidity control
* Sensors: DHT22 2pieces , LDR sensor 1 piece
* Communication: nRF24L: used channel: 12
* Version: 1.0
**/
#include "DHT.h"
#include <SPI.h>  
#include "RF24.h"

#define DHTPIN                    4  
#define DHTTYPE                   DHT22
#define DHTPIN2                    5  
#define DHTTYPE2                   DHT22  
#define LDR_PIN                   0
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0
#define REF_RESISTANCE            5030  // measure this for best results
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
