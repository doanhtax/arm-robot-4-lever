#include "Arduino.h"
#include "EEPROM.h"
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);  // Chân CE và CSN của module NRF24L01
LiquidCrystal_I2C lcd(0x3F, 20, 4);
const byte diachi[6] = "12345"; // Địa chỉ ống nhận dữ liệu
/*
  Servo A-8; B-9; Z-10; Kep-11; K
  Bien tro: A-A0; B-A1; Z-A2
  Nut nhan - 12

  Menu: len-7; xuong-6; enter-5
*/

#define enter 4
#define len 5
#define xuong 6
byte gt_len; byte gt_xuong; byte gt_enter;
int dem_menu = 0; int dem_lenxuong = 0;
int macdinh1 = 1; int macdinh2 = 1; int macdinh3 = 1;
byte mang[4];



#define nutnhan 3
#define bientro_A A0// sevor2
#define bientro_B A2// sevor4
#define bientro_Z A1// sevor3
#define bientro_G A3// sevor4
int gt_bientro_A; int gt_bientro_B; int gt_bientro_Z;int gt_bientro_G;
int gt_biendoi_A;
int gt_nutnhan;
int biendem = 0;
int macdinh = 1;

unsigned int vitri = 1;

unsigned int diachi_A = 6; unsigned int diachi_B = 7; unsigned int diachi_Z = 8; unsigned int diachi_Kep = 9;
unsigned int diachi_cuoi;

int batdau = 10;
int ketthuc;

unsigned long time = 0;
unsigned long dem_chay = 1; boolean run;

void HT_goc(int hang)
{
  if (mang[1] < 10)
  {
    lcd.setCursor(0, hang); lcd.print("A:");
    lcd.setCursor(2, hang); lcd.print(mang[1]);
    lcd.setCursor(3, hang); lcd.print("  ");
  }
  else if (mang[1] < 100)
  {
    lcd.setCursor(0, hang); lcd.print("A:");
    lcd.setCursor(2, hang); lcd.print(mang[1]);
    lcd.setCursor(4, hang); lcd.print(" ");
  }
  else
  {
    lcd.setCursor(0, hang); lcd.print("A:");
    lcd.setCursor(2, hang); lcd.print(mang[1]);
  }

  if (mang[2] < 10)
  {
    lcd.setCursor(6, hang); lcd.print("B:");
    lcd.setCursor(8, hang); lcd.print(mang[2]);
    lcd.setCursor(9, hang); lcd.print("  ");
  }
  else if (mang[2] < 100)
  {
    lcd.setCursor(6, hang); lcd.print("B:");
    lcd.setCursor(8, hang); lcd.print(mang[2]);
    lcd.setCursor(10, hang); lcd.print(" ");
  }
  else
  {
    lcd.setCursor(6, hang); lcd.print("B:");
    lcd.setCursor(8, hang); lcd.print(mang[2]);
  }

  if (mang[3] < 10)
  {
    lcd.setCursor(11, hang); lcd.print("Z:");
    lcd.setCursor(13, hang); lcd.print(mang[3]);
    lcd.setCursor(14, hang); lcd.print("  ");
  }
  else if (mang[3] < 100)
  {
    lcd.setCursor(11, hang); lcd.print("Z:");
    lcd.setCursor(13, hang); lcd.print(mang[3]);
    lcd.setCursor(15, hang); lcd.print(" ");
  }
  else
  {
    lcd.setCursor(11, hang); lcd.print("Z:");
    lcd.setCursor(13, hang); lcd.print(mang[3]);
  }
  if (mang[0] == 40)
  {
    lcd.setCursor(17, hang); lcd.print("K:");
    lcd.setCursor(19, hang); lcd.print("1");
  }
  else {
    lcd.setCursor(17, hang); lcd.print("K:");
    lcd.setCursor(19, hang); lcd.print("0");
  }
}

void Xem_lenh()
{
  Serial.print("1="); Serial.print(EEPROM.read(1));
  Serial.print("  - CUOI="); Serial.println(EEPROM.read(2));
  for (int i = 10; i < 30; i++)
  {
    Serial.print(i); Serial.print("="); Serial.print(EEPROM.read(i)); Serial.print("; ");
  }
}

