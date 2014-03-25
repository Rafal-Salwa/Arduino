#include <TimedAction.h>
#include <Keypad.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <CapacitiveSensor.h>
#include <IRremote.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SoftwareSerial.h>

int iSensorValue = 0;

EthernetServer server(80);
EthernetClient client;
String bufferRequest = "";

byte mac[] = {  
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
  
  
IPAddress ip(192, 168, 1, 113);
IPAddress ip1(192, 168, 1, 111);
EthernetServer server2(8032);
unsigned int localPort = 8032; 
boolean incoming = 0;
// local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
long dane;




TimedAction timedAction1 = TimedAction(2000,sprawdz_temp);
TimedAction timedAction2 = TimedAction(2000,hc_sr_04);

CapacitiveSensor   cs_4_2 = CapacitiveSensor(8,9);
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);
long pojemnosc = 0;
boolean poczta = false;


/////////////////////////////
// IR
#define irPin 11
IRrecv irrecv(irPin);
decode_results results;

/////////////////////////////

/////////////////////////////
//PIR 1

int pir_calibrationTime = 15;

long unsigned int pir_lowIn;

long unsigned int pir_pause = 5000;

boolean pir_lockLow = true;
boolean pir_takeLowTime;

int pir_Pin = 48;
int pir_ledPin = 32;

/////////////////////////////
//PIR 2
// kod dla czujnika PIR zostal stworzony na podstwie dokumentacji Arduino 
// Adres: http://playground.arduino.cc/Code/PIRsense#.UyPtxN6JKwo
//the time when the sensor outputs a low impulse
long unsigned int pir_lowIn2;         

long unsigned int pir_pause2 = 5000;  

boolean pir_lockLow2 = true;
boolean pir_takeLowTime2;  

int pir_Pin2 = 49;
int pir_ledPin2 = 8;

/////////////////////////////
// CZUJNIK TEMP WILG
#define dht_dpin A15
#define dht_dpin2 A12
dht DHT;
dht DHT2;
/////////////////////////////

#define foto_pin 14
#define pot_pin 13
int pot_pot = 0;
int foto_pot = 0;
int fotorezystor = 0;
int potencjometr = 0;

/////////////////////////////
int buzzerPin = 7;
int buzzerPin2 = 46;


// ULTRADZWIEKI
#define trigPin 12
#define echoPin 11
long czas_trw, odleglosc;
// ULTRADZWIEKI

// ULTRADZWIEKI 2
#define trigPin2 47
#define echoPin2 46
long czas_trw2, odleglosc2;


//LCD
int LCD1 = 22;
int LCD2 = 23;
int LCD3 = 24;
int LCD4 = 25;
int LCD5 = 26;
int LCD6 = 27;
LiquidCrystal lcd(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6);
int wart = 0;
int curr_menu = 0;
int lcd_menu = 0;
boolean koniec_menu=false;

//LCD

//DIODA RGB
int RedPin = 5;
int GreenPin = 3;
int BluePin = 2;
//DIODA RGB



int zestaw_1_diodaPin1 = 30;
boolean zestaw_1_dioda1stan =false;
int zestaw_1_diodaPin2 = 31;
boolean zestaw_1_dioda2stan =false;
int zestaw_1_diodaPin3 = 28;
boolean zestaw_1_dioda3stan =false;
int zestaw_1_diodaPin4 = 29;
boolean zestaw_1_dioda4stan =false;

int zestaw_2_diodaPin1 = 9;
boolean zestaw_2_dioda1stan =false;
int zestaw_2_diodaPin2 = 34;
boolean zestaw_2_dioda2stan =false;
int zestaw_2_diodaPin3 = 35;
boolean zestaw_2_dioda3stan =false;
int zestaw_2_diodaPin4 = 13;
boolean zestaw_2_dioda4stan =false;

int odebrane=0;
int czas = 0;



const byte ROWS = 4; 
const byte COLS = 4; 
char kod[4] = {
  '1', '2', '3', '4'};
