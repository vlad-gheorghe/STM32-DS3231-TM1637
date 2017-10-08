//PA0, PA1 --4,7 KOHM LA Vcc

#include <Wire.h>
#include "TM1637.h" 
#include <DHT.h> 
#define DHTPIN PA8     // what pin we're connected the DHT output
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE); 
#define CLK PC13         
#define DIO PC14 
#define brightness 7 

 int h;  //REGLAJ ORA
 int m;  //REGLAJ MINUTE
 //int thh; //afisare temp-hum
TM1637 tm1637(CLK,DIO);  
#define DS3231_I2C_ADDRESS 0x68
  
volatile boolean flag;
 unsigned char ClockPoint = 1;
byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}
 
byte bcdToDec(byte val){
  return ( (val/16*10) + (val%16) );
}
 
void setDateDs3231(byte second,  
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS3231_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));     
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}
 
void getDateDs3231(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{
 
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
 
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
 
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f); 
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}
 
void setINT(){  
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E);
  Wire.write(0x0);
  Wire.endTransmission();
}
 
//byte mm = 0;
//byte hh = 0;
 
void setup() {

  Wire.begin();

  tm1637.init();
  tm1637.set(brightness);  
 dht.begin();  
}

void loop(){
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 
  getDateDs3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 
  int8_t TimeDisp[4]; 
 // hh = hour;
  //mm = minute;
 // timp = hh*100+mm;
  for (int pp=20; pp>0; pp--)
  {
  if ((hour/10)==0) TimeDisp[0] = 17;
  else
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
 ClockPoint = (~ClockPoint) & 0x01;
 if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); delay(500);
  
  h = analogRead(PA0);   // REGLAJ ORA
  m = analogRead(PA1);   //REGLAJ MINUTE
// thh = analogRead(PB0);
 if (h>=0 && h<=50){
   second = 0;     
      hour++;  
      if (hour > 23) hour = 0;  
      setDateDs3231(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
      delay(200);
  }
  if (m>=0 && m<=50){
      second = 0;
      minute++;
      if (minute > 59) minute = 0;
      setDateDs3231(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
      delay(200);
  }
  tm1637.display(TimeDisp);}
  th();
 //if (thh>=0 && thh<=50){
//th();}

}

void th(){
  
   int hum = dht.readHumidity();
   int temp = dht.readTemperature();  
   tm1637.display(0,17);  // empty
    tm1637.display(1,17);  // empty
    tm1637.display(2,17);  // empty
    tm1637.display(3,17);  // empty
    tm1637.point(POINT_OFF);
    
   tm1637.display (0,temp/10);
  tm1637.display (1,temp % 10);
  tm1637.display(2,18);
  tm1637.display (3,12);
  delay(2000);
  tm1637.display(0,hum/10); 
    tm1637.display(1,hum % 10);
    tm1637.display(2,19);  // r
    tm1637.display(3,20);  // h
    delay(2000);
    tm1637.display(0,17);  // empty
    tm1637.display(1,17);  // empty
    tm1637.display(2,17);  // empty
    tm1637.display(3,17);  // empty
}
/*void clk(){
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 
  getDateDs3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 
  int8_t TimeDisp[4]; 
  if ((hour/10)==0) TimeDisp[0] = 17;
  else
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
 ClockPoint = (~ClockPoint) & 0x01;
 if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); delay(500);
  
  h = analogRead(PA0);   // REGLAJ ORA
  m = analogRead(PA1);   //REGLAJ MINUTE
 
 if (h>=0 && h<=50){
   second = 0;     
      hour++;  
      if (hour > 23) hour = 0;  
      setDateDs3231(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
      delay(200);
  }
  if (m>=0 && m<=50){
      second = 0;
      minute++;
      if (minute > 59) minute = 0;
      setDateDs3231(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
      delay(200);
  }
  tm1637.display(TimeDisp);
}
*/
