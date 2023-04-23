#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


/* Put your SSID & Password */
const char* ssid = "Smart-Farm";       
const char* password = "123456789"; 

ESP8266WebServer server(80);

bool LEDstatus = HIGH;
int totalDevice = 0;
bool sendNotification = LOW;
bool smsSent = LOW;


int diseaseSensorPin = D1;
int lightsPin = D2;
bool automaticLights = HIGH;
int automaticLightsPin = D3;
int smsPin = D5;


void setup() {
  Serial.begin(9600);
  pinMode(D4, OUTPUT); 
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP:");
  Serial.println(myIP);

  pinMode(diseaseSensorPin,INPUT);
  pinMode(lightsPin,OUTPUT);
  pinMode(automaticLightsPin,OUTPUT);
  pinMode(smsPin,OUTPUT);
  
  
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.on("/sendNotificationOn",handle_sendNotificationOn);
  server.on("/sendNotificationOff",handle_sendNotificationOff);
  server.on("/autoLightsOn",handle_automaticLightsOn);
  server.on("/autoLightsOff",handle_automaticLightsOff);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  //Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();

  totalDevice = WiFi.softAPgetStationNum();
  
  if(totalDevice)
  {digitalWrite(D4, LOW);}
  else
  {digitalWrite(D4, HIGH);}


  if(sendNotification){

    digitalWrite(smsPin,HIGH);
    
  }else{

    digitalWrite(smsPin,LOW);
  }

  Serial.println("automaticLights: ");
  Serial.println(automaticLights);

  if(automaticLights){

    digitalWrite(lightsPin,HIGH);
    
  }else{


    if(LEDstatus){

      digitalWrite(lightsPin,HIGH);
      
    }else{

      digitalWrite(lightsPin,LOW);
      
    }
  }

  
}

void handle_OnConnect() {
  LEDstatus = LOW;
  Serial.println("LED: ON");
  server.send(200, "text/html", updateWebpage(LEDstatus,sendNotification,automaticLights)); 
}

void handle_ledon() {
  LEDstatus = HIGH;
  Serial.println("LED: ON");
  server.send(200, "text/html", updateWebpage(LEDstatus,sendNotification,automaticLights)); 
}

void handle_ledoff() {
  LEDstatus = LOW;
  Serial.println("LED: OFF");
  server.send(200, "text/html", updateWebpage(LEDstatus,sendNotification,automaticLights)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_sendNotificationOn(){
  //sendNotification

  sendNotification = HIGH;

  server.send(200, "text/html" , updateWebpage(LEDstatus,sendNotification,automaticLights));
  
}

void handle_sendNotificationOff(){
  //sendNotification

  sendNotification = LOW;

  server.send(200, "text/html" , updateWebpage(LEDstatus,sendNotification,automaticLights));
  
}

void handle_automaticLightsOn(){

  automaticLights = HIGH;

  server.send(200, "text/html" , updateWebpage(LEDstatus,sendNotification,automaticLights));
  
}

void handle_automaticLightsOff(){

  automaticLights = LOW;

  server.send(200, "text/html" , updateWebpage(LEDstatus,sendNotification,automaticLights));
  
}

String updateWebpage(uint8_t LEDstatus,bool sendNotificationStatus,bool automaticLights){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Welcome to Smart Farm</h1>\n";
  ptr +="<h3>Here are the stats of your farm</h3>\n";

  Serial.println(totalDevice);
  
  if(totalDevice==0){

    ptr +="<h3>Total Number Of Devices Connected To Farm : 0</h3>\n";
    
  }else if(totalDevice==1){

     ptr +="<h3>Total Number Of Devices Connected To Farm : 1</h3>\n";
    
  }else if(totalDevice==2){

     ptr +="<h3>Total Number Of Devices Connected To Farm : 2</h3>\n";
    
  }else if(totalDevice==3){

     ptr +="<h3>Total Number Of Devices Connected To Farm : 3</h3>\n";
    
  }else if(totalDevice==4){

     ptr +="<h3>Total Number Of Devices Connected To Farm : 4</h3>\n";
    
  }else if(totalDevice==5){

     ptr +="<h3>Total Number Of Devices Connected To Farm : 5</h3>\n";
    
  }else{

    ptr +="<h3>Total Number Of Devices Connected To Farm : 5+</h3>\n";
    
  }

   if(sendNotificationStatus){
    ptr +="<p>Disease SMS Notifications : ON</p><a class=\"button button-off\" href=\"/sendNotificationOff\">Turn OFF</a>\n";
   }else{
    ptr +="<p>Disease SMS Notifications : OFF</p><a class=\"button button-on\" href=\"/sendNotificationOn\">Turn ON</a>\n";
  }
  
  
   if(automaticLights){

    ptr +="<p>Scheduled Lights : ON</p><a class=\"button button-off\" href=\"/autoLightsOff\">Turn OFF</a>\n";
    
   }else{

    ptr +="<p>Scheduled Lights : OFF</p><a class=\"button button-on\" href=\"/autoLightsOn\">Turn ON</a>\n";

    if(LEDstatus){
    ptr +="<p>Farm Lights : ON</p><a class=\"button button-off\" href=\"/ledoff\">Turn OFF</a>\n";
   }else{
    ptr +="<p>Farm Lights : OFF</p><a class=\"button button-on\" href=\"/ledon\">Turn ON</a>\n";
    
  }
   }
   
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
