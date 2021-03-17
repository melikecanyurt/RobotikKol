//ELE 495 Bitirme Projesi Grup 13
//Bu kod Robotik Kol Projesi için parmakların kablosuz bir şekilde kontrol edecek
//olan verici devresini çalıştırmak için kullanılmaktadır.


// Verici Kod

#include <SPI.h>                      
#include "RF24.h"                     

int msg[3]; //gönderilecek data sayısına göre oluşturulan mesaj

//Flex sensör pinleri

int flex_3 = A2;//yüzük ve serçe
int flex_2 = A1;//işaret ve orta
int flex_1 = A0;//baş parmak

//flexten okunacak olan direç değerleri için değişkenler

int flex_3_val;
int flex_2_val;
int flex_1_val;

RF24 radio(7,8);  // kablosuz modülün CS ve CSN pinlerinin arduino bağlantı noktaları
                                      
const uint64_t pipe = 0xE8E8F0F0E1LL; //modülün adresi

void setup(void){
  Serial.begin(9600);
  radio.begin();                      //modemin aktive edilmesi
  radio.openWritingPipe(pipe);        
}

void loop(void){

 //flex sensörlerden okunan değerler servo motorlardaki açı değerleri ile eşleştirilmesi
  flex_3_val = analogRead(flex_3);
  flex_3_val = map(flex_3_val, 490, 730, 170, 50);
 
  flex_2_val = analogRead(flex_2);
  flex_2_val = map(flex_2_val, 450, 730, 170, 50);
  
  flex_1_val = analogRead(flex_1);
  flex_1_val = map(flex_1_val, 490, 620, 170, 50);
  
  // açı değerlerini içeren mesajın oluşturulması
  msg[2] = flex_3_val;
  msg[1] = flex_2_val;
  msg[0] = flex_1_val;
  radio.write(msg, sizeof(msg)); // mesajın alıcı modüle iletilmesi
}