void menu()
{
  if (dem_lenxuong == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">BACK");
    lcd.setCursor(0, 1);
    lcd.print(" THEM LENH");
    lcd.setCursor(0, 2);
    lcd.print(" XOA LENH");
    lcd.setCursor(0, 3);
    lcd.print(" CHAY LENH");
  }
  else if (dem_lenxuong == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(">THEM LENH");
    lcd.setCursor(0, 2);
    lcd.print(" XOA LENH");
    lcd.setCursor(0, 3);
    lcd.print(" CHAY LENH");
  }
  else if (dem_lenxuong == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(" THEM LENH");
    lcd.setCursor(0, 2);
    lcd.print(">XOA LENH");
    lcd.setCursor(0, 3);
    lcd.print(" CHAY LENH");
  }
  else if (dem_lenxuong == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(" THEM LENH");
    lcd.setCursor(0, 2);
    lcd.print(" XOA LENH");
    lcd.setCursor(0, 3);
    lcd.print(">CHAY LENH");
  }
}

void dk_servo()
{
  if (gt_nutnhan != macdinh)
  {
    if (gt_nutnhan == 0)
    {
      biendem += 1;
    }
    macdinh = gt_nutnhan;
  }
  if (biendem % 2 == 0) mang[0] = 10; //Kep
  else  mang[0] = 40; //Mo

  gt_bientro_A = analogRead(bientro_A); //Doc bien tro A 
  if (  gt_bientro_A >= 390 &&  gt_bientro_A <= 470){
  gt_biendoi_A = map(gt_bientro_A, 390, 470, 0, 140); 
  mang[1] =  gt_biendoi_A; 
  }

  gt_bientro_B = analogRead(bientro_B);
  if (gt_bientro_B >= 570)
  {
    mang[2] = 40;

  }
  else
  {
    mang[2] = 0;

  }
  gt_bientro_Z = analogRead(bientro_Z);
  gt_bientro_G = analogRead(bientro_G);
  if ( gt_bientro_Z >= 500) {
    mang[3] = 180;

  }
  else if ( gt_bientro_G >= 500)
  {
    mang[3] = 0;
  }
 
  else {
    mang[3] = 90;

  }
 
  // gửi dữ liệu đi
  radio.write(&mang, sizeof(mang));
  delay (50);
}

bool KT_co_lenh()
{
  boolean co;
  if (EEPROM.read(1) == 1) //Da co lenh duoc luu
    co = true;
  else
    co = false;
  return co;
}

void man_hinh()
{
  lcd.setCursor(2, 2);
  lcd.print("TRUONG CAO DANG");
  lcd.setCursor(1, 3);
  lcd.print("KY THUAT CAO THANG");
}

void xoa_lenh()
{
  if (KT_co_lenh() == true) //Co LENH
  {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("DANG XOA LENH...");
    lcd.setCursor(4, 2);
    lcd.print("VUI LONG CHO!");

    dem_menu = 0; dem_lenxuong = 0;

    for (int i = 0 ; i < EEPROM.length() ; i++)
    {
      EEPROM.write(i, 0);
    }

    // Xem_lenh();

    delay(3000);

    lcd.clear();
    man_hinh();
  }
  else //Hien khong co lenh trong EEPROM
  {
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("KHONG CO LENH NAO");
    lcd.setCursor(4, 2);
    lcd.print("DUOC LUU...!");

    dem_menu = 0; dem_lenxuong = 0;

    delay(3000);
    lcd.clear();
    man_hinh();
  }

}

void Them_lenh()
{
  if (KT_co_lenh() == true) //Da co lenh duoc luu
  {
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("DA CO LENH...!");
    delay(5000);
    lcd.clear();
    man_hinh();
    dem_menu = 0; dem_lenxuong = 0;
  }
  else //Chua co lenh nao duoc luu
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("XIN MOI NHAP LENH");
    lcd.setCursor(4, 1);
    lcd.print("VI TRI: ");
    lcd.setCursor(12, 1);
    lcd.print(vitri);
  }
}

