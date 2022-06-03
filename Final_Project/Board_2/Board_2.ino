
#define col1   9
#define col2   8
#define col3   7
#define col4   6
#define r1     13
#define r2     12
#define r3     11
#define r4     10

void setup()
{
  Serial.begin(9600);
  
  pinMode(col1, INPUT_PULLUP);
  pinMode(col2, INPUT_PULLUP);
  pinMode(col3, INPUT_PULLUP);
  pinMode(col4, INPUT_PULLUP);
  
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
}

void loop()
{
  row1();
  row2();
  row3();
  row4();
}

void row1() {
  digitalWrite(r1, LOW);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);
  
  if (digitalRead(col1) == LOW) {
    Serial.println("1"); // Serial print 1
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col2) == LOW) {
    Serial.println("2"); // Serial print 2
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col3) == LOW) {
    Serial.println("3"); // Serial print 3
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col4) == LOW) {
    Serial.println("A"); // Serial print A
    delay(100); // Delay of 1 sec
  }
}

void row2() {
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);
  
  if (digitalRead(col1) == LOW) {
    Serial.println("4");// Serial print 4
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col2) == LOW) {
    Serial.println("5"); // Serial print 5
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col3) == LOW) {
    Serial.println("6"); // Serial print 6
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col4) == LOW) {
    Serial.println("B"); // Serial print B
    delay(100); // Delay of 1 sec
  }
}

void row3(){
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, LOW);
  digitalWrite(r4, HIGH);
  
  if (digitalRead(col1) == LOW) {
    Serial.println("7"); // Serial print 7
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col2) == LOW) {
    Serial.println("8"); // Serial print 8
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col3) == LOW) {
    Serial.println("9"); // Serial print 9
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col4) == LOW) {
    Serial.println("C"); // Serial print C
    delay(100); // Delay of 1 sec
  }
}

void row4(){
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, LOW);
  
  if (digitalRead(col1) == LOW) {
    Serial.println("*"); // Serial print *
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col2) == LOW) {
    Serial.println("0"); // Serial print 0
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col3) == LOW) {
    Serial.println("#"); // Serial print #
    delay(100); // Delay of 1 sec
  }
  else if (digitalRead(col4) == LOW) {
    Serial.println("D"); // Serial print D
    delay(100); // Delay of 1 sec
  }
}
