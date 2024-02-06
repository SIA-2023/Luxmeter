#include <LiquidCrystal_I2C.h>//monitor 

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

void setup() {
  // Setting mode of Input Pin, initializing LCD screen and starting serial interface
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

  // Lowpass
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

  // Use first function if luminosity (measured by first function) is below 400 lx
  if (f1(resistance) < 400.0)
  {
    return f1(resistance);
  }
  // If it's above, use the second
  else
  {
    return f2(resistance);
  }
}

// Calculate resistance (R1 = 10 kOhm)
float Resistance(float voltage)
{
  return (5.0*9800.0/voltage)-9800.0;
}

// Calculate voltage
float Voltage(int value)
{
  return value/1023.0*5.0;
}

// First function (I=[0;400])
float f1(float x)
{
  return pow(x/56889.5860605443, 1.0/-0.732075748353836);
}

// Second function (I=[400;10000])
float f2(float x)
{
  return pow(x/11864.12303184592,1.0/-0.478173839323303);
}
