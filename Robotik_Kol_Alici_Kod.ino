//ELE 495 Bitirme Projesi Grup 13
//Bu kod Robotik Kol Projesi için x-ekseni, y-ekseni ve hareketlerinin yapılmasını,
//sıcaklığın LCD ekrana yazdırılmasını ve parmakların çalışmasını sağlayacak alıcı
//devresini çalıştırmak için kullanılmaktadır.

// Alıcı Kod
#include <Servo.h>    //Servo motor kütüphanesi
#include <SPI.h>      //Seri çevresel arabirim kütüphanesi
#include "RF24.h"     //Kablosuz haberleşme modülü kütüphanesi

//LCD Ekran

#include <LiquidCrystal.h> //LCD Ekran kütüphanesi
LiquidCrystal lcd(12, 11, 46, 47, 44, 45); 

// Sıcaklık Sensoru
#include <mlx90615.h> // Kızılötesi sıcaklık sensörü kütüphanesi
MLX90615 mlx = MLX90615();

//x ekseni hareketi için Step motor
#include <Stepper.h> // Step motor kütüphanesi
const int stepPin = 32;
const int dirPin = 33;
#define Buton_1 22
#define Buton_2 24

// Bilek hareketi
Servo servo_bilek;
int bilek_pos=90;
int buton_sag=26;
int buton_sol=27;

// y ekseni için Step motorlar 
const int stepPin1 = 38;
const int dirPin1 = 39;
const int stepPin2 = 40;
const int dirPin2 = 41; 
#define Buton_3 23 
#define Buton_4 25 



// Parmak hareketler için Servo motorlar
Servo myServo1;
Servo myServo3;
Servo myServo4;
Servo myServo2;
Servo myServo5;

// Kablosuz haberleşme
// transmitter kodda aynı pipe adresine sahip olan modül ile haberleşecek
//ve aldığı datayı "msg" dizisine yazdıracak.
RF24 radio(7,8);   
const uint64_t pipe = 0xE8E8F0F0E1LL; 
int msg[3];

void setup(){
// Sıcaklık sensörü ve LCD ekran
lcd.begin(16,2); 
mlx.begin();
  
// x ekseni için Step motor
pinMode(stepPin,OUTPUT); 
pinMode(dirPin,OUTPUT); 
pinMode(Buton_1,INPUT); //kolu sağa döndürmek için
pinMode(Buton_2,INPUT); //kolu sola döndürmek için

//Bilek hareketi
servo_bilek.attach(10);
  pinMode(buton_sag,INPUT);
  pinMode(buton_sol,INPUT);

// Step motorlar (y-ekseni icin)
  pinMode(stepPin1,OUTPUT);  
  pinMode(dirPin1,OUTPUT);   
  pinMode(stepPin2,OUTPUT);  
  pinMode(dirPin2,OUTPUT);   
  pinMode(Buton_3,INPUT);    
  pinMode(Buton_4,INPUT);      
  
// Parmak hareketleri için servo motorlar

  myServo1.attach(2); //Eldivende A0 pinine bağlı olan flex ile kontrol edilecek (baş parmak)
  myServo2.attach(3); //Eldivende A1 pinine bağlı olan flex ile kontrol edilecek (işaret parmak)
  myServo3.attach(4); //Eldivende A1 pinine bağlı olan flex ile kontrol edilecek (orta parmak)
  myServo4.attach(5); //Eldivende A2 pinine bağlı olan flex ile kontrol edilecek (yüzük parmak)
  myServo5.attach(6); //Eldivende A2 pinine bağlı olan flex ile kontrol edilecek (serçe parmak)
  
 // Kablosuz
  radio.begin();                    //kablosuz modül aktive ediliyor.
  radio.openReadingPipe(1, pipe);   //data alacağı adresi belirliyor,
  radio.startListening();           //modülden veri alma etkinleştiriliyor,
  }


