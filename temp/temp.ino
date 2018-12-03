// DHT11 센서를 쉽게 제어하기 위한 라이브러리를 추가해줍니다.
#include "DHT.h"
 
// I2C LCD를 쉽게 제어하기 위한 라이브러리를 추가해줍니다.
//#include <LiquidCrystal_I2C.h>
 
// 온습도 센서를 디지털 2번 핀에 연결합니다.
#define DHTPIN 2
#define DHTTYPE DHT11
 
//#define  KLCD_NUL           0x00                     // Null
#define  KLCD_HOME          0x01                     // Cursor Home
#define  KLCD_CLSTART       0x02                     // Cursor Move to Line Start
#define  KLCD_CRIGHT        0x03                     // Cursor Move to Right
#define  KLCD_CLEFT         0x04                     // Cursor Move to Left
#define  KLCD_CUP           0x05                     // Cursor Move to Up
#define  KLCD_CDOWN         0x06                     // Cursor Move to Down
//#define  KLCD_BELL          0x07                     //
#define  KLCD_NORMAL        0x08                     // Normal Screen Forground : Black / Background : White
#define  KLCD_REVERSE       0x09                     // Invert Screen Forground : White / Backgriund : Black   
//#define  KLCD_LINEFEED      0x0A                     // Line Feed                               
#define  KLCD_CLEAR         0x0B                     // Clear Screen with Cursor Home
//#define  KLCD_ASCII_0C      0x0C                
#define  KLCD_ENTER         0x0D                     // Carriage Return                 
#define  KLCD_LOCATE        0x0E                                                    
//#define  KLCD_ASCII_0F      0x0F

//int season=0;//0 - 겨울, 1 - 여름
int   count;
float value;
String stringOne, stringTwo, stringThree;
 
void klcd(unsigned int v) {
//  1 바이트의 영문자/숫자/제어코드 또는 2 바이트로 구성되는 한글코드(KS5601-1987)를
//  LCD화면에 출력합니다.
 unsigned char d;
 if (v > 0xFF) { d = (v>>8) & 0xFF;   Serial.write(d); }
 d = v & 0xFF; Serial.write(d);
}
 
void klcd_locate(unsigned char r, unsigned char c) {
//  커서 위치를 설정합니다. 행(r)과 열(c)을 지정합니다 행은 0-3 범위이고 열은 0-15 범위의 숫자입니다.
  unsigned char t;
  Serial.write(KLCD_LOCATE);
  t = ((r<<5) | (c & 0x1F));
  Serial.write(t);
}
 
DHT dht(DHTPIN, DHTTYPE);
 
// 16X2 크기의 LCD 객체를 생성합니다.
// 만약 LCD 화면이 나오지 않으면 0x3F 대신 0x27를 넣어주세요.
//LiquidCrystal_I2C lcd(0x3F, 16, 2);
 
void setup() {
  Serial.begin(9600);
  dht.begin();
 
  klcd(KLCD_CLEAR);                  // Clear Screen
  klcd(KLCD_NORMAL);                 // Normal Screen
   
}
void tempchk(int season, int tempint){
  if(season == 0){
    klcd(0xBFA9);   //    "여"
    klcd(0xB8A7);   //    "름"
    klcd(0xC3B6);   //    "철"
    Serial.print(": ");
    if(tempint<26){
       klcd(0xB3B6);   //    "낭"
       klcd(0xBAF1);   //    "비"
       klcd(0xC1DF);   //    "중"
    }
    else if(tempint == 26){
       klcd(0xC0FB);   //    "적"
       klcd(0xC1A4);   //    "정"
    }
    else{
       klcd(0xC0FD);   //    "절"
       klcd(0xBEE0);   //    "약"
       klcd(0xC1DF);   //    "중"
    }
  }
  else{
    klcd(0xB0DC);   //    "겨"
    klcd(0xBFEF);   //    "울"
    klcd(0xC3B6);   //    "철"
    Serial.print(": ");
    if(tempint>20){
       klcd(0xB3B6);   //    "낭"
       klcd(0xBAF1);   //    "비"
       klcd(0xC1DF);   //    "중"
    }
    else if(tempint == 20){
       klcd(0xC0FB);   //    "적"
       klcd(0xC1A4);   //    "정"
    }
    else{
       klcd(0xC0FD);   //    "절"
       klcd(0xBEE0);   //    "약"
       klcd(0xC1DF);   //    "중"
    }
  }
}
void loop() {
  // 습도와 온도값을 측정하고, 제대로 측정되었는지 확인해줍니다.
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
 
  if (isnan(humidity) || isnan(temperature) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  klcd(KLCD_CLEAR);                  // Clear Screen
  klcd(KLCD_NORMAL);                 // Normal Screen

  Serial.print("Energy Saver");
  Serial.println();                // New Line
  //temperature = 18; // test
  //humidity = 20;  // test
  // 온도와 습도값을 시리얼 모니터에 출력해 줍니다.
  klcd(0xC7F6);    //  "현"
  klcd(0xC0E7);    //  "재"
  klcd(0xBFC2);    //  "온"
  klcd(0xB5B5);    //  "도"
  klcd('=');
  Serial.print(" ");
  Serial.print((int)temperature); Serial.print("*C");
  Serial.println();                // New Line
  klcd(0xC7F6);    //  "현"
  klcd(0xC0E7);    //  "재"
  klcd(0xBDC0);    //  "습"
  klcd(0xB5B5);    //  "도"
  klcd('=');
  Serial.print(" ");
  Serial.print((int)humidity); Serial.println("%");
  //Serial.println();                // New Line
  
  if((int)humidity>=40){
    tempchk(0,(int)temperature);
  }
  else{
    tempchk(1,(int)temperature);
  }
  
  
/*
  // LCD에 출력할 습도 메세지를 만듭니다.
  String humi = "Humi : ";
  humi += (String)humidity;
  humi += "%";
 
  // LCD에 출력할 온도 메세지를 만듭니다.
  String temp = "temp : ";
  temp += (String)temperature;
  temp += "C";
 
  // 첫번째 줄, 첫번째 칸부터 "Humi = 000%" 를 출력해 줍니다.
  lcd.setCursor(0, 0);
  lcd.print(humi);
 
  // 두번째 줄, 첫번째 칸부터 "temp = 000C" 를 출력해 줍니다.
  lcd.setCursor(0, 1);
  lcd.print(temp);
*/
  delay(7000);
}

