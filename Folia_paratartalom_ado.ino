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

//DHT22 sensor defines:
#define DHTPIN_01                 4  
#define DHTTYPE_01                DHT22
#define DHTPIN_02                 5  
#define DHTTYPE_02                DHT22  

//LDR sensor defines:
#define LDR_PIN                   0
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0
#define REF_RESISTANCE            5030  // measure this for best results
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405


//RF24 Setup
RF24 myRadio (7,8);
byte addresses [] [6] = {"0"};
const int led_pin = 13;

int timetoprint = 2000;
int current_time = 0;
int ellapsed_time = 0;


struct package_sensor1
{
  float temperature;
  float humidity;
  float lightsensor;
};

typedef struct package_sensor1 Package_sensor1;
Package_sensor1 data_sensor1;

struct package_sensor2
{
  float temperature;
  float humidity;
  float lightsensor;
};

typedef struct package_sensor2 Package_sensor2;
Package_sensor2 data_sensor2;


//DHT Sensor setup
DHT dht_sensor1(DHTPIN_01, DHTTYPE_01);
DHT dht_sensor2(DHTPIN_02, DHTTYPE_02);



void setup() {

Serial.begin(9600);
pinMode(led_pin, OUTPUT);
dht_sensor1.begin();
dht_sensor2.begin();

//start RF24:
myRadio.begin();  
myRadio.setChannel(12); 
myRadio.setPALevel(RF24_PA_MAX);
myRadio.setDataRate( RF24_250KBPS ) ; 
myRadio.openWritingPipe( addresses[0]);

//add delay 1s:    
delay(1000); 
current_time = millis();

}

void loop() {


digitalWrite(led_pin, HIGH); //Flash LED for transmitting

readSensor1();
readSensor2();
float Ldrluxval = Ldrcalc();
data_sensor1.lightsensor = Ldrluxval;



ellapsed_time = millis()-current_time;

if (ellapsed_time  >= timetoprint)
{
current_time = millis();  
ellapsed_time = 0;
//Serial port print sensor1:
Serial.println("Szenzor 1:");
Serial.print("T: ");
Serial.print(data_sensor1.temperature);
Serial.print(" H: ");
Serial.println(data_sensor1.humidity);

//Serial port print sensor2:
Serial.println("Szenzor 2:");
Serial.print("T: ");
Serial.print(data_sensor2.temperature);
Serial.print(" H: ");
Serial.println(data_sensor2.humidity);

//LDR value

Serial.println("LDR szenzor:");
Serial.print("Light=");
Serial.println(Ldrluxval);
myRadio.write(&data_sensor1, sizeof(data_sensor1));
myRadio.write(&data_sensor2, sizeof(data_sensor2));
digitalWrite(led_pin, LOW);

}
delay(1000);
  

}

void readSensor1()
  {
  
    data_sensor1.humidity    = dht_sensor1.readHumidity();
    data_sensor1.temperature = dht_sensor1.readTemperature();
    
  }



void readSensor2()
  {
  
  data_sensor2.humidity    = dht_sensor2.readHumidity();
  data_sensor2.temperature = dht_sensor2.readTemperature();
  
  
  }

float Ldrcalc()
{
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;

  ldrRawData = analogRead(LDR_PIN);

  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  // debug: print out the results
  //Serial.print("LDR Raw Data   : "); Serial.println(ldrRawData);
  //Serial.print("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  //Serial.print("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  //Serial.print("LDR Illuminance: "); Serial.print(ldrLux); Serial.println(" lux");  

  return ldrLux;  
  }