char znaki[4];
int i=0;
char keys[ROWS][COLS] = {
  {
    '1','2','3','A'                                      }
  ,
  {
    '4','5','6','B'                                      }
  ,
  {
    '7','8','9','C'                                      }
  ,
  {
    '*','0','#','D'                                      }
};
byte rowPins[ROWS] = {
  36,37,38,39}; //connect to row pinouts 
byte colPins[COLS] = {
  40,41,42,43}; //connect to column pinouts

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
boolean dostep = false;
boolean kod_anulowany = false;
int klaw_menu = 0;



void dioda1Wlacz(){
  digitalWrite(zestaw_1_diodaPin1, HIGH);
  zestaw_1_dioda1stan = true;
}

void dioda1Wylacz(){
  digitalWrite(zestaw_1_diodaPin1, LOW);
  zestaw_1_dioda1stan = false;
}

void dioda2Wlacz(){
  digitalWrite(zestaw_1_diodaPin2, HIGH);
  zestaw_1_dioda2stan = true;
}

void dioda2Wylacz(){
  digitalWrite(zestaw_1_diodaPin2, LOW);
  zestaw_1_dioda2stan = false;
}

void dioda3Wlacz(){
  digitalWrite(zestaw_1_diodaPin3, HIGH);
  zestaw_1_dioda3stan = true;
}

void dioda3Wylacz(){
  digitalWrite(zestaw_1_diodaPin3, LOW);
  zestaw_1_dioda3stan = false;
}

void dioda4Wlacz(){
  digitalWrite(zestaw_1_diodaPin4, HIGH);
  zestaw_1_dioda4stan = true;
}

void dioda4Wylacz(){
  digitalWrite(zestaw_1_diodaPin4, LOW);
  zestaw_1_dioda4stan = false;
}

void zestaw_2_dioda1Wlacz(){
  digitalWrite(zestaw_2_diodaPin1, HIGH);
  zestaw_2_dioda1stan = true;
}

void zestaw_2_dioda1Wylacz(){
  digitalWrite(zestaw_2_diodaPin1, LOW);
  zestaw_2_dioda1stan = false;
}

void zestaw_2_dioda2Wlacz(){
  digitalWrite(zestaw_2_diodaPin2, HIGH);
  zestaw_2_dioda2stan = true;
}

void zestaw_2_dioda2Wylacz(){
  digitalWrite(zestaw_2_diodaPin2, LOW);
  zestaw_2_dioda2stan = false;
}

void zestaw_2_dioda3Wlacz(){
  digitalWrite(zestaw_2_diodaPin3, HIGH);
  zestaw_2_dioda3stan = true;
}

void zestaw_2_dioda3Wylacz(){
  digitalWrite(zestaw_2_diodaPin3, LOW);
  zestaw_2_dioda3stan = false;
}

void zestaw_2_dioda4Wlacz(){
  digitalWrite(zestaw_2_diodaPin4, HIGH);
  zestaw_2_dioda4stan = true;
}

void zestaw_2_dioda4Wylacz(){
  digitalWrite(zestaw_2_diodaPin4, LOW);
  zestaw_2_dioda4stan = false;
}


int wczytajTemp(){
  return DHT.temperature; 
}

int wczytajWilg(){
  return DHT.humidity; 
}

int wczytajTemp2(){
  return DHT2.temperature; 
}

int wczytajWilg2(){
  return DHT2.humidity; 
}

