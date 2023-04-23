#include <Servo.h>

Servo myservo;  

int pos = 0; 

void setup() {
  myservo.attach(3); 

  pinMode(A2,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  for (pos = 0; pos <= 180 ; pos += 1) {
    myservo.write(pos); 
    delay(100);  
    
    if(pos==90 || pos==160){

      signalCheckPlant(pos);
      
    }
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);
    delay(30);                 
  }


  analogWrite(A2,HIGH); //Turn on the motors

  delay(4000);

  analogWrite(A2,LOW);
  

  
}


void signalCheckPlant(int pos){

  
  Serial.print(pos);

  while(Serial.available()==0){

    
  }

  Serial.read();
  

  Serial.print("Done");
  
}