void loop(){
  
  //Sıcaklık Sensöründen değer okunması ve değerin  ve LCD ekrana yazdırılması.
 Serial.println("---------------------------------------------------------------");
 int nesne = mlx.get_object_temp();
 Serial.print("Nesne sıcaklığı=");
 Serial.println(nesne);
 Serial.println("-------------------------");
 lcd.clear(); 
 lcd.setCursor(0,0);
 lcd.print("Temp:");
 lcd.setCursor(6,0);
 lcd.print(nesne);
 lcd.setCursor(10,0);
 lcd.print("*C");
  
// x ekseni hareketi için Step motor kodları
if (digitalRead(Buton_1) == 1){
digitalWrite(dirPin,HIGH); 
digitalWrite(stepPin,HIGH);
delayMicroseconds(2000);
digitalWrite(stepPin,LOW);
delayMicroseconds(2000); 
}
if (digitalRead(Buton_2) == 1){
digitalWrite(dirPin,LOW); 
digitalWrite(stepPin,HIGH);
delayMicroseconds(2000);
digitalWrite(stepPin,LOW);
delayMicroseconds(2000); 
}

//Bilek hareketlerinin gerçekleştirilmesi
 if(digitalRead(buton_sag)==1){
    bilek_pos=100;
    servo_bilek.write(bilek_pos);
  }
 else if(digitalRead(buton_sol)==1){
    bilek_pos=20;
    servo_bilek.write(bilek_pos);
  } 
  else {
    bilek_pos=60;
    servo_bilek.write(bilek_pos);
    }

// y ekseni hareketi için Step motor kodları
Serial.print("buton_3="); 
 Serial.println(digitalRead(Buton_3));
    if  (digitalRead(Buton_3) == 1){ 
      digitalWrite(dirPin1,HIGH); 
      digitalWrite(dirPin2,LOW);
      digitalWrite(stepPin1,HIGH);
     digitalWrite(stepPin2,HIGH);
     delayMicroseconds(500);
     digitalWrite(stepPin1,LOW);
     digitalWrite(stepPin2,LOW);
     delayMicroseconds(500);
      }
    Serial.print("buton_4=");
    Serial.println(digitalRead(Buton_4));
   if  (digitalRead(Buton_4) == 1){
      digitalWrite(dirPin1,LOW); 
      digitalWrite(dirPin2,HIGH);                                                                                                            
      digitalWrite(stepPin1,HIGH);
      digitalWrite(stepPin2,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin1,LOW);
      digitalWrite(stepPin2,LOW);
      delayMicroseconds(500);
   }
  // Kol Kablosuz haberleşme
  // Eğer arduino nanoya bağlanmış olan kablosuz modülden servolar için
  // derece bilgisi gelirse bu açı bilgilerini ilgili servolara gönderilip parmak hareketleri
  // sağlanacaktır. 
  if(radio.available()){
    bool done = false;
    while (!done){
    done = radio.read(msg, sizeof(msg));
if (nesne<50) {// bu satırdaki "if" komutu ile nesne sıcaklığı kontrol edilip 50 derece üzerinde değil ise
// bu kod parçası çalışacak ve parmaklar kablosuz haberleşme modülünden gelen bilgilere göre hareket
//ettirilecektir.
     
    myServo1.write(msg[0]); //A0 bas
    myServo2.write(msg[1]); //A1 isaret
    myServo3.write(msg[1]); //A1 orta
    myServo4.write(msg[2]); //A2 yuzuk
    myServo5.write(msg[2]); //A2 serce

    }
    else if(nesne>50){ // bu satırdaki if komutu ile nesne sıcaklığı 50 derecenin üzerinde ise 
      //bu kod parçası çalışacak yukarıdaki parça çalışmayacaktır. Böylece sıcaklık yüksek ise 
      // kablosuz haberleşme modülünden gelen bilgi ne olursa olsun parmaklar açık konumda tutulacaktır.
    myServo1.write(170); //A0 bas
    myServo2.write(170); //A1 isaret
    myServo3.write(170); //A1 orta
    myServo4.write(170); //A2 yuzuk
    myServo5.write(170); //A2 serce

      }
    }
  }
}
