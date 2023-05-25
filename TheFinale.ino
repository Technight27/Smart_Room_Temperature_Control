#include <SoftwareSerial.h>
String apiKey = "P6WZMDXX3TCFX4BI";
#define PHOTO_PIN A1
#define RAIN_PIN 6

#define in1 11
#define in2 12
#define en1 13

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE    DHT22
DHT dht(DHTPIN, DHTTYPE);
#define trigPin 9
#define echoPin 10
long duration;
int distance; 
SoftwareSerial ser(2, 3); // RX, TX

float ExtremeCold[4] = {5,5,5,10};
float Cold[4] = {5,10,10,15};
float Mild[4] = {10,15,15,20};
float Ideal[4] = {15,20,25,30};
float Warm[4] = {25,30,35,40};
float Hot[4] = {35,40,45,50};
float ExtremeHot[4] = {40,45,50,50};

float ExtremeDry[4] = {0,0,20,40};
float Dry[4] = {20,40,40,60};
float Normal[4] = {40,60,60,80};
float Humid[4] = {60,80,80,100};
float VeryHumid[4] = {80,100,120,120};

float Level0[4] = {0,0,0,0};
float Level1[4] = {100,125.83,125.83,151.66};
float Level2[4] = {125.83,151.66,151.66,177.49};
float Level3[4] = {151.66,177.49,177.49,203.32};
float Level4[4] = {177.49,203.32,203.32,229.15};
float Level5[4] = {203.32,229.15,229.15,255};
float Level6[4] = {229.15,255,255,280.85};

void setup()
{
  pinMode(PHOTO_PIN, INPUT);
   ser.begin(9600);
    Serial.begin(9600);  
    unsigned char check_connection=0;
    unsigned char times_check=0;
    Serial.println("Connecting to Wifi");
  while(check_connection==0)
  {
   Serial.print("..");
   ser.print("AT+CWJAP=\"Arka maikup\",\"Akr126$74B\"\r\n");
   ser.setTimeout(5000);
 if(ser.find("WIFI CONNECTED\r\n")==1 )
 {
 Serial.println("WIFI CONNECTED");
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
  Serial.println("Trying to Reconnect..");
  }
  }
  delay(5000);
  dht.begin();
}

float compute(float val, float arr[100])
{
  float u;
  float slope1 = 1/(arr[1]-arr[0]);
  float slope2 = 1/(arr[2]-arr[3]);

  if ((val < arr[1]) && (arr[0]!=arr[1]))
    u = slope1*(val - arr[0]);
  else if ((val > arr[2]) && (arr[2]!=arr[3]))
    u = slope2*(val - arr[3]);
  else
    u = 1.00;
  return u;
}