void hc_sr_04(){
  digitalWrite(trigPin, LOW);

  delayMicroseconds(20);
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(20); 
  digitalWrite(trigPin, LOW);

  czas_trw = pulseIn(echoPin, HIGH);

  odleglosc = (czas_trw/2) / 29.1;



  if (odleglosc >= 400 || odleglosc <= 0){

  }

  else if (odleglosc >=100 && odleglosc <= 150){
    digitalWrite(zestaw_1_diodaPin1, HIGH);

  }
  else if (odleglosc >=50 && odleglosc <= 100){
    digitalWrite(zestaw_1_diodaPin1, HIGH);
    digitalWrite(zestaw_1_diodaPin2, HIGH);

  }
  else if (odleglosc >=10 && odleglosc <= 50){
    digitalWrite(zestaw_1_diodaPin1, HIGH);
    digitalWrite(zestaw_1_diodaPin2, HIGH);
    digitalWrite(zestaw_1_diodaPin3, HIGH);

  }
  else if (odleglosc >0 && odleglosc <= 10){
    digitalWrite(buzzerPin, HIGH);


  }
  else {
    digitalWrite(zestaw_1_diodaPin1, LOW);
    digitalWrite(zestaw_1_diodaPin2, LOW);
    digitalWrite(zestaw_1_diodaPin3, LOW);
    digitalWrite(buzzerPin, LOW);

  }



  digitalWrite(trigPin2, LOW);
  delayMicroseconds(20);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(20); 
  digitalWrite(trigPin2, LOW);
  czas_trw2 = pulseIn(echoPin2, HIGH);
  odleglosc2 = (czas_trw2/2) / 29.1;
  if (odleglosc2 >= 400 || odleglosc2 <= 0){

  }

  else if (odleglosc2 >=100 && odleglosc2 <= 150){
    digitalWrite(zestaw_2_diodaPin1, HIGH);

  }
  else if (odleglosc2 >=50 && odleglosc2 <= 100){
    digitalWrite(zestaw_2_diodaPin1, HIGH);
    digitalWrite(zestaw_2_diodaPin2, HIGH);

  }
  else if (odleglosc2 >=10 && odleglosc2 <= 50){
    digitalWrite(zestaw_2_diodaPin1, HIGH);
    digitalWrite(zestaw_2_diodaPin2, HIGH);
    digitalWrite(zestaw_2_diodaPin3, HIGH);

  }
  else if (odleglosc2 >0 && odleglosc2 <= 10){
    digitalWrite(zestaw_2_diodaPin1, HIGH);
    digitalWrite(zestaw_2_diodaPin2, HIGH);
    digitalWrite(zestaw_2_diodaPin3, HIGH);
    digitalWrite(buzzerPin, HIGH);
  }
  else {
    digitalWrite(zestaw_2_diodaPin1, LOW);
    digitalWrite(zestaw_2_diodaPin2, LOW);
    digitalWrite(zestaw_2_diodaPin3, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}

void foto_poto(){
  foto_pot = analogRead(foto_pin);
  fotorezystor = map(foto_pot, 0, 1023, 0, 100);
  pot_pot = analogRead(pot_pin);
  potencjometr = map(pot_pot, 0, 1023, 0, 100);
  if(fotorezystor <=10){
    digitalWrite(33, HIGH);
  }
  else{
    digitalWrite(33, LOW);
  }
  sprawdz_klawiature();
}

void oblicz_pojemnosc(){

  long start = millis();
  long total1 =  cs_4_2.capacitiveSensor(30);
  long total2 =  cs_4_5.capacitiveSensor(30);
  long total3 =  cs_4_8.capacitiveSensor(30);

  Serial.print(millis() - start); 
  Serial.print("\t");

  Serial.print(total1);
  Serial.print("\t");
  pojemnosc = total1;
  Serial.print(total2);
  Serial.print("\t");
  Serial.println(total3);
  if(total1 >5000){
    Serial.print("poczta");
    poczta = true;
  }
  sprawdz_klawiature();
}



void sprawdz_wejscie(int odebrane){

  switch(odebrane){
  case 1:
    if(zestaw_1_dioda1stan){
      dioda1Wylacz();
      Serial.println("Dioda 1 wylaczona");
    }
    else{
      dioda1Wlacz();
      Serial.println("Dioda 1 wlaczona");
    }

    break;

  case 2:
    if(zestaw_1_dioda2stan){
      dioda2Wylacz();
      Serial.println("Dioda 2 wylaczona");
    }
    else{

      dioda2Wlacz();
      Serial.println("Dioda 2 wlaczona");
    }
    break;

  case 3:
    if(zestaw_1_dioda3stan){
      dioda3Wylacz();
      Serial.println("Dioda 3 wylaczona");
    }
    else{

      dioda3Wlacz();
      Serial.println("Dioda 3 wlaczona");
    }
    break;

  case 4:
    if(zestaw_1_dioda4stan){
      dioda4Wylacz();
      Serial.println("Dioda 4 wylaczona");
    }
    else{

      dioda4Wlacz();
      Serial.println("Dioda 4 wlaczona");
    }
    break;

  case 5:
    if(zestaw_2_dioda1stan){

      zestaw_2_dioda1Wylacz();
      Serial.println("Dioda 1 wylaczona");

    }
    else{

      zestaw_2_dioda1Wlacz();
      Serial.println("Dioda 1 wlaczona");
    }

    break;

  case 6:
    if(zestaw_2_dioda2stan){

      zestaw_2_dioda2Wylacz();
      Serial.println("Dioda 2 wylaczona");

    }
    else{

      zestaw_2_dioda2Wlacz();
      Serial.println("Dioda 2 wlaczona");
    }
    break;

  case 7:
    if(zestaw_2_dioda3stan){

      zestaw_2_dioda3Wylacz();
      Serial.println("Dioda 3 wylaczona");

    }
    else{

      zestaw_2_dioda3Wlacz();
      Serial.println("Dioda 3 wlaczona");
    }
    break;

  case 8:
    if(zestaw_2_dioda4stan){

      zestaw_2_dioda4Wylacz();
      Serial.println("Dioda 4 wylaczona");

    }
    else{

      zestaw_2_dioda4Wlacz();
      Serial.println("Dioda 4 wlaczona");
    }
    break;

  case 9:
    // dodatkowa opcja led
    break;

  case 10:
    // ddatkowa opcja led
    break;

  case 11:
    Serial.print(DHT.temperature,1);
    break;

  case 12:
    Serial.print(DHT.humidity,1);
    break;

  case 13:

    Serial.print(DHT2.temperature,1);
    break;

  case 14:

    Serial.print(DHT2.humidity,1);
    break;

  case 15:

    Serial.println(potencjometr);
    break;

  case 16:

    Serial.println(fotorezystor);
    break;

  case 17:

    Serial.println(pojemnosc);
    break;

  case 18:

    break;

  case 19:

    break;

  case 20:

    break;
  }

}


void menu_kuchnia(){
  int o = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("4-Wstecz 6-Dalej");
  delay(1000);
  while(1){

    char key = keypad.getKey();

    if (key != NO_KEY){
      Serial.println(key);

      if(key == '4'){
        if(o>0){
          o--;
        }
        else{
          o=0;
        }

      }
      if(key == '6'){
        if(o<=6){
          o++;
        }
        else{
          o=6; 
        }
      }
      if(key == 'D'){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RS Home Witamy");
        break;
      }

      if(key == '5'){
        if(o ==0){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Kuch. Temp");
          lcd.setCursor(0, 1);
          DHT.read11(dht_dpin);
          lcd.print(DHT.temperature,1);
        }
        if(o ==1){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Kuch. Wilg");
          lcd.setCursor(0, 1);
          DHT.read11(dht_dpin);
          lcd.print(DHT.humidity,1);
        }
        if(o ==2){
          if(zestaw_1_dioda1stan){
            dioda1Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            dioda1Wlacz();
            Serial.println("Dioda 1 wlaczona");
          }
        }
        if(o ==3){
          if(zestaw_1_dioda2stan){
            dioda2Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            dioda2Wlacz();
            Serial.println("Dioda 1 wlaczona");
          }
        }
        if(o ==4){
          if(zestaw_1_dioda3stan){
            dioda3Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            dioda3Wlacz();
            Serial.println("Dioda 1 wlaczona");
          } 
        }
        if(o ==5){
          if(zestaw_1_dioda4stan){
            dioda4Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            dioda4Wlacz();
            Serial.println("Dioda 1 wlaczona");
          } 
        }
      }





      switch(o){

      case 0:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuch. Temp");
        lcd.setCursor(0, 1);
        lcd.print(DHT.temperature,1);
        break;
      case 1:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuch. Wilg");
        lcd.setCursor(0, 1);
        lcd.print(DHT.humidity,1);
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuch. Gl Zas");
        lcd.setCursor(0, 1);
        if(zestaw_1_dioda1stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }


        break;
      case 3:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuch. Swiatlo");
        lcd.setCursor(0, 1);
        if(zestaw_1_dioda2stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }
        break;
      case 4:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuch. Port 1");
        lcd.setCursor(0, 1);
        if(zestaw_1_dioda3stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }
        break;
      case 5:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuch. Port 2");
        lcd.setCursor(0, 1);
        if(zestaw_1_dioda4stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }
        break;
      case 6:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Koniec Menu");
        break;


      }//switch
    }//key != NO_KEY
  }//while
}

void menu_zew(){
  int o = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("4-Wstecz 6-Dalej");
  delay(1000);
  while(1){

    char key = keypad.getKey();

    if (key != NO_KEY){
      Serial.println(key);

      if(key == '4'){
        if(o>0){
          o--;
        }
        else{
          o=0;
        }

      }
      if(key == '6'){
        if(o<=7){
          o++;
        }
        else{
          o=7; 
        }
      }
      if(key == 'D'){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RS Home Witamy");
        break;
      }

      if(key == '5'){
        if(o ==1){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Zew. Temp");
          lcd.setCursor(0, 1);
          DHT2.read11(dht_dpin2);
          lcd.print(DHT2.temperature,1);
        }
        if(o ==2){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Zew. Wilg");
          lcd.setCursor(0, 1);
          DHT2.read11(dht_dpin2);
          lcd.print(DHT2.humidity,1);
        }

        if(o ==3){
          if(zestaw_2_dioda1stan){
            zestaw_2_dioda1Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            zestaw_2_dioda1Wylacz();
            Serial.println("Dioda 1 wlaczona");
          }
        }
        if(o ==4){
          if(zestaw_2_dioda2stan){
            zestaw_2_dioda2Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            zestaw_2_dioda2Wlacz();
            Serial.println("Dioda 1 wlaczona");
          }
        }
        if(o ==5){
          if(zestaw_2_dioda3stan){
            zestaw_2_dioda3Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            zestaw_2_dioda3Wylacz();
            Serial.println("Dioda 1 wlaczona");
          } 
        }
        if(o ==6){
          if(zestaw_2_dioda4stan){
            zestaw_2_dioda4Wylacz();
            Serial.println("Dioda 1 wylaczona");
          }
          else{
            zestaw_2_dioda4Wylacz();
            Serial.println("Dioda 1 wlaczona");
          } 
        }
      }





      switch(o){

      case 1:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew. Temp");
        lcd.setCursor(0, 1);
        lcd.print(DHT2.temperature,1);
        break;
      case 2:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew. Wilg");
        lcd.setCursor(0, 1);
        lcd.print(DHT2.humidity,1);
        break;
      case 3:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew. Gl Zas");
        lcd.setCursor(0, 1);
        if(zestaw_2_dioda1stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }


        break;
      case 4:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew. Swiatlo");
        lcd.setCursor(0, 1);
        if(zestaw_2_dioda2stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }
        break;
      case 5:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew. Port 1");
        lcd.setCursor(0, 1);
        if(zestaw_2_dioda3stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }
        break;
      case 6:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew. Port 2");
        lcd.setCursor(0, 1);
        if(zestaw_2_dioda4stan){

          lcd.print("Wl");
        }
        else{
          lcd.print("Wyl");
        }
        break;

      case 7:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Koniec Menu");
        break;


      }//switch
    }//key != NO_KEY
  }//while
}

void sprawdz_klawiature(){


  char key = keypad.getKey();
  char key2 = keypad.getKey();
  int i=0;
  if (key != NO_KEY){
    Serial.println(key);

    if(key == 'A')
    {
      if(dostep){

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kuchnia");
        lcd.setCursor(0, 1);
        lcd.print("Wczytuje...");
        klaw_menu = 1;
        delay(2000);
        menu_kuchnia();
      }
      else{
        lcd.setCursor(0, 1);
        lcd.print("Podaj Kod...");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RS Home");
      }
    }
    if(key == 'B')
    {
      if(dostep){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zew");
        lcd.setCursor(0, 1);
        lcd.print("Wczytuje...");
        klaw_menu = 2;
        delay(2000);
        menu_zew();
      }
      else{
        lcd.setCursor(0, 1);
        lcd.print("Podaj Kod...");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RS Home");
      }
    }
    if(key == 'C')
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Podaj Kod");

      while(dostep == false || key2 != 'D'){
        key2 = keypad.getKey();
        if (key2 != NO_KEY){
          lcd.setCursor(i, 1);
          lcd.print("*");
          if(i<=3){
            znaki[i]=key2;
            i++;
            if(i==4){
              if(znaki[0] ==kod[0] && znaki[1] ==kod[1] && znaki[2] ==kod[2] && znaki[3] ==kod[3] )
              {
                dostep = true;
                znaki[0]='\0';
                znaki[1]='\0'; 
                znaki[2]='\0'; 
                znaki[3]='\0';
                i=0;
              }
              else{
                Serial.println(" PODAJ GO PONOWNIE");
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("Zle, Powtorz");
                delay(2000);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Podaj Kod");
                znaki[0]='\0';
                znaki[1]='\0'; 
                znaki[2]='\0'; 
                znaki[3]='\0';
                i=0;
              }
            }
          }
          if(dostep == true){
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Kod Ok");
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("RS Home Witamy");
            break;
          }
          if(key2 == 'D'){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Anulowano");
            kod_anulowany = true;
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("RS Home");
            break;
          }

        }// if != NO_KEY
      }//while

    }// if key==C
    if(key == 'D')
    {
      Serial.println("ddd");
    }
    if(key == '5'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("5555");
    }


  }
}





void pir1(){
  if(digitalRead(pir_Pin) == HIGH){
    digitalWrite(pir_ledPin, HIGH); 
    if(pir_lockLow){  

      pir_lockLow = false;            
      Serial.println("PIR1 ");

    }         
    pir_takeLowTime = true;
  }

  if(digitalRead(pir_Pin) == LOW){       
    digitalWrite(pir_ledPin, LOW); 

    if(pir_takeLowTime){
      pir_lowIn = millis();       
      pir_takeLowTime = false;       
    }

    if(!pir_lockLow && millis() - pir_lowIn > pir_pause){  

      pir_lockLow = true;                        
      Serial.println("PIR 1 K ");    


    }
  } 
}

void pir2(){
  if(digitalRead(pir_Pin2) == HIGH){
    digitalWrite(pir_ledPin2, HIGH);  
    if(pir_lockLow2){  

      pir_lockLow2 = false;            
      Serial.println("PIR 2 ");

    }         
    pir_takeLowTime2 = true;
  }

  if(digitalRead(pir_Pin2) == LOW){       
    digitalWrite(pir_ledPin2, LOW); 

    if(pir_takeLowTime2){
      pir_lowIn2 = millis();       
      pir_takeLowTime2 = false;       
    }

    if(!pir_lockLow2 && millis() - pir_lowIn2 > pir_pause2){  

      pir_lockLow2 = true;                        
      Serial.println("PIR 2 K ");   
    }
  } 
}


void processRequest(EthernetClient* client) {
  client->flush();


  if (bufferRequest.startsWith("GET /d=1")) {
    sprawdz_wejscie(1);
    client->print("OK");

  } 
  else if (bufferRequest.startsWith("GET /d=2")) {
    sprawdz_wejscie(2);
    client->print("OK");

  } 
  else if (bufferRequest.startsWith("GET /d=3")) {
    sprawdz_wejscie(3);
    client->print("OK");
  } 
  else if (bufferRequest.startsWith("GET /d=4")) {
    sprawdz_wejscie(4);
    client->print("OK");
  } 
  else if (bufferRequest.startsWith("GET /d=5")) {
    sprawdz_wejscie(5);
    client->print("OK");
  } 
  else if (bufferRequest.startsWith("GET /d=6")) {
    sprawdz_wejscie(6);
    client->print("OK");
  }
  else if (bufferRequest.startsWith("GET /d=7")) {
    sprawdz_wejscie(7);
    client->print("OK");

  } 
  else if (bufferRequest.startsWith("GET /d=8")) {
    sprawdz_wejscie(8);
    client->print(digitalRead(13));
    client->print("OK");
  } 
  else if (bufferRequest.startsWith("GET /d=9")) {
    sprawdz_wejscie(9);
    client->print(digitalRead(13));
    client->print("OK");
  } 
  else if (bufferRequest.startsWith("GET /d=10")) {
    sprawdz_wejscie(10);
    client->print(digitalRead(13));
    client->print("OK");
  } 
  else if (bufferRequest.startsWith("GET /d=11")) {
    sprawdz_wejscie(11);
    client->print(DHT.temperature,1);

  } 
  else if (bufferRequest.startsWith("GET /d=12")) {
    sprawdz_wejscie(12);
    client->print(DHT.humidity,1);

  } 
  else if (bufferRequest.startsWith("GET /d=13")) {
    sprawdz_wejscie(13);
    client->print(DHT2.temperature,1);

  } 
  else if (bufferRequest.startsWith("GET /d=14")) {
    sprawdz_wejscie(14);
    client->print(DHT2.humidity,1);

  } 
  else if (bufferRequest.startsWith("GET /d=15")) {
    sprawdz_wejscie(15);
    client->print(potencjometr);

  } 
  else if (bufferRequest.startsWith("GET /d=16")) {
    sprawdz_wejscie(16);
    client->print(fotorezystor);

  } 
  else if (bufferRequest.startsWith("GET /d=17")) {
    sprawdz_wejscie(17);
    client->print(pojemnosc);

  }

  else if (bufferRequest.startsWith("GET /a=15")) {
    client->println(analogRead(15));
    client->println("");
    client->println("\n");
  } 
  else if (bufferRequest.startsWith("GET /a=14")) {
    client->print(analogRead(14));

  } 
  else if (bufferRequest.startsWith("GET /a=13")) {
    client->print(analogRead(13));

  } 
  else if (bufferRequest.startsWith("GET /a=12")) {
    client->print(analogRead(12));

  } 
  else if (bufferRequest.startsWith("GET /a=11")) {
    client->print(analogRead(11));

  } 
  else if (bufferRequest.startsWith("GET /a=10")) {
    client->print(analogRead(10));

  } 
  else if (bufferRequest.startsWith("GET /a=9")) {
    client->print(analogRead(9));

  } 
  else if (bufferRequest.startsWith("GET /a=8")) {
    client->print(analogRead(8));

  } 
  else if (bufferRequest.startsWith("GET /a=7")) {
    client->print(analogRead(7));

  } 
  else if (bufferRequest.startsWith("GET /a=6")) {
    client->print(analogRead(6));

  } 
  else if (bufferRequest.startsWith("GET /a=5")) {
    client->print(analogRead(5));

  } 
  else if (bufferRequest.startsWith("GET /a=4")) {
    client->print(analogRead(4));

  } 
  else if (bufferRequest.startsWith("GET /a=3")) {
    client->print(analogRead(2));

  } 
  else if (bufferRequest.startsWith("GET /a=2")) {
    client->print(analogRead(2));

  } 
  else if (bufferRequest.startsWith("GET /a=1")) {
    client->print(analogRead(1));

  } 
  else if (bufferRequest.startsWith("GET /a=0")) {
    client->print(analogRead(0));

  } 

}



void setup(){
  Serial.begin(9600);
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);

  pinMode(zestaw_1_diodaPin1, OUTPUT);
  pinMode(zestaw_1_diodaPin2, OUTPUT);
  pinMode(zestaw_1_diodaPin3, OUTPUT);
  pinMode(zestaw_1_diodaPin4, OUTPUT);

  pinMode(zestaw_2_diodaPin1, OUTPUT);
  pinMode(zestaw_2_diodaPin2, OUTPUT);
  pinMode(zestaw_2_diodaPin3, OUTPUT);
  pinMode(zestaw_2_diodaPin4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(RedPin, OUTPUT); 
  pinMode(GreenPin, OUTPUT); 
  pinMode(BluePin, OUTPUT); 

  pinMode(buzzerPin, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);

  digitalWrite(RedPin, HIGH);
  digitalWrite(GreenPin, LOW);
  digitalWrite(BluePin, LOW);


  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kalibracja sys.");
  Serial.print("Kalibracja systemu");

  pinMode(pir_Pin, INPUT);
  pinMode(pir_ledPin, OUTPUT);
  digitalWrite(pir_Pin, LOW);
  pinMode(pir_Pin2, INPUT);
  pinMode(pir_ledPin2, OUTPUT);
  digitalWrite(pir_Pin2, LOW);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");

  }
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();

  Udp.begin(localPort);
  
  for(int i = 0; i < pir_calibrationTime; i++){
    Serial.print(".");
    lcd.setCursor(i, 1);
    lcd.print(".");
    delay(1000);
  }


  Serial.println("done");
  Serial.println("System Aktywny");
  delay(50);
  lcd.clear();
  lcd.setCursor(0, 0);

  digitalWrite(RedPin, LOW);
  digitalWrite(GreenPin, LOW);
  digitalWrite(BluePin, LOW);
  lcd.print("RS Home");
  Serial.write("RS Home");
}

