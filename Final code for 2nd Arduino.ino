/*
servo #3
*/
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd1(12, 5, 7, 8, 9, 10);
LiquidCrystal lcd2(12, 4, 7, 8, 9, 10);
Servo myservo;
int c,pro_clr=0,pos = 0,i,r=0,b=0;
char string[2],str[2], stb[2];
void setup() 
{
  Serial.begin(9600);
myservo.attach(3);
for(i=0;i<5;i++)
  {
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    delay(15);
    }
Wire.begin(9); 
Wire.onReceive(receiveEvent);
lcd1.begin(16, 2);
lcd2.begin(16, 2);
}

void receiveEvent(int bytes) {
  pro_clr = Wire.read();
  r = Wire.read();
  b = Wire.read();
  
}


void loop() 
{
  lcd1.setCursor(0, 0);
  lcd1.print("TOTAL: ");
  c=r+b;
  sprintf(string, "%d", c);
  lcd1.print(string);
  lcd1.setCursor(0, 1);
  sprintf(str, "%d", r);
  sprintf(stb, "%d", b);
  lcd1.print("RED:");lcd1.print(str);
  lcd1.setCursor(8, 1);
  lcd1.print("BLUE:");lcd1.print(stb);
  delay(2000);
  lcd2.setCursor(0, 0);
  lcd2.print("   MONITORING");
  lcd2.setCursor(0, 1);
  lcd2.print("     SYSTEM");
  if(pro_clr == 1)
    {
      myservo.write(0); delay(15);
      }
  else if(pro_clr == 2)
    {
      myservo.write(60); delay(15);
      }

}
