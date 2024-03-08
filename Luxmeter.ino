#include <LiquidCrystal_I2C.h>//monitor 
#include <LiquidCrystal.h>
#include <Wire.h>

#define RESISTOR 9800.0
#define SENSOR A2

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  int value = analogRead(SENSOR);
  float resistance = Resistance(Voltage(value));

  // Tiefpass
  int temp = 0;

  for (int i = 0; i<50; i++)
  {
    temp += Measure();
    delay(10);
  }
  
  Serial.println(Measure());
  lcd.print("Lux: "+(String)(temp/50.0));
  
  lcd.setCursor(0,1);
  lcd.print("R: "+(String)resistance+" Ohm");
  delay(500);
}

float Measure()
{
  int value = analogRead(SENSOR);
  float resistance = Resistance(Voltage(value));
  
  if (resistance < 419.0)
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
  return (5.0*RESISTOR/voltage)-RESISTOR;
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