void loop(){

  czas = millis()/1000;
  timedAction1.check();
  sprawdz_klawiature();
  pir1();
  pir2();

  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        bufferRequest.concat(c);

        if (c == '\n') {
          processRequest(&client);
          Serial.println(bufferRequest);
          break;
        }
      }
    }

    bufferRequest = "";
    delay(10);
    client.stop();
  }
  sprawdz_klawiature();
  if(poczta ==true){
    Serial.print("poczta");
    digitalWrite(RedPin, HIGH);
    digitalWrite(GreenPin, HIGH);
    digitalWrite(BluePin, LOW);
  }
  else{
    digitalWrite(RedPin, LOW);
    digitalWrite(GreenPin, LOW);
    digitalWrite(BluePin, LOW);
  }



 int packetSize = Udp.parsePacket();

if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    Serial.println(packetBuffer[0]);
  
    if(packetBuffer[0]=='1' && packetBuffer[1]=='1'){
       dane = DHT.temperature;
    }else if(packetBuffer[0]=='1' && packetBuffer[1]=='2'){
       dane = DHT.humidity;
      
    }else if(packetBuffer[0]=='1' && packetBuffer[1]=='3'){
       dane = DHT2.temperature;
    }else if(packetBuffer[0]=='1' && packetBuffer[1]=='4'){
      dane = DHT2.humidity;
       
    }
    Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
    Udp.write(dane);
    Udp.endPacket();
    Serial.print("UDP: ");
    Serial.println(dane);
  }          
          
          
}//loop


void sprawdz_temp(){
  DHT.read11(dht_dpin);
  DHT2.read11(dht_dpin2);
  foto_poto();
  oblicz_pojemnosc();
  iSensorValue = analogRead(A0);
  if(iSensorValue > 60){
    digitalWrite(buzzerPin, LOW);
  }
  Serial.println(iSensorValue);
}


