#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);
float Lectura_Actual;
float Temperatura;

void setup() {

  Serial.begin(9600);

  
  Wire.begin();

  delay(250);

  
  if (!display.begin(i2c_Address, true)) {
    Serial.println("ERROR: OLED no encontrada");

    while (1);
  }

  Serial.println("OLED funcionando correctamente");

}

void loop() {
  Lectura_Actual = analogRead(A3);
  Temperatura=(Lectura_Actual*(5.0/1024) *100);

  display.clearDisplay();
  
  display.setTextColor(SH110X_INVERSE);
  display.setTextSize(2);
  display.setCursor(15, 10);
  display.println("wenas!");

  display.setTextSize(1);
  display.setCursor(15, 40);
  display.print(Temperatura);
  display.print(" C");
  display.display();

  delay(1000);
  
  
  
}
