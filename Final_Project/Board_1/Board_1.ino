
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#include <Servo.h>


#define LCD_R5      13
#define LCD_E       12
#define LCD_DB4     11
#define LCD_DB5     10
#define LCD_DB6     9
#define LCD_DB7     8
#define PIN         7
#define NUMPIXELS   8
#define SPEAKER     6
#define SERVOR_PIN  5
#define SENSOR      A0


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Servo myservo;

String command = "";
int pos = 0;

LiquidCrystal lcd_1(LCD_R5, LCD_E, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

void setup()
{
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  lcd_1.print("hello world!");
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  
  pinMode(SPEAKER, OUTPUT);
  pinMode(SENSOR, INPUT);
  myservo.attach(SERVOR_PIN);
}

void loop()
{
  readInput();
  if(command == "A"){
    ServoRun(5);
    ClearCommand();
  }
  sleep(300);
}

void sleep(int s){
  for(int i=1;i<s;i++){
    delay(1);
    readInput();
  }
}

void readInput(){
   if (Serial.available()){
    command = Serial.readStringUntil('\r\n');
    command.trim();
    Serial.println(command);
    lcd_1.setCursor(0, 1);
    lcd_1.print(command);
  }
}

void LedWaterIn(){
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    sleep(200);
  }
  pixels.clear();
  pixels.show();
}

void LedWaterOut(){
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 255));
    pixels.show();
    sleep(200);
  }
  pixels.clear();
  pixels.show();
}

void Speak(int time){
  for(int i = 0; i < time; i++){
    digitalWrite(SPEAKER, HIGH);
    sleep(1000);
    digitalWrite(SPEAKER, LOW);
  }
}

void ServoRun(int speed){
  for(pos = 0; pos < 180; pos++){
    myservo.write(pos);
    sleep(speed);
  }
  for(pos = 180; pos > 0; pos--){
    myservo.write(pos);
    sleep(speed); 
  }  
}

void ClearCommand(){
  command = "";
}
