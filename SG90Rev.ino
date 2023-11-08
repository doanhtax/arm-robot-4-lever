#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>      // Thư viện điều khiển servo
// Khai báo đối tượng myservo dùng để điều khiển servo
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
int servoPin_1 = 2;       // Khai báo chân điều khiển servo1
int servoPin_2 = 3;       // Khai báo chân điều khiển servo2
int servoPin_3 = 4;       // Khai báo chân điều khiển servo3
int servoPin_4 = 5;       // Khai báo chân điều khiển servo4


byte mang[4];
RF24 radio(8, 9);  // Chân CE và CSN của module NRF24L01
const byte diachi[6] = "12345"; // Địa chỉ ống nhận dữ liệu
void setup() {
  Serial.begin(9600);
  if (!radio.begin())
  {
    Serial.println("Module không khởi động được");
    while (1) {}
  }
  radio.openReadingPipe(0, diachi);
  radio.setPALevel(RF24_PA_MIN);  // Cấu hình mức công suất tín hiệu (tuỳ chọn RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX)
  radio.setChannel(80);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();//Cào đặt module là RX;
  if (!radio.available())
  {
    Serial.println("Chưa kết nối được với TX ...");
    Serial.println("Chờ kết nối ...");
  }
  myservo1.attach(servoPin_1);
  myservo2.attach(servoPin_2);
  myservo3.attach(servoPin_3);
  myservo4.attach(servoPin_4);
}

void loop() {
  if (radio.available())
  {
    radio.read(&mang, sizeof(mang));
//    Serial.print("Sevor1: "); Serial.println(mang[4]);
   
  }
  myservo1.write(mang[0]);
  myservo2.write(mang[1]);
  myservo3.write(mang[2]);
  myservo4.write(mang[3]);

}
