/* Color Sensor Connection:
 * VCC        5V
 * GND        GND
 * OUT        A0
 * RED        8
 * GREEN      9
 * BLU        10
 * 
 * Load Sensor:
 * Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND 

 servo1 #3
 servo2 #4
*/

#include <HX711.h>
#include <Wire.h>
#include <Servo.h>

#define RED 8
#define GREEN 9
#define BLUE 10
#define SENSOROUTPUT A0
#define SAMPLE_NUM 50    
#define MILISEC 10      
#define REST 0     
double value_red = 0.0, value_green = 0.0, value_blue = 0.0, value = 0.0;
float red_factor = 1.0, green_factor = 0, blue_factor = 1.0;

HX711 scale(5, 6);

float calibration_factor = 2230; 
float units;
float ounces;
int pro_clr=0, r=0, b=0;
Servo myservo,upservo;
int i,pos = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare(); 

  long zero_factor = scale.read_average(); 
  Serial.print("Zero factor: ");
  Serial.println(zero_factor);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(SENSOROUTPUT, INPUT);
  for(i=0;i<5;i++)
  {
    myservo.write(0);
    delay(15);
    upservo.write(0);
    delay(15);
    }
  myservo.attach(3);
  upservo.attach(4);
  Wire.begin();
}


void loop()
{
  scale.set_scale(calibration_factor);
  Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0) {units = 0.00;}
  ounces = units * 0.035274;
  Serial.print(units);
  Serial.print(" grams"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
  delay(500);

  if(units>=23 && units<=42)
  {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  value_red = red_factor * color_intensity(RED, SAMPLE_NUM, MILISEC);
  Serial.print("Red= "); Serial.print(value_red,0);
  value_green = green_factor * color_intensity(GREEN, SAMPLE_NUM, MILISEC);
  Serial.print("     Green= "); Serial.print(value_green,0);
  value_blue = blue_factor * color_intensity(BLUE, SAMPLE_NUM, MILISEC);
  Serial.print("     Blue= "); Serial.print(value_blue,0); Serial.print("\t");
  sort(value_red, value_green, value_blue);
  Wire.beginTransmission(9); 
  Wire.write(pro_clr);
  Wire.write(r);
  Wire.write(b);
  Wire.endTransmission();
  delay(500);
  
  for (pos = 0; pos <= 180; pos += 1) {myservo.write(pos); delay(15);}
  delay(1500);
  for (pos = 180; pos >= 0; pos -= 1) {myservo.write(pos); delay(15);}
  }

  else if(units>=5 && units<=22)
  {
    for (pos = 0; pos <= 85; pos += 1) {upservo.write(pos); delay(15);}
    for (pos = 85; pos >= 0; pos -= 1) {upservo.write(pos); delay(10);}
    
  }
  
  delay(500);
  
  
}



double color_intensity(char colorPin, int sample_num, int milisec)
{
  double value = 0.0;
  digitalWrite(colorPin, LOW);  //updated "value_green" global variable
  delay(50);
  for (int i = 1; i <= sample_num; i++)
  {
    value += analogRead(SENSOROUTPUT);
    delay(milisec);
  }
  value = value / (double) sample_num;
  digitalWrite(colorPin, HIGH);
  return value;
}


void sort(int value_red, int value_green, int value_blue)
{
  if (value_red > value_green)
  {
    if (value_red > value_blue) {Serial.println("RED."); pro_clr=1;r++;}
    else {Serial.println("BLUE.");pro_clr=2;b++;} //value_red < value_blue ie. value_blue > value_red > value_green
  }
  else    //ie. value_green > vlaue_red
  {
    if (value_green > value_blue) {Serial.println("GREEN.");pro_clr=0;}
    else {Serial.println("BLUE.");pro_clr=2;b++;} //value_green < value_blue ie. value_blue > value_green > value_red
  }
}
