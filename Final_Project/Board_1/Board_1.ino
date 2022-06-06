#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Servo.h>

#define LCD_R5 13
#define LCD_E 12
#define LCD_DB4 11
#define LCD_DB5 10
#define LCD_DB6 9
#define LCD_DB7 8
#define PIN 7
#define NUMPIXELS 8
#define SPEAKER 6
#define SERVOR_PIN 5
#define WINDOW_SENSOR 4
#define RESET_PIN 3
#define SENSOR A0

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Servo myservo;

String command = "";

// ======== Chế độ giặt =========
// 1 : Giặt mạnh
// 2 : Giặt nhẹ
// default : Giặt mạnh
int mode_wash = 1;
// ==============================

// ======== Chế mức nước =========
// 1 : Nước cao
// 2 : Nước trung bình
// 3 : Nước thấp
// default : Nước trung bình
int level_water = 2;
// ==============================

// ======= Cảm biến nước ========
const int water_level_1 = 300;
const int water_level_2 = 600;
const int water_level_3 = 900;
const int water_level_drain = 50;
int water_sensor = 0;
// ==============================

// ========= Trạng thái =========
// 0 : Đang chờ
// 1 : Đang cấp nước
// 2 : Đang giặt lần 1
// 3 : Đang giặt lần 2
// 4 : Đang giặt lần 3
// 5 : Đang xả nước
// 6 : Đang vắt
// 7 : Hoàn thành
// 8 : Dừng
int status = 0;
// ==============================

// ========== Tạm dừng ==========
// 0 : Không
// 1 : Tạm dừng
int pause = 0;
// ==============================

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
  pinMode(WINDOW_SENSOR, INPUT);
  pinMode(SENSOR, INPUT);

  digitalWrite(RESET_PIN, HIGH);
  delay(200);
  pinMode(RESET_PIN, OUTPUT);  

  myservo.attach(SERVOR_PIN);
}

void loop()
{
  ReadInput();

  // Chọn chế độ
  if (command == "A")
  {
    SelectMode();
    ClearCommand();
  }

  // Chọn mức nước
  if (command == "B")
  {
    SelectWater();
    ClearCommand();
  }

  // Start / Stop
  if (command == "C")
  {
    if (status == 0 || status == 7)
    {
      Serial.println("Start washing");
      lcd_1.clear();
      lcd_1.print("Bat dau giat");
      StartWashing();
    }
    ClearCommand();
  }

  // Pause / Resume
  if (command == "D")
  {
    if (pause == 0)
    {
      pause = 1;
      Serial.println("Pause washing");
      lcd_1.clear();
      lcd_1.print("Tam dung");
    }
    else
    {
      pause = 0;
      Serial.println("Resume washing");
      lcd_1.clear();
      lcd_1.print("Tiep tuc");
    }
    ClearCommand();
  }
}

void StartWashing()
{
  Serial.println("Start washing");

  // bơm nước
  Serial.println("Bom nuoc");
  WaterSupply();

  // giặt
  Serial.println("Giat");
  Wash();

  // xả nước
  Serial.println("Xa nuoc");
  WaterDrain();

  // vắt
  Serial.println("Vat");
  Squeeze();

  // hoàn thành
  Serial.println("Hoan thanh");
  Complete();
}

void PauseWashing()
{
  pause = 1;
}

void ResumeWashing()
{
  pause = 0;
}

void StopWashing()
{
  status = 8;
  Serial.println("Stop washing");
  lcd_1.clear();
  lcd_1.print("Dung");
  pixels.clear();
  pixels.show();
  delay(500);
  digitalWrite(RESET_PIN, LOW);
}

void WaterSupply()
{
  status = 1;
  lcd_1.clear();
  lcd_1.print("Dang bom nuoc");
  int water_level = level_water == 1 ? water_level_1 : (level_water == 2 ? water_level_2 : water_level_3);
  while (water_sensor < water_level)
  {
    LedWaterIn(water_level);
  }
  lcd_1.clear();
  lcd_1.print("Bom nuoc xong");
  delay(500);
}

void Wash()
{
  status = status == 1 ? 2 : status;
  for (int i = status; i <= 4; i++)
  {
    status = i;
    lcd_1.clear();
    lcd_1.print("Dang giat");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Lan " + String(status - 1));
    for (int i = 0; i < 3; i++)
    {
      ServoRun(3);
    }
  }
}

