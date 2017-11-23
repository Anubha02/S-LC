//set pin numbers

const int ledPin = 13;   //the number of the LED pin
const int ldrPin = A1;  //the number of the LDR pin

int motorPin = A2; // pin that turns on the motor
int blinkPin = 13; // pin that turns on the LED
int watertime = 2; // how long to water in seconds
float waittime = 0.01; // how long to wait between watering, in minutes

#include <dht.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

dht DHT;

#define DHT22_PIN 6

void setup()
{
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(ledPin, OUTPUT);  //initialize the LED pin as an output
  pinMode(ldrPin, INPUT);   //initialize the LDR pin as an input
  //Serial.println("Type \tstatus \tHumidity (%) \tTemperature (C) \tMoisture Content \tLight");
  Serial.println("Humidity (%) \tTemperature (C) \tMoisture Content \tLight");

  pinMode(motorPin, OUTPUT); // set A0 to an output so we can use it to turn on the transistor
  pinMode(blinkPin, OUTPUT); // set pin 13 to an output so we can use it to turn on the LED

//Piezo Sensor Pin to be coonected as output.
  pinMode(10, OUTPUT);
  beep(50);
  beep(50);
  beep(50);
  delay(1000);
 }

void loop()
{
  // READ DATA
  //Serial.print("DHT22 \t");
  int chk = DHT.read22(DHT22_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
   // Serial.print("OK \t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  
// DISPLAY DATA
  
  float temp = DHT.temperature;
  float humi = DHT.humidity;
  int sensorValue = analogRead(A0);
/*
  // if sensor value is more then 800 then dry and then motor on
  if(sensorValue > 800)
  {
  digitalWrite(motorPin, HIGH); // turn on the motor
  //digitalWrite(blinkPin, HIGH); // turn on the LED
  delay(watertime*1000);        // multiply by 1000 to translate seconds to milliseconds
  
  digitalWrite(motorPin, LOW);  // turn off the motor
 // digitalWrite(blinkPin, LOW);  // turn off the LED
  delay(waittime*60000);        // multiply by 60000 to translate minutes to milliseconds
  }
*/

if(sensorValue > 800) {
      digitalWrite(motorPin, HIGH);
      delay(watertime*1000);
      digitalWrite(motorPin, LOW);
      delay(waittime*60000);
  }/* else {
     digitalWrite(motorPin, LOW); 
  }*/


  //print the values on the lcd screen
  Serial.print(humi);
  Serial.print(" \t\t");
  Serial.print(temp);
  Serial.print(" \t\t\t");
  Serial.print(sensorValue);
  Serial.print(" \t\t\t");
  lcd.setCursor(0,0);
  lcd.print("T-");
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("H-");
  lcd.print(humi);
  lcd.print("% ");
  lcd.setCursor(9,0);
  lcd.print("M-");
  //lcd.print(sensorValue);
  if(sensorValue > 800) {
      lcd.print("DRY");
  }
      else{
        lcd.print("MOI");
      }
  
  lcd.setCursor(9,1);
  lcd.print("L-");
  int ldrStatus = analogRead(ldrPin);   //read the status of the LDR value

 //check if temperature is greater than equal to 28.2 then buzzer beeps sound.
  if(temp >= 30)
  {
    beep(100);  //calls a function beep().
  }
   //check if the LDR status is <= 100
  //if it is, the LED is HIGH
   if (ldrStatus <=100) 
   {
    digitalWrite(ledPin, HIGH);               //turn LED on
    Serial.println("DARK_SIGNAL_DANGER"); 
    lcd.print("LOW");
   }
  else
  {
    digitalWrite(ledPin, LOW);          //turn LED off
    Serial.println("LIGHT_SIGNAL_FINE");
    lcd.print("FNE");
  } 
  delay(500);

}

void beep(unsigned char delayms){
  analogWrite(10, 20);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(10, 0);       // 0 turns it off
  //delay(delayms);          // wait for a delayms ms   
}  
