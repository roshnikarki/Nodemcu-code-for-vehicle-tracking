#include<Wire.h>
#include <FirebaseESP8266.h>  
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define FIREBASE_HOST "vehicle-tracking-project-7af92-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Tx1x6nZK9rd6Nx6LrQjLJimHuM2qoIgDXZ3Utzhj"
#define WIFI_SSID "nodemcu1"
#define WIFI_PASSWORD "nodemcu1"

FirebaseData firebaseData;
FirebaseJson json;  
          

void setup() {
  Wire.begin();
  Serial.begin(9600);

  wifiConnect();

  Serial.println("Connecting Firebase.....");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase OK.");
}

void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop() {
  Wire.requestFrom(2,30);
  String string,string1,string2,string3,string4,string5,string6;
  do
  {
    char c = Wire.read();
    string = string+c;
    string1 = string.substring(0,8);
    string2 = string.substring(9,17);
    string4 = string.substring(18,21);
    string5 = string.substring(22,25);
    string6 = string.substring(26,27);
    string3 = string.substring(28);
  } while (Wire.available());

  char buf1[10];
  char buf2[10];
  char buf3[10];
  char buf4[10];
  char buf5[10];
  char buf6[10];
  Serial.println(string);
  string1.toCharArray(buf1, 10);
  long lati = atol(buf1);
  Serial.println((lati*0.000001),6);
  string2.toCharArray(buf2, 10);
  long longi = atol(buf2);
  Serial.println((longi*0.000001),6);
  string3.toCharArray(buf3, 10);
  int Speed = atoi(buf3);
  int count = atoi(buf6);
  Serial.println(Speed);

  string4.toCharArray(buf4, 10);
  string5.toCharArray(buf5, 10);
  string6.toCharArray(buf6,10);
  int Xaxis = atoi(buf4);
  int Yaxis = atoi(buf5);
  
  Serial.print("X-axis :");
  Serial.println(Xaxis);
  Serial.print("Y-axis :");
  Serial.println(Yaxis);
  Serial.println();

  Firebase.setFloat(firebaseData,"GPS/Latitude",lati*0.000001);
  Firebase.setFloat(firebaseData,"GPS/Longitude",longi*0.000001);
  Firebase.setFloat(firebaseData,"GPS/Speed",Speed);
  Firebase.setFloat(firebaseData,"GPS/SpeedLimitCrossed",count);

  if (Xaxis<=270 || Yaxis<=270 || Xaxis>=390 || Yaxis>=390)
  {
    Firebase.setString(firebaseData,"GPS/Acc-Alert","Alert");
  }
  else
  {
    Firebase.setString(firebaseData,"GPS/Acc-Alert","Safe");
  }  

  // put your main code here, to run repeatedly:

}