void WaterDrain()
{
  status = 5;
  lcd_1.clear();
  lcd_1.print("Dang xa nuoc");
  while (water_sensor > water_level_drain)
  {
    LedWaterOut();
  }
}

void Squeeze()
{
  status = 6;
  lcd_1.clear();
  lcd_1.print("Dang vat");
  for (int i = 0; i < 5; i++)
  {
    ServoRun(2);
  }
  lcd_1.clear();
  lcd_1.print("Vat xong");
}

void Complete()
{
  status = 7;
  lcd_1.clear();
  lcd_1.print("Hoan thanh");
  delay(1000);
}

void SelectMode()
{
  Serial.println("Select Mode");
  while (command != "#")
  {
    if (command == "1")
    {
      mode_wash = 1;
    }
    if (command == "2")
    {
      mode_wash = 2;
    }
    lcd_1.clear();
    lcd_1.print("Select Mode: ");
    lcd_1.setCursor(0, 1);
    lcd_1.print(mode_wash == 1 ? "Giat manh" : "Giat nhe");
    Sleep(100);
  }
  lcd_1.clear();
  lcd_1.print("Selected Mode: ");
  lcd_1.setCursor(0, 1);
  lcd_1.print(mode_wash == 1 ? "Giat manh" : "Giat nhe");
}

void SelectWater()
{
  Serial.println("Select Water");
  while (command != "#")
  {
    if (command == "1")
    {
      level_water = 1;
    }
    if (command == "2")
    {
      level_water = 2;
    }
    if (command == "3")
    {
      level_water = 3;
    }
    lcd_1.clear();
    lcd_1.print("Select Water: ");
    lcd_1.setCursor(0, 1);
    lcd_1.print(level_water == 1 ? "Nuoc cao" : level_water == 2 ? "Nuoc trung binh"
                                                                 : "Nuoc thap");
    Sleep(100);
  }
  lcd_1.clear();
  lcd_1.print("Selected Water: ");
  lcd_1.setCursor(0, 1);
  lcd_1.print(level_water == 1 ? "Nuoc cao" : level_water == 2 ? "Nuoc trung binh"
                                                               : "Nuoc thap");
}

void Sleep(int s)
{
  for (int i = 1; i < s; i++)
  {
    delay(1);
    ReadInput();
    if (status == 1 || status == 5)
    {
      ReadSensor();
    }
  }
}

void ReadInput()
{
  if (Serial.available())
  {
    command = Serial.readStringUntil('\r\n');
    command.trim();
    Serial.println(command);
    Beep();
    if (command == "A" && !(status == 0 || status == 7))
    {
      StopWashing();
    }
  }
}

void ReadSensor()
{
  water_sensor = analogRead(SENSOR);
}

void LedWaterIn(int water_level)
{
  pixels.clear();
  for (int i = 0; (i < NUMPIXELS && water_sensor < water_level); i++)
  {
    Serial.println("Water: " + String(water_sensor));
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    Sleep(100);
  }
  pixels.clear();
  pixels.show();
}

void LedWaterOut()
{
  pixels.clear();
  for (int i = 0; (i < NUMPIXELS && water_sensor > water_level_drain); i++)
  {
    Serial.println("Water: " + String(water_sensor));
    pixels.setPixelColor(i, pixels.Color(0, 255, 255));
    pixels.show();
    Sleep(100);
  }
  pixels.clear();
  pixels.show();
}

void Alarm(int time)
{
  for (int i = 0; i < time; i++)
  {
    digitalWrite(SPEAKER, HIGH);
    delay(1000);
    digitalWrite(SPEAKER, LOW);
  }
}

void Beep()
{
  digitalWrite(SPEAKER, HIGH);
  delay(100);
  digitalWrite(SPEAKER, LOW);
}

void ServoRun(int speed)
{
  for (pos = 0; pos < 180; pos++)
  {
    myservo.write(pos);
    Sleep(speed);
  }
  for (pos = 180; pos > 0; pos--)
  {
    myservo.write(pos);
    Sleep(speed);
  }
}

void ClearCommand()
{
  command = "";
}
