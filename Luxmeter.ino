#include <LiquidCrystal.h>
#include <Wire.h>

#define RESISTOR 4700.0
#define SENSOR A1

#define RIGHT 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define SELECT 4
#define NONE 5

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

long cycles = 0;

float UIN = 0.0;
float UOUT = 0.0;
float RESISTANCE = 0.0;
float LUX = 0.0;
float MAX = 0.0;
float MIN = 0.0;
long nextPrint;

int DIV = 1;
const float DIVISORS[5] = {2.0, 4.0, 8.0, 16.0, 32.0};

int Mode = 0; // 0: Standard; 1: Min/Max; 2: Divisoreinstellung

byte omega[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000
};

byte ArrowChar[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

void Measure()
{
  UIN = analogRead(SENSOR);
  UOUT = UOUT + (UIN - UOUT) / DIVISORS[DIV]; // Tiefpass
  RESISTANCE = Resistance(Voltage(UOUT));
  LUX = f(RESISTANCE);

  // Maximums- und Minumumswerte schreiben, wenn mehr als 100 Cycles durchgegangen sind
  if (cycles > 100)
  {
    if (LUX > MAX) { MAX = LUX; }
    else if (LUX < MIN) { MIN = LUX; }
  }
  
  else
  {
    MIN = LUX;
  }

  cycles++;
}

void StandardMode()
{
  lcd.setCursor(0,0);
  lcd.print("Lux: "+(String)(LUX) + " lx    ");
  
  lcd.setCursor(0,1);
  lcd.print("R: "+(String)(RESISTANCE));
  
  lcd.createChar(1, omega);
  lcd.setCursor(11,1);
  lcd.write(1);
}

void MinMax()
{
  lcd.setCursor(0,0);
  lcd.print("Max: "+(String)(MAX) + " lx    ");
  
  lcd.setCursor(0,1);
  lcd.print("Min: "+(String)(MIN) + " lx    ");
}

void SelectDivisor()
{
  lcd.setCursor(2,0);
  lcd.print("2 4 8 16 32");

  switch (Buttons())
  {
    case LEFT:
    {
      int currentArrowPos = ArrowPosition(DIV);
      PreviousDivisor();
      while (Buttons() == LEFT) {}
      lcd.setCursor(currentArrowPos, 1);
      lcd.print(" ");
      break;
    }

    case RIGHT:
    {
      int currentArrowPos = ArrowPosition(DIV);
      NextDivisor();
      while (Buttons() == RIGHT) {}
      lcd.setCursor(currentArrowPos, 1);
      lcd.print(" ");
      break;
    }
  }
  
  lcd.createChar(1, ArrowChar);
  lcd.setCursor(ArrowPosition(DIV), 1);
  lcd.write(1);
  
}

int ArrowPosition(int index)
{
  switch (index)
  {
    case 0:
    {
      return 2;
      break;
    }
    
    case 1:
    {
      return 4;
      break;
    }
    
    case 2:
    {
      return 6;
      break;
    }

    case 3:
    {
      return 8;
      break;
    }

    case 4:
    {
      return 11;
      break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SENSOR, INPUT);
  pinMode(A0, INPUT);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  nextPrint = millis() + 100;

  // Taster abfragen
  
  switch (Buttons())
  {
    case UP:
    {
      PreviousMode();
      while (Buttons() == UP) {}
      lcd.clear();
      break;
    }

    case DOWN:
    {
      NextMode();
      while (Buttons() == DOWN) {}
      lcd.clear();
      break;
    }
  }


  switch (Mode)
  {
    case 0:
    {
      StandardMode();
      break;
    }

    case 1:
    {
      MinMax();
      break;
    }

    case 2:
    {
      SelectDivisor();
      break;
    }
  }
  
  // Werte auslesen
  while (millis() < nextPrint)
  {
    Measure();
  }
}

int Filter(int value)
{
  return UOUT + (UIN - UOUT) / DIV;
}

float Resistance(float voltage)
{
  return (5.0*RESISTOR/(5.0-voltage))-RESISTOR;
}

float Voltage(int value)
{
  return value/1023.0*5.0;
}

float f(float x)
{
  return 2.0 * pow((x-70.0)/33993.95062, 1.0/-0.6535);
}

void NextMode()
{
  if (Mode == 2)
  {
    Mode = 0;
  }
  else
  {
    Mode++;
  }
}

void PreviousMode()
{
  if (Mode == 0)
  {
    Mode = 2;
  }
  
  else
  {
    Mode--;
  }
}

void NextDivisor()
{
  if (DIV == 4)
  {
    DIV = 0;
  }
  else
  {
    DIV++;
  }
}

void PreviousDivisor()
{
  if (DIV == 0)
  {
    DIV = 4;
  }
  
  else
  {
    DIV--;
  }
}


int Buttons()
{
  int value = analogRead(A0); // Auslesen der Taster am Analogen Pin A0.
  if (value > 1000) return NONE;
  if (value < 50) return RIGHT;
  if (value < 195) return UP;
  if (value < 380) return DOWN;
  if (value < 555) return LEFT;
  if (value < 680) return SELECT;
  if (value < 790) return RIGHT;
   
  return NONE; // Ausgabe wenn kein Taster gedrückt wurde.
}
