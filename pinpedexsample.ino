//includy pro klávesnici
#include "MatrixKeypad.h"
#include <stdint.h>

//includy pro display
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

//globalni promene pro klavesnici
const uint8_t rown = 4; //4 rows
const uint8_t coln = 3; //3 columns
uint8_t rowPins[rown] = {2, 3, 4, 5}; //frist row is connect to pin 10, second to 9...
uint8_t colPins[coln] = {8, 9, 10}; //frist column is connect to pin 6, second to 5...
char keymap[rown][coln] = 
  {{'1','2','3'}, //key of the frist row frist column is '1', frist row second column column is '2'
   {'4','5','6'}, //key of the second row frist column is '4', second row second column column is '5'
   {'7','8','9'},
   {'*','0','#'}};
MatrixKeypad_t *keypad; //keypad is the variable that you will need to pass to the other functions

char key;

// globalni promnene
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int show = -1;

byte dotOff[] = { 0b00000, 0b01110, 0b10001, 0b10001,
                  0b10001, 0b01110, 0b00000, 0b00000 };
byte dotOn[] = { 0b00000, 0b01110, 0b11111, 0b11111,
                 0b11111, 0b01110, 0b00000, 0b00000 };


String text = "";
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  keypadInit(); //toto provede incializaci keypad
  lcdInit(); //toto provede incializaci  ldc
}
/*
slozeni pro incializaci LCD
*/
void lcdInit(){
  int error;


  Serial.println("LCD...");

  // wait on Serial to be available on Leonardo
  while (!Serial)
    ;

  Serial.println("Probing for PCF8574 on address 0x27...");

  // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
    lcd.begin(16, 2);  // initialize the lcd

    lcd.createChar(1, dotOff);
    lcd.createChar(2, dotOn);

  } else {
    Serial.println(": LCD not found.");
  }  // if

}  // setup()

void keypadInit(){pinMode(LED_BUILTIN, OUTPUT);

	keypad = MatrixKeypad_create((char*)keymap /* don't forget to do this cast */, rowPins, colPins, rown, coln); //creates the keypad object
}

void loop() {
  // put your main code here, to run repeatedly:
  keypadloop();
  lcdloop();
  blink();

  delay(20);
} 

void keypadloop(){
	MatrixKeypad_scan(keypad); //scans for a key press event
	if(MatrixKeypad_hasKey(keypad)){ //if a key was pressed
		key = MatrixKeypad_getKey(keypad); //get the key
    
		Serial.print(key); //prints the pressed key to the serial output
    if(key == '*'){
    text += key;
    }

    else if(text.length() >= 4){
    
    Serial.println("zadano vice jak 4 cisel");
    }
    else{
        text += key;
    }
  }
}
void blink () {
	static int led_state = LOW;
	if(led_state == HIGH){
		led_state = LOW;
	}
	else {
		led_state = HIGH;
	}
	digitalWrite(LED_BUILTIN, led_state);
}

void lcdloop(){
  if (show == 0) {
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("Hello LCD");
    lcd.setCursor(0, 1);
    lcd.print(text);
    }
}