void luu_lenh()
{
  //6,7,8,9 ; 10,11,12,13 ; 14,15,16,17 ; 18,19,20,21
  diachi_A += 4;
  diachi_B += 4;
  diachi_Z += 4;
  diachi_Kep += 4;

  EEPROM.write(diachi_A, mang[1]);
  EEPROM.write(diachi_B, mang[2]);
  EEPROM.write(diachi_Z, mang[3]);
  EEPROM.write(diachi_Kep, mang[0]);
  delay(50);
}

void chay_lenh()
{
  if (KT_co_lenh() == true) //Neu co lenh
  {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("CHUONG TRINH");
    lcd.setCursor(6, 1);
    lcd.print("DANG CHAY");

    byte doc_A; byte doc_B; byte doc_Z; byte doc_kep;
    ketthuc = EEPROM.read(2); //doc o nho cuoi cung
    while (run == true)
    {
      time = millis();
      gt_enter = digitalRead(enter);
      if (gt_enter == 1 && dem_menu == 2 && dem_lenxuong == 3)
      {
        while (batdau < ketthuc)
        {
          gt_enter = digitalRead(enter);
          if (millis() - time > 1500 && gt_enter == 1)
          {
            HT_goc(2);
            doc_A = EEPROM.read(batdau);
            doc_B = EEPROM.read(batdau + 1);
            doc_Z = EEPROM.read(batdau + 2);
            doc_kep = EEPROM.read(batdau + 3);

            mang[1] = doc_A;
            mang[2] = doc_B;
            mang[3] = doc_Z;
            mang[0] = doc_kep;
            // gửi dữ liệu đi
            radio.write(&mang, sizeof(mang));
            delay (50);


            Serial.print("Dem_menu: "); Serial.print(dem_menu); Serial.print(" | GT_ENTER: "); Serial.print(gt_enter);
            Serial.print("  - Time: "); Serial.print(time); Serial.print(" | Bat dau: "); Serial.print(batdau);
            Serial.print("  - A: "); Serial.print(doc_A); Serial.print("  - B: ");
            Serial.print(doc_B); Serial.print("  - Z: "); Serial.print(doc_Z);
            Serial.print("  - KEP: "); Serial.println(doc_kep);

            time = millis();
            batdau += 4;
          }
          else if (gt_enter == 0)
          {
            run = false;
            break;
          }
        }
      }
      else if (gt_enter == 0 && dem_menu == 2 && dem_lenxuong == 3)
      {
        run = false;
        break;
      }

      batdau = 10;
    }
  }
  else //KHONG CO LENH NAO DUOC LUU
  {
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("KHONG CO LENH NAO");
    lcd.setCursor(4, 2);
    lcd.print("DUOC LUU...!");

    dem_menu = 0; dem_lenxuong = 0;

    delay(3000);
    lcd.clear();
    man_hinh();
  }
}



void setup()
{

  if (!radio.begin())
  {

    Serial.println("Module không khởi động được");
    while (1) {}
  }
  radio.openWritingPipe(diachi);
  radio.setPALevel(RF24_PA_MIN);  // Cấu hình mức công suất tín hiệu (tuỳ chọn RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX)
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();//Cài đặt module là TX

  //
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  man_hinh();



  pinMode(bientro_A, INPUT);
  pinMode(bientro_B, INPUT);
  pinMode(bientro_Z, INPUT);
  pinMode(nutnhan, INPUT_PULLUP);

  pinMode(len, INPUT_PULLUP);
  pinMode(xuong, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);
}

