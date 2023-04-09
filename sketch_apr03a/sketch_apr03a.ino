rt#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <dht.h> // librarie pentru temperatura si umiditate


//variables and objects
Servo motor; // servomotor 
int primit = 48; // ce cod primim de la telefon
bool deschis = 0; // inchis
int pos = 180, raining = A2;
int temp = 7;
int relay4 = 4;
int mic = 12;
dht tempmodule;
bool verificare_ploua = false;
//functions
void ploua();
void sound();
void temperature();
///pentru deschis == 1 - atunci geamul este deschis, altfel este inchis
void setup() {
  pinMode(raining, INPUT);
  pinMode(mic, INPUT);
  Serial.begin(9600);
  motor.attach(8); // va fi atasat de pinul 8
  motor.write(180);
}
  
void loop() {
  if(Serial.available()){
      primit = Serial.read();
   }
   if(primit == 49){ // trebuie sa deschid fereastra
    deschis = 1;
    if(pos >= 1){
       motor.write(pos), pos--;
       delay(15);
     }
   }
    if(primit == 52){ //trebuie sa inchid
      if(pos <= 180){
         motor.write(pos), pos++;
          delay(15);
        }  
       if(!pos) deschis = 0; // asta inseamna ca geamul este inchis
    }
  
  ploua(); //in cazul in care ploua trebuie sa inchid geamul
  delay(15);
  if(!verificare_ploua)
  temperature(); // verific temperatura
  delay(15);
  sound(); // masor numarul de decibeli pentru a inchide geamul
 // delay(10); 
}
void closeWindow(){ //functie pentru a inchide geamul
    if(deschis){ // daca fereastra este deschisa
      while(pos <= 180){
        motor.write(pos);
        pos ++;
        delay(15);
      }
      deschis = 0; // geamul este inchis
      
    }
  
}
void openWindow(){ //functia pentru a deschide geamul
  if(!deschis){
    while(pos >= 1){
      motor.write(pos);
      pos--;
      delay(15);
    }
    deschis = 1;
  }
}
void ploua(){
    int inchideGeamul_ca_te_ploua = analogRead(A2);
    //Serial.println(inchideGeamul_ca_te_ploua);
    if(deschis && inchideGeamul_ca_te_ploua < 400){ // daca ploua si fereastra este deschisa
       closeWindow();
       verificare_ploua = true;
    }
    else verificare_ploua = false;
}

void sound(){
    int min1 = 1024, max1 = 0;
    int sunet = analogRead(A3);
   /* for(int i = 1; i <= 100; ++i){
      min1 = min(min1, sunet);
      max1 = max(max1, sunet);
    }
    int dif = max1 - min1;*/
    int x = map(sunet, 0, 1204, 0, 123);
    //Serial.println(x);
    if(deschis && x > 50){
      closeWindow();
      delay(15);
    }
  
}

void temperature(){
    int cr = tempmodule.read11(temp); //citesc datele venite de la modului de temperatura si umiditate
    float tmp = tempmodule.temperature; //citesc temperatura 
    float umd = tempmodule.humidity; //citesc umiditatea 
    if(tmp >= (float)29){
      openWindow(); //deschis fereastra
    }
    Serial.println(tmp);
}
/*
  comenzi:
  48 - nimic
  49 - open
  52 - close
  daca ploua il inchid 
  daca sunt prea multi decibeli il inchid
  
 */
