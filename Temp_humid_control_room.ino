//include libraries for sensor,servo & LCD
#include <DHT11.h>
#include <Servo.h>
#include <LiquidCrystal.h>

//connect DHT11 temp sensor to pin 2
DHT11 dht11(2);
//define Servo as 'servo'
Servo servo;

//initialization
int temp =0;
int humid = 0;

int heat = 0;
int heater = 8;

int pos = 0;

int speed=0; 

//motor driver's pins
const int in1 = 4;
const int in2 = 6;
const int en_m = 7;

//LCD's pins
const int rs = 42, en_l = 43, d4 = 34 ,d5 = 32 ,d6 = 30 ,d7 = 28; 
LiquidCrystal lcd(rs, en_l, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);

  pinMode(heater,OUTPUT);
  pinMode(en_m,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  servo.attach(10);

  

}

void loop() {

  //storing temperature & humidity values
  int reading = dht11.readTemperatureHumidity(temp,humid);
  
  //checking for errors
  if (reading==0) {
    Serial.print ("Temperature: ");
    Serial.print (temp);
    Serial.print(" °C\tHumidity: ");
    Serial.print (humid);
    Serial.println(" %");

    //displaying on LCD
    lcd.setCursor(0,0);
    lcd.print(" Temp: ");
    lcd.print(temp);
    lcd.print("'C");

    lcd.setCursor(0,1);
    lcd.print(" Humid: ");
    lcd.print(humid);
    lcd.print("%");    
  } 
  else {
    Serial.println(DHT11::getErrorString(reading));
  }
  
  if (temp > 23 ){
      //slowly turn off heater(LEDs) when too hot 
      if (heat!=0){
      for (heat=255; heat>=0; heat-=5 ){
        analogWrite(heater,heat);
        delay(30);
      }
      heat=0;
    }
    //turn on the fan (servo rotate and motor spin)
      for (pos = 0; pos<=180; pos+=1){
          servo.write(pos);
          delay(10);
      }
      for (pos =180; pos>=0; pos-=1){
          servo.write(pos);
          delay(10);
      }
      speed = map(reading,0,50,0,255);// match the speed of spin with temp
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      analogWrite(en_m,speed);
  }

  else if(temp < 23  ){
    //turn off the motor
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    analogWrite(en_m,0);

    // slowly turn on heater(LEDs)
    if (heat!=255){
      for (heat=0; heat<=255; heat+=3 ){
        analogWrite(heater,heat);
        delay(40);
      }
      heat=255;
    }
      
    }
  

}
