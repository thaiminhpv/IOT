int rows[4] = {13, 12, 11, 10};
int cols[4] = {9, 8, 7, 6};
char matrix[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

int temp;
char outChar, _outChar;
bool isPressed = false;

void ifButtonPressed(void (*callback)(char));

void setup()
{
  for (int _: rows) pinMode(_, OUTPUT);
  for (int _: cols) pinMode(_, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop()
{
    ifButtonPressed([](char charPressed) {
        // This will be sent to the serial monitor and Board_1
        Serial.print(charPressed);
    });
}

void ifButtonPressed(void (*callback)(char))
{
    _outChar = scanMatrix();
    if (_outChar != '\0') {
        isPressed = true;
        outChar = _outChar;
    } else if (_outChar == '\0' && isPressed) {
        isPressed = false;
        callback(outChar);
    }
}

char scanMatrix() {
  for (int i = 0; i < 4; i++) {
    writeRows(i);
    temp = readCols();
    if (temp != -1) {
      // x = temp;
      // y = i;
      return matrix[i][temp];
    }
  }
  return '\0'; // buttons are not pressed
}

void writeRows(int index) {
  for (int i = 0, counter = 0; i < 4; i++, counter++) digitalWrite(rows[i], counter == index ? LOW : HIGH);
}
                 
int readCols() {
  for(int i = 0; i < 4; i++) if (digitalRead(cols[i]) == 0) return i;
  return -1;
}