void loop()
{
  gt_nutnhan = digitalRead(nutnhan);
  gt_enter = digitalRead(enter);
  gt_len = digitalRead(len);
  gt_xuong = digitalRead(xuong);

  if (gt_enter != macdinh1) //Nut ENTER
  {
    if (gt_enter == 0) dem_menu += 1;

    if (dem_menu == 1) menu(); //Vao menu chinh

    if (dem_menu == 2 && dem_lenxuong == 0) //Chon BACK ve man hinh
    {
      lcd.clear();
      man_hinh();
      dem_menu = 0; dem_lenxuong = 0;
    }

    if (dem_menu == 2 && dem_lenxuong == 2) //Chon xoa lenh
    {
      xoa_lenh();
    }

    if (dem_menu == 2 && dem_lenxuong == 1) //Chon them lenh moi
    {
      Them_lenh();
    }

    if (dem_menu == 3 && dem_lenxuong == 1) //Thoat khoi THEM LENH
    {
      EEPROM.write(1, 1); //Luu so 1 vao o 1 de xac nhan da co lenh duoc luu
      diachi_cuoi = diachi_Kep;
      EEPROM.write(2, diachi_cuoi); //Luu dia chi cuoi cung vao o so 2
      // Serial.println(EEPROM.read(2));

      lcd.clear();
      man_hinh();
      dem_menu = 0;
      dem_lenxuong = 0;
      vitri = 1;
      diachi_A = 6; diachi_B = 7; diachi_Z = 8; diachi_Kep = 9;
    }

    if (dem_menu == 2 && dem_lenxuong == 3) //Chon CHAY LENH
    {
      run = true;
    }

    if (dem_menu == 3 && dem_lenxuong == 3)
    {
      run = false;
      batdau = 10;
      ketthuc = 0;
      dem_chay = 0; dem_menu = 0; dem_lenxuong = 0;
      lcd.clear();
      man_hinh();
    }

    macdinh1 = gt_enter;
  }

  if (gt_len != macdinh2) //NUT LEN
  {
    if (gt_len == 0 && dem_menu == 1) //Di chuyen o MENU
    {
      dem_lenxuong -= 1;
      if (dem_lenxuong < 0)  dem_lenxuong = 3;
      menu();
    }

    if (gt_len == 0 && dem_menu == 2) //Neu o MENU them lenh thi nhan nut len de them VI TRI cua lenh
    {
      luu_lenh();
      vitri += 1;
      lcd.setCursor(12, 1);
      lcd.print(vitri);

      // Serial.print("DC A: "); Serial.print(diachi_A); Serial.print("  - DC B: "); Serial.print(diachi_B);
      // Serial.print("  - DC Z: "); Serial.print(diachi_Z); Serial.print("  - DC KEP: "); Serial.println(diachi_Kep);

      // Xem_lenh();
    }

    macdinh2 = gt_len;
  }

  if (gt_xuong != macdinh3) //NUT XUONG
  {
    if (gt_xuong == 0 && dem_menu == 1) //Di chuyen o MENU
    {
      dem_lenxuong += 1;
      if (dem_lenxuong > 3) dem_lenxuong = 0;
      menu();
    }
    macdinh3 = gt_xuong;
  }

  if (dem_menu == 0 && dem_lenxuong == 0) //Tai MAN HINH
  {
    dk_servo();
    HT_goc(0);
  }
  else if (dem_menu == 2 && dem_lenxuong == 1) //Tai MENU THEM LENH
  {
    dk_servo();
    HT_goc(2);
  }

  if (run == true && dem_menu == 2 && dem_lenxuong == 3) //CHAY LENH
  {
    chay_lenh();
  }

  // Serial.print("BT_A: "); Serial.print(gt_bientro_A); Serial.print(" - GOC A: "); Serial.print(goc_A);
  // Serial.print("  | BT_B: "); Serial.print(gt_bientro_B); Serial.print(" - GOC B: "); Serial.print(goc_B);
  // Serial.print("  | BT_Z: "); Serial.print(gt_bientro_Z); Serial.print(" - GOC Z: "); Serial.print(goc_Z);
  // Serial.print("  | biendem: "); Serial.print(biendem);

  Serial.print("Len: "); Serial.print(gt_len); Serial.print("  - Xuong: "); Serial.print(gt_xuong); Serial.print("  - Enter: "); Serial.print(gt_enter);
  Serial.print(" | Dem_MENU: "); Serial.print(dem_menu); Serial.print(" - Dem_LX: "); Serial.print(dem_lenxuong);
  Serial.print(" - RUN: "); Serial.print(run);
  Serial.print(" - BatDau: "); Serial.print(batdau); Serial.print(" - KetThuc: "); Serial.print(ketthuc);
  Serial.print(" - TIME: "); Serial.print(time);
  Serial.print(" - MILLIS: "); Serial.print(millis());

  Serial.println();
  delay(30);
}
