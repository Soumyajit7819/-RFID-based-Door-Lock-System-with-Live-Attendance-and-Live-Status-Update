#include <ESP8266WiFi.h>
#include <Wire.h> 

#include <LiquidCrystal_I2C.h>
#include <SPI.h> 
#include <RFID.h>
#include "FirebaseESP8266.h"                                                                              // Install Firebase ESP8266 library

 
#include <NTPClient.h>
#include <WiFiUdp.h>

#define RELAY D3

#define FIREBASE_HOST ""                       //Without http:// or https:// schemes
#define FIREBASE_AUTH ""

const char ssid[] = "IEEE_ADGITM";
const char pass[] = "IEEE@PVT2022";

RFID rfid(D8, D0);                                                                                       //D10:pin of tag reader SDA. D9:pin of tag reader RST 
unsigned char str[MAX_LEN];                                                                              //MAX_LEN is 16: size of the array 
LiquidCrystal_I2C lcd(0x27,16,2);                                                                        // set the LCD address to 0x27 for a 16 chars and 2 line display

WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 19800; //(UTC+5:30)
NTPClient timeClient(ntpUDP, "pool.ntp.org");

String uidPath= "/";
FirebaseJson json;
FirebaseData firebaseData;                                                                               //Define FirebaseESP8266 data object

unsigned long lastMillis = 0;
String alertMsg;
String StrNumber;
long int rfidnum = 0;
String device_id="IEEE Adgitm EXECOMM 2022";
//boolean checkIn = true;



void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n connected!");
}




void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  SPI.begin();
  rfid.init();

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);       // relay NO Configuration, current not flowing normally i.e door closed
//digitalWrite(RELAY, LOW);        // relay NC Configuration, current not flowing normally i.e door closed
  
  lcd.init();                      // initialize the lcd 
  lcd.clear();
  lcd.backlight();
  
  timeClient.begin();
  timeClient.setTimeOffset(utcOffsetInSeconds);
  connect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}



void checkAccess (String temp)    //Function to check if an identified tag Current Status and mark the attendance
{
    Wire.setClock(10000);
    lcd.setCursor(1,0);   
    lcd.print("SCAN YOUR RFID");
    if(Firebase.getInt(firebaseData, uidPath+"/users/"+temp)){
      
      if (firebaseData.intData() == 0)         //If firebaseData.intData() == checkIn
      {  
          alertMsg="CHECKING IN";
          lcd.setCursor(2,1);   
          lcd.print(alertMsg);
          delay(1000);

          json.add("time", String(timeClient.getFormattedDate()));
          json.add("id", device_id);
          json.add("uid", temp);
          json.add("status",1);

          Firebase.setInt(firebaseData, uidPath+"/users/"+temp,1);
          
          if (Firebase.pushJSON(firebaseData, uidPath+ "/attendence", json)) {
            Serial.println(firebaseData.dataPath() + firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }
      }
      else if (firebaseData.intData() == 1)   //If the lock is open then close it
      { 
          alertMsg="CHECKING OUT";
          lcd.setCursor(2,1);   
          lcd.print(alertMsg);
          delay(1000);

          Firebase.setInt(firebaseData, uidPath+"/users/"+temp,0);
          
          json.add("time", String(timeClient.getFormattedDate()));
          json.add("id", device_id);
          json.add("uid", temp);
          json.add("status",0);
          if (Firebase.pushJSON(firebaseData, uidPath+ "/attendence", json)) {
            Serial.println(firebaseData.dataPath() + firebaseData.pushName()); 
          } else {
            Serial.println(firebaseData.errorReason());
          }
      }
      
 
    }
    else
    {
      Serial.println("Failed to Connect");
      Serial.println("REASON: " + firebaseData.errorReason());
    }
}

void loop() {
  Wire.setClock(10000);
  timeClient.update();
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)                         //Wait for a tag to be placed near the reader
  { 
//  Serial.println("Card found"); 
    String temp = "";                                                   //Temporary variable to store the read RFID number
    if (rfid.anticoll(str) == MI_OK)                                    //Anti-collision detection, read tag serial number 
    { 
//    Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                                       //Record and display the tag serial number 
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      
//    Serial.println (temp);
      for(int i=0 ; i<= 8; i++){
        StrNumber += temp[i];
      }
//    Serial.println (StrNumber);
      rfidnum = StrNumber.toInt();
//    Serial.println (rfidnum);
      
      if ( rfidnum == 811001091 || rfidnum == 11015029 || rfidnum == 511590899 || rfidnum == 111164089 || rfidnum == 101111802 || 
           rfidnum == 101113150 || rfidnum == 101160010 || rfidnum == 411511151 || rfidnum == 511116151 || rfidnum == 611013141 || 
           rfidnum == 711137141 || rfidnum == 811271511 || rfidnum == 711141051  || rfidnum == 911070299 || rfidnum == 911656312 ){
        
        digitalWrite(RELAY, LOW);                                                        // relay NO Configuration, current flowing i.e door opens
        delay(2000);                                                                     // Door Open for 1 sec
        digitalWrite(RELAY, HIGH);                                                       // relay NO Configuration, current not flowing normally i.e door closed
        checkAccess (temp);                                                              //Checks the current status of student and marks the attendance
//      Serial.println("Authorized access");
//      Serial.println();
      }
      
      else {
//      Serial.println("Access Denied");
//      Serial.println("Card Not Registered");
        alertMsg="ACCESS DENIED";
        lcd.setCursor(2,1);   
        lcd.print(alertMsg);
        delay(1000);
      }
     } 

    StrNumber.clear();
//  Serial.println (StrNumber);
    rfid.selectTag(str);                                                        //Lock card to prevent a redundant read, removing the line will make the sketch read cards continually
  }
  rfid.halt();

  lcd.setCursor(1,0);   
  lcd.print("SCAN YOUR RFID");
  lcd.setCursor(2,1);   
  lcd.print("IEEE ADGITM");
  delay(500);
  lcd.clear();
}
