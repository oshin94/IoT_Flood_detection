/* This is the code for the Arduino Uno which updates the LCD and sends SMS using the GSM module */

#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

SoftwareSerial uno(9,10);

LiquidCrystal_I2C lcd(0x27,16,2);

String data;
bool A_flag=false;
bool B_flag=false;

void lcd_update(int x){
  if (x == 1){
    lcd.setCursor(0,0);
    lcd.print("Location A:Alert");
  }
  else if (x == 2){
    lcd.setCursor(0,1);
    lcd.print("Location B:Alert");
  }
  else if (x == 3){
    lcd.setCursor(0,0);
    lcd.print("Location A: Safe  ");
  }
  else if (x == 4){
    lcd.setCursor(0,1);
    lcd.print("Location B: Safe  ");
  }
}


void setup(){
  uno.begin(115200);
  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Location A: Safe  ");
  lcd.setCursor(0,1);
  lcd.print("Location B: Safe  ");
}

void loop(){
  if (uno.available()> 0){
    data = uno.readString();
    data.trim();
    Serial.println(data);

    if (data == "Sens2: 1"){
      if (!B_flag){
        B_flag = true;
        lcd_update(2);
        digitalWrite(13, HIGH);
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); //replace xxxxxxxxxx by the number where message is to be sent
        delay(500);
        if (!A_flag){
          Serial.println("The river has risen to dangerous levels in Location B, evacuate immediately to Location A");
        }
        else{
          Serial.println("The river has risen to dangerous levels in Location B as well as A, evacuate immediately from both locations");
        }
        delay(500);
        Serial.println((char)26);
        delay(500);
        digitalWrite(13, LOW);
        Serial.println("Message sent");
      }
    }
    else if (data == "Sens1: 1"){
      if (!A_flag){
        A_flag = true;
        lcd_update(1);
        digitalWrite(13, HIGH);
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); //replace xxxxxxxxxx by the number where message is to be sent
        delay(500);
        if (!B_flag){
          Serial.println("The river has risen to dangerous levels in Location A, evacuate immediately to Location B");
        }
        else{
          Serial.println("The river has risen to dangerous levels in Location A as well as B, evacuate immediately from both locations");
        }
        delay(500);
        Serial.println((char)26);
        delay(500);
        digitalWrite(13, LOW);
        Serial.println("Message sent");
      }
    }
    else if (data == "Sens2: 0"){
      if (B_flag){
        B_flag = false;
        lcd_update(4);
        digitalWrite(13, HIGH);
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); //replace xxxxxxxxxx by the number where message is to be sent
        delay(500);
        Serial.println("Location B is now Safe");
        delay(500);
        Serial.println((char)26);
        delay(500);
        digitalWrite(13, LOW);
        Serial.println("Message sent");
      }
    }
    else if (data == "Sens1: 0"){
      if (A_flag){
        A_flag = false;
        lcd_update(3);
        digitalWrite(13, HIGH);
        Serial.println("AT+CMGF=1");
        delay(500);
        Serial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); //replace xxxxxxxxxx by the number where message is to be sent
        delay(500);
        Serial.println("Location A is now Safe");
        delay(500);
        Serial.println((char)26);
        delay(500);
        digitalWrite(13, LOW);
        Serial.println("Message sent");
      }
    }
  }
}
