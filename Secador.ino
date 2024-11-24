#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buzzerPin = 5;
const int ledsUv = 4;

bool ledsPower = false;
unsigned long tiempoStamp_uv = 0;
unsigned long tiempoStamp_lcd = 0;
unsigned long workTime = 600000;
int incomingByte = 0;

void setup()
{
  tiempoStamp_uv = millis();
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(3, 0);
  lcd.print("Secador UV");
  lcd.setCursor(0, 1);
  lcd.print("Conect Bluetooth");

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledsUv, OUTPUT);
}

void loop()
{
  if (Serial.available() > 0) {
    // String text = Serial.readString();
    String text = Serial.readStringUntil('\n');
    int numerito = text.toInt();
    Serial.println(numerito);
    incomingByte = Serial.read() - 48;  // Convertir el byte ASCII a número
    // Serial.println(incomingByte);
    switch (numerito) {
      case 1:
        workTime = 60000;  // 1 minuto
        ledsPower = true;
        tiempoStamp_uv = millis();

        break;
      case 2:
        workTime = 120000;  // 2 minutos
        ledsPower = true;
        tiempoStamp_uv = millis();
        break;
      case 3:
        workTime = 300000;  // 5 minutos
        ledsPower = true;
        tiempoStamp_uv = millis();
        break;
      case 0:
        ledsPower = false;
        break;
      default:
        workTime = incomingByte * 1000;  
        ledsPower = true;
        tiempoStamp_uv = millis();
        break;
    }
  }

  if (ledsPower)
  {
    digitalWrite(ledsUv, HIGH);

    unsigned long elapsedTime = millis() - tiempoStamp_uv;

    if (elapsedTime >= workTime)
    {
      ledsPower = false;
      digitalWrite(ledsUv, LOW);
 
      buzzerBeep();
    }

    if ((millis() - tiempoStamp_lcd) >= 1000)
    {
      charge(elapsedTime);
      tiempoStamp_lcd = millis();
    }
  }
}

void charge(unsigned long elapsedTime)
{
  int falta = (workTime - elapsedTime) / 1000;
  int numHashes = (elapsedTime * 16) / workTime;

  lcd.setCursor(3, 0);
  lcd.print("Faltan:");
  lcd.setCursor(12, 0);
  lcd.print(falta);

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++)
  {
    if (i < numHashes)
      lcd.print("#");
    else
      lcd.print("/");
  }
}

void buzzerBeep()
{
  
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
   delay(500);
}