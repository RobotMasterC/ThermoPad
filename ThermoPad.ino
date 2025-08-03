#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TEMP 0x44
#define OLED 0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

bool showTemp = true;
bool lastState = LOW;
float Temp, Humid, tempHigh, tempLow, humidHigh, humidLow;
int mode;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() {
  pinMode(D1, INPUT);
  Serial.begin(9600);
  Wire.begin();
  display.clearDisplay();
  display.setTextSize(1);
  delay(100);
}


void loop() {
  readTempHumid();

  bool buttonState = digitalRead(D1);

  if (lastState == LOW && buttonState == HIGH) {
    mode = (mode+1) % 3;
  }
  lastState = buttonState;

  readTempHumid();

  if (mode == 0) {
    displayTemp();
  } else if (mode == 1) {
    displayHumid();
  } else if (mode == 2) {
    displayHighLow();
  }


  delay(200);
}

void readTempHumid() {
  int16_t rawTemp, rawHumid;
  float tempHigh = -1000, tempLow = 1000;
  float humidHigh = -1000, humidLow = 1000;
  int mode = 0;
  Wire.beginTransmission(TEMP);
  Wire.write(0x2C);
  Wire.write(0x06);
  Wire.endTransmission(false);

  Wire.requestFrom(TEMP, 6);

  rawTemp = (Wire.read() << 8) | Wire.read();
  Wire.read();
  rawHumid = (Wire.read() << 8) | Wire.read();
  Wire.read();

  Temp = -45 + 175 * ((float)rawTemp / 65535.0);
  Humid = 100 * ((float)rawHumid / 65535.0);

  if (Temp > tempHigh) tempHigh = Temp;
  if (Temp < tempLow)  tempLow = Temp;

  if (Humid > humidHigh) humidHigh = Humid;
  if (Humid < humidLow)  humidLow = Humid;

}

void displayTemp() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(Temp, 2);
  display.print(" C");

  if (Humid <= 30) {
    display.setCursor(0, 40);
    display.print("Bloody Nose Possible");
  }
  display.display();
}



void displayHumid() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Humidity: ");
  display.print(Humid, 2);
  display.print(" %");

  if (Humid <= 30) {
    display.setCursor(0, 40);
    display.print("Bloody Nose Possible");
  }
  display.display();
}

void displayHighLow() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(tempLow, 1);
  display.print(" - ");
  display.print(tempHigh, 1);
  display.setCursor(0, 16);
  display.print("Humid: ");
  display.print(humidLow, 1);
  display.print(" - ");
  display.print(humidHigh, 1);
  if (Humid <= 30) {
    display.setCursor(0, 40);
    display.print("Bloody Nose Possible");
  }
  display.display();
}
