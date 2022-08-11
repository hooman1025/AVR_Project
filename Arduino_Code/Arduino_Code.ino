
/**********헤더파일 추가 및 변수 선언***********/
#include <Wire.h>
#include <MsTimer2.h>
#include <Key.h>
#include <Keypad.h>
#include <SPI.h>
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <LiquidCrystal_I2C.h>

//SPI핀 -- RFID
#define  RST_PIN 19
#define  SS_PIN 10

//서보 변수
String Door = "close";
int emerg_state = 0;

//LCD 선언
LiquidCrystal_I2C lcd(0x27, 16, 2);  //보안 LCD


/***************키패드 기능 선언***************/
const byte ROWS = 4;  // 행의 개수
const byte COLS = 4;  // 열의 개수

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 5, 4, 3, 2 };
// R1, R2, R3, R4의 차례로 R1~R4와 연결된 디지털 핀번호
byte colPins[COLS] = {6, 7, 8, 9 };
// C1, C2, C3, C4의 차례로 C1~C4와 연결된 디지털 핀번호

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// Keypad 라이브러리를 설정 (배열값, 행의 핀, 열의 핀, 행 갯수, 열 갯수)

//패스워드 설정
char pw[5] = {'1', '2', '3', '4'};


/***************RFID 변수 선언****************/
byte user[4] = {'\0'};
int flag = 0;
int cnt = 0;
int tru = 0;

MFRC522 mfrc(SS_PIN, RST_PIN);
//RFID 라이브러리를 설정


/***************Timer****************/
String Timer()
{
  String Info = Door + "#" + (String)emerg_state + "L";
  //soft.print(Info);
  Serial.print(Info);
}


/***********포트 선언 및 초기 값 설정***********/
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();

  lcd.init();
  lcd.backlight();

  MsTimer2::set(1000, Timer);
  MsTimer2::start();
}

/******************메인 루프******************/
void loop()
{
  //LCD 초기 설정
  lcd.setCursor(1, 0);
  lcd.print("MY");
  lcd.setCursor(1, 1);
  lcd.print("HOME");

  /***************보안****************/
  char statekey = kpd.getKey();
  if (statekey == 'A')
  {
    //Serial.println(statekey);
    lcd.clear();
    int pwcheck = pw_check();
    if (pwcheck == 0)
    {
      Su();
      Door = "open";
      //문열림
      delay(5000);
      Door = "close";
    }
    else
    {
      Fa();
    }
  }
  else if (statekey == 'B')
  {
    //Serial.println(statekey);
    lcd.clear();
    int rfidcheck = rfid_check();
    if (rfidcheck == 0)
    {
      Su();
      Door = "open";
      //문열림
      delay(3000);
      Door = "close";
    }
    else if (rfidcheck == 1)
    {
      Fa();
    }
    else
    {
      //Serial.print("등록완료");
    }
  }
  else if (statekey == 'C')
  {
    //emerg_state = 1;
    emerg_state = 0;
    Door = "close";
  }
  else if (statekey == 'D')
  {
    emerg_state = 1;
  }
}

/***************RFID Check******************/
int rfid_check()
{
  while (1)
  {
    lcd.setCursor(0, 0);
    lcd.print("Tag RFID");
    if (mfrc.PICC_IsNewCardPresent() && mfrc.PICC_ReadCardSerial())
    {
      //      Serial.print("Card UID:");
      //      for (byte i = 0; i < 4; i++)
      //      {
      //        Serial.print(mfrc.uid.uidByte[i]);
      //        Serial.print(" ");
      //      }
      //      Serial.println();

      if (mfrc.uid.uidByte[0] == user[0] &&
          mfrc.uid.uidByte[1] == user[1] &&
          mfrc.uid.uidByte[2] == user[2] &&
          mfrc.uid.uidByte[3] == user[3])
      {
        //Serial.println("Door Open");
        delay(500);
        return 0;
      }
      else {
        //Serial.println("Access denied");
        delay(500);
        return 1;
      }
    }



    char customkey = kpd.getKey();
    if (customkey)
    {
      //Serial.println(customkey);
      // 시리얼 모니터에 한줄씩 key값을 출력

      if (customkey == 'D')
      {
        lcd.clear();
        int pwcheck = pw_check();
        if (pwcheck == 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("New_user_TAG");
          delay(5000);

          if (mfrc.PICC_IsNewCardPresent() &&
              mfrc.PICC_ReadCardSerial())
          {
            for (int i = 0; i < 4; i++)
            {
              user[i] = mfrc.uid.uidByte[i];
            }
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Register_success");
            delay(2000);
            lcd.clear();
            return 2;
          }
          else
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Register_fail");
            delay(2000);
            lcd.clear();
            return 2;
          }
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Register_fail");
          delay(2000);
          lcd.clear();
          return -1;
        }
      }
    }

  }
}


/***************PW Check******************/

int pw_check()
{
  while (1)
  {

    if (flag == 0)
    {

      lcd.setCursor(1, 0);
      lcd.print("PASSWORD");
      lcd.setCursor(0, 1);
      char key = kpd.getKey();

      if (key == '*')
      {
        flag = 1;
      }
    }

    else if (flag == 1)
    {
      lcd.setCursor(10, 0);
      lcd.print("INPUT");
      char key2 = kpd.getKey();
      if (key2)
      {
        if (key2 == pw[cnt])
        {
          lcd.setCursor(cnt, 1);
          lcd.print("*");
          cnt++;
          tru++;
        }
        else if (key2 != pw[cnt])
        {
          lcd.setCursor(cnt, 1);
          lcd.print("*");
          cnt++;
        }
      }
      if (cnt == 4)
        flag = 2;
    }

    else if (flag == 2)
    {
      char key = kpd.getKey();

      if (key == '*')
      {
        if (tru == 4) {
          flag = 0;
          tru = 0;
          cnt = 0;
          return 0;//성공함수
        }

        else {
          flag = 0;
          tru = 0;
          cnt = 0;
          return 1;//실패함수
        }
      }
    }
  }
}

/****************함수 선언****************/
void Su()//성공했을시 성공메세지 보내기
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("PASSWORD");
  lcd.setCursor(0, 1);
  lcd.print("access");
  delay(2000);
  lcd.clear();
}

void Fa()//실패했을시 실패메세지 보내기
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("PASSWORD");
  lcd.setCursor(0, 1);
  lcd.print("denied");
  delay(2000);
  lcd.clear();
}

void serialEvent() {
  if (Serial.available()) {
    char inchar = (char)Serial.read();
    //inputString += inchar;
    if (inchar == 'E') {
      Serial.flush();
      emerg_state = 1;
      Door = "open";
    }
  }
}
