#include <LiquidCrystal_I2C.h>//monitor 

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  int value = analogRead(A0);
  float resistance = Resistance(Voltage(value));

  // Tiefpass
  int temp = 0;

  for (int i = 0; i<50; i++)
  {
    temp += Measure();
  }
  
  Serial.println(Measure());
  lcd.print("Lux: "+(String)(temp/50.0));
  
  lcd.setCursor(0,1);
  lcd.print("R: "+(String)resistance+" Ohm");
  delay(500);
}

float Measure()
{
  int value = analogRead(A0);
  float resistance = Resistance(Voltage(value));
  
  if (f1(resistance) < 400.0)
  {
    return f1(resistance);
  }
  else
  {
    return f2(resistance);
  }
}

float Resistance(float voltage)
{
  return (5.0*9800.0/voltage)-9800.0;
}

float Voltage(int value)
{
  return value/1023.0*5.0;
}

float f1(float x)
{
  return pow(x/56889.5860605443, 1.0/-0.732075748353836);
}

float f2(float x)
{
  return pow(x/11864.12303184592,1.0/-0.478173839323303);
}