void loop()
{

  float u1, u2, Area, num = 0.0, den = 0.0, vlevel;

  float tem = dht.readTemperature();
  float hum = dht.readHumidity();
  int rain = digitalRead(RAIN_PIN);
  int sun = analogRead(PHOTO_PIN);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
 distance = duration * 0.034 / 2;
  
  
  Serial.print("\n----------------------WEATHER UPDATE----------------------");

  Serial.print("\nTemperature : ");
  Serial.print(tem);
  Serial.print("°C");

  Serial.print("\nHumidity : ");
  Serial.print(hum);
  Serial.print("%");

  if(rain == 0)
    Serial.print("\nRain Detected");
  else
    Serial.print("\nNo Rain Detected");

  if(sun <= 500)
    Serial.print("\nSunlight Detected");
  else
    Serial.print("\nNo Sunlight Detected");

  Serial.print("\n------------------------------------------------------------");  

  Serial.print("\nPerson detected at distance : ");
  Serial.print(distance);
  Serial.print("cm");

  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


 if (distance < 100)
 {
    
    if ((tem >= 5 && tem <= 15) && (hum >= 60 && hum <= 100))
    {
      u1 = compute(tem,Cold);
      u2 = compute(hum,Humid);
      if (u1>=u2)
        Area = 0.5*(Level1[3]-Level1[0])*u1*(2-u1);
      else
        Area = 0.5*(Level1[3]-Level1[0])*u2*(2-u2);

    num = num + Area*(Level1[3]+Level1[0])/2;
    den = den + Area;
    }
    
    if ((tem >= 5 && tem <= 15) && (hum >= 80))
    {
      u1 = compute(tem,Cold);
      u2 = compute(hum,VeryHumid);
      if (u1<=u2)
        Area = 0.5*(Level1[3]-Level1[0])*u1*(2-u1);
      else
        Area = 0.5*(Level1[3]-Level1[0])*u2*(2-u2);

    num = num + Area*(Level1[3]+Level1[0])/2;
    den = den + Area;
    }

    if ((tem >= 10 && tem <= 20) && (hum <= 40))
    {
      u1 = compute(tem,Mild);
      u2 = compute(hum,ExtremeDry);
      if (u1>=u2)
        Area = 0.5*(Level1[3]-Level1[0])*u1*(2-u1);
      else
        Area = 0.5*(Level1[3]-Level1[0])*u2*(2-u2);

      num = num + Area*(Level1[3]+Level1[0])/2;
      den = den + Area;
    }


    if ((tem >= 10 && tem <= 20) && (hum >= 20 && hum <= 60))
    {
      u1 = compute(tem,Mild);
      u2 = compute(hum,Dry);
      if (u1>=u2)
        Area = 0.5*(Level1[3]-Level1[0])*u1*(2-u1);
      else
        Area = 0.5*(Level1[3]-Level1[0])*u2*(2-u2);

      num = num + Area*(Level1[3]+Level1[0])/2;
      den = den + Area;
    }


    if ((tem >= 10 && tem <= 20) && (hum >= 40 && hum <= 80))
    {
      u1 = compute(tem,Mild);
      u2 = compute(hum,Normal);
      if (u1>=u2)
        Area = 0.5*(Level1[3]-Level1[0])*u1*(2-u1);
      else
        Area = 0.5*(Level1[3]-Level1[0])*u2*(2-u2);

      num = num + Area*(Level1[3]+Level1[0])/2;
      den = den + Area;

    
    }


    if ((tem >= 10 && tem <= 20) && (hum >= 60 && hum <= 100))
    {
      u1 = compute(tem,Mild);
      u2 = compute(hum,Humid);
      if (u1>=u2)
        Area = 0.5*(Level2[3]-Level2[0])*u1*(2-u1);
      else
        Area = 0.5*(Level2[3]-Level2[0])*u2*(2-u2);
    
    num = num + Area*(Level2[3]+Level2[0])/2;
    den = den + Area;

    
    }
    
    if ((tem >= 10 && tem <= 20) && (hum >= 80))
    {
      u1 = compute(tem,Mild);
      u2 = compute(hum,VeryHumid);
      if (u1<=u2)
        Area = 0.5*(Level2[3]-Level2[0])*u1*(2-u1);
      else
        Area = 0.5*(Level2[3]-Level2[0])*u2*(2-u2);

    num = num + Area*(Level2[3]+Level2[0])/2;
    den = den + Area;
    }

    if ((tem >= 10 && tem <= 20) && (hum <= 40))
    {
      u1 = compute(tem,Ideal);
      u2 = compute(hum,ExtremeDry);
      if (u1>=u2)
        Area = 0.5*(Level2[3]-Level2[0])*u1*(2-u1);
      else
        Area = 0.5*(Level2[3]-Level2[0])*u2*(2-u2);

      num = num + Area*(Level2[3]+Level2[0])/2;
      den = den + Area;
    }


    if ((tem >= 15 && tem <= 30) && (hum >= 20 && hum <= 60))
    {
      u1 = compute(tem,Ideal);
      u2 = compute(hum,Dry);
      if (u1>=u2)
        Area = 0.5*(Level3[3]-Level3[0])*u1*(2-u1);
      else
        Area = 0.5*(Level3[3]-Level3[0])*u2*(2-u2);

      num = num + Area*(Level2[3]+Level2[0])/2;
      den = den + Area;
    }


    if ((tem >= 15 && tem <= 30) && (hum >= 40 && hum <= 80))
    {
      u1 = compute(tem,Ideal);
      u2 = compute(hum,Normal);
      if (u1>=u2)
        Area = 0.5*(Level4[3]-Level4[0])*u1*(2-u1);
      else
        Area = 0.5*(Level4[3]-Level4[0])*u2*(2-u2);

    num = num + Area*(Level4[3]+Level4[0])/2;
    den = den + Area;
    }

    if ((tem >= 15 && tem <= 30) && (hum >= 60 && hum <= 100))
    {
      u1 = compute(tem,Ideal);
      u2 = compute(hum,Humid);
      if (u1<=u2)
        Area = 0.5*(Level5[3]-Level5[0])*u1*(2-u1);
      else
        Area = 0.5*(Level5[3]-Level5[0])*u2*(2-u2);

      num = num + Area*(Level5[3]+Level5[0])/2;
      den = den + Area;
    }
    
    if ((tem >= 15 && tem <= 30) && (hum >= 80))
    {
      u1 = compute(tem,Ideal);
      u2 = compute(hum,VeryHumid);
      if (u1<=u2)
        Area = 0.5*(Level5[3]-Level5[0])*u1*(2-u1);
      else
        Area = 0.5*(Level5[3]-Level5[0])*u2*(2-u2);

    num = num + Area*(Level5[3]+Level5[0])/2;
    den = den + Area;
    }

    if ((tem >= 10 && tem <= 20) && (hum <= 40))
    {
      u1 = compute(tem,Warm);
      u2 = compute(hum,ExtremeDry);
      if (u1>=u2)
        Area = 0.5*(Level2[3]-Level2[0])*u1*(2-u1);
      else
        Area = 0.5*(Level2[3]-Level2[0])*u2*(2-u2);

      num = num + Area*(Level2[3]+Level2[0])/2;
      den = den + Area;
    }


    if ((tem >= 25 && tem <= 40) && (hum >= 20 && hum <= 60))
    {
      u1 = compute(tem,Warm);
      u2 = compute(hum,Dry);
      if (u1>=u2)
        Area = 0.5*(Level3[3]-Level3[0])*u1*(2-u1);
      else
        Area = 0.5*(Level3[3]-Level3[0])*u2*(2-u2);

    num = num + Area*(Level3[3]+Level3[0])/2;
    den = den + Area;
    }


    if ((tem >= 25 && tem <= 40) && (hum >= 40 && hum <= 80))
    {
      u1 = compute(tem,Warm);
      u2 = compute(hum,Normal);
      if (u1>=u2)
        Area = 0.5*(Level4[3]-Level4[0])*u1*(2-u1);
      else
        Area = 0.5*(Level4[3]-Level4[0])*u2*(2-u2);

    num = num + Area*(Level4[3]+Level4[0])/2;
    den = den + Area;
    }

    
    if ((tem >= 25 && tem <= 40) && (hum >= 60 && hum <= 100))
    {
      u1 = compute(tem,Warm);
      u2 = compute(hum,Humid);
      if (u1<=u2)
        Area = 0.5*(Level5[3]-Level5[0])*u1*(2-u1);
      else
        Area = 0.5*(Level5[3]-Level5[0])*u2*(2-u2);

    num = num + Area*(Level5[3]+Level5[0])/2;
    den = den + Area;
    }
    
    if ((tem >= 25 && tem <= 40) && (hum >= 80))
    {
      u1 = compute(tem,Warm);
      u2 = compute(hum,VeryHumid);
      if (u1<=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

    num = num + Area*(Level6[3]+Level6[0])/2;
    den = den + Area;
    }

    if ((tem >= 10 && tem <= 20) && (hum <= 40))
    {
      u1 = compute(tem,Hot);
      u2 = compute(hum,ExtremeDry);
      if (u1>=u2)
        Area = 0.5*(Level4[3]-Level4[0])*u1*(2-u1);
      else
        Area = 0.5*(Level4[3]-Level4[0])*u2*(2-u2);

      num = num + Area*(Level4[3]+Level4[0])/2;
      den = den + Area;
    }


    if ((tem >= 35 && tem <= 50) && (hum >= 20 && hum <= 60))
    {
      u1 = compute(tem,Hot);
      u2 = compute(hum,Dry);
      if (u1>=u2)
        Area = 0.5*(Level5[3]-Level5[0])*u1*(2-u1);
      else
        Area = 0.5*(Level5[3]-Level5[0])*u2*(2-u2);

    num = num + Area*(Level5[3]+Level5[0])/2;
    den = den + Area; 
    }


    if ((tem >= 35 && tem <= 50) && (hum >= 40 && hum <= 80))
    {
      u1 = compute(tem,Hot);
      u2 = compute(hum,Normal);
      if (u1>=u2)
        Area = 0.5*(Level5[3]-Level5[0])*u1*(2-u1);
      else
        Area = 0.5*(Level5[3]-Level5[0])*u2*(2-u2);

    num = num + Area*(Level5[3]+Level5[0])/2;
    den = den + Area; 
    }

    if ((tem >= 35 && tem <= 50) && (hum >= 60 && hum <= 100))
    {
      u1 = compute(tem,Hot);
      u2 = compute(hum,Humid);
      if (u1<=u2)
        Area = 0.5*(Level5[3]-Level5[0])*u1*(2-u1);
      else
        Area = 0.5*(Level5[3]-Level5[0])*u2*(2-u2);

    num = num + Area*(Level5[3]+Level5[0])/2;
    den = den + Area;
    }
    
    if ((tem >= 35 && tem <= 50) && (hum >= 80))
    {
      u1 = compute(tem,Hot);
      u2 = compute(hum,VeryHumid);
      if (u1<=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

    num = num + Area*(Level6[3]+Level6[0])/2;
    den = den + Area;
    }

    if ((tem >= 10 && tem <= 20) && (hum <= 40))
    {
      u1 = compute(tem,ExtremeHot);
      u2 = compute(hum,ExtremeDry);
      if (u1>=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

      num = num + Area*(Level6[3]+Level6[0])/2;
      den = den + Area;
    }
    
    
    if ((tem >= 40) && (hum >= 20 && hum <= 60))
    {
      u1 = compute(tem,ExtremeHot);
      u2 = compute(hum,Dry);
      if (u1>=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

    num = num + Area*(Level5[3]+Level5[0])/2;
    den = den + Area; 
    }


    if ((tem >= 40) && (hum >= 40 && hum <= 80))
    {
      u1 = compute(tem,ExtremeHot);
      u2 = compute(hum,Normal);
      if (u1>=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

    num = num + Area*(Level6[3]+Level6[0])/2;
    den = den + Area; 
    }

    if ((tem >= 40) && (hum >= 60 && hum <= 100))
    {
      u1 = compute(tem,ExtremeHot);
      u2 = compute(hum,Humid);
      if (u1>=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

    num = num + Area*(Level6[3]+Level6[0])/2;
    den = den + Area;
    }
    
    if ((tem >= 40) && (hum >= 80))
    {
      u1 = compute(tem,ExtremeHot);
      u2 = compute(hum,VeryHumid);
      if (u1<=u2)
        Area = 0.5*(Level6[3]-Level6[0])*u1*(2-u1);
      else
        Area = 0.5*(Level6[3]-Level6[0])*u2*(2-u2);

    num = num + Area*(Level6[3]+Level6[0])/2;
    den = den + Area;
    }
  }

 
  float defuzz = num/den;
  
  if (num != 0)
  {
    analogWrite(en1,defuzz);
    Serial.print("\nFAN STATUS : ON");
    vlevel = (5*defuzz)/255;
  }
  else
  {
    analogWrite(en1,num);
    Serial.print("\nFAN STATUS : OFF");
    vlevel = 0;
  }
  Serial.print("\nThe Motor Rotates at : ");
  Serial.print(vlevel);
  Serial.print("V\n");
  


// Fan speed control ends here



  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  if(ser.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(tem);
  getStr +="&field2=";
  getStr += String(hum);
  getStr +="&field3=";
  getStr += String(distance);
  getStr +="&field4=";
  getStr += String(rain);
  getStr +="&field5=";
  getStr += String(sun);
   getStr +="&field6=";
  getStr += String(vlevel);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">"))
  {
    ser.print(getStr);
    Serial.println(getStr);
  }
  else
  {
    ser.println("AT+CIPCLOSE");
    Serial.println("CIPCLOSE");
  }
    
  // thingspeak needs 15 sec delay between updates
 delay(16000);  
}
