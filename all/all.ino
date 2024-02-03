#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESP32_Servo.h>

//firebase
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define API_KEY "AIzaSyD-b2ypEVgtn_kFwldA5lXokhG5zbzSWTs"
#define DATABASE_URL "smart-access-2316b-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "Test@gmail.com"
#define USER_PASSWORD "testtest"
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#define SS_PIN 5    // ESP32 pin GIOP5
#define RST_PIN 27  // ESP32 pin GIOP27
#define servoPin 4
#define door_IR 14

//Wifi設定
const char* ssid = "CCP";
const char* password = "0900752812";

// const char* ssid = "landis_c217_2.4g";
// const char* password = "landisc217";

unsigned long previousMillis = 0;
const long interval = 5000;  // 5 秒

WiFiServer server(80);
int door = 0, i = 0;
bool door_open = fbdo.boolData();
int door_RFID = 0;
int count = 0;

MFRC522 rfid(SS_PIN, RST_PIN);
// byte keyTagUID[4] = { 0x34, 0xDC, 0x97, 0xB8 };
byte keyTagUID[4] = { 0x25, 0x85, 0xAC, 0x4A };

Servo door_servo;

void setup() {
  Serial.begin(9600);
  door_servo.attach(servoPin);
  int pos = 0;

  Wifi_setup();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;

  pinMode(door_IR, INPUT);

  SPI.begin();      // init SPI bus
  rfid.PCD_Init();  // init MFRC522
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
  door_servo.write(180);

  Firebase.RTDB.getBool(&fbdo, "door_open");  //讀取網頁門開關
  if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_boolean) {
    door_open = fbdo.to<int>();
    // Serial.print(door_open);
  }
  door = digitalRead(door_IR);
  Firebase.RTDB.setBool(&fbdo, "door_IR", door);

  if (door_open == 1)
    open();

  //
  if (rfid.PICC_IsNewCardPresent()) {  // new tag is available
    if (rfid.PICC_ReadCardSerial()) {  // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      if (rfid.uid.uidByte[0] == keyTagUID[0] && rfid.uid.uidByte[1] == keyTagUID[1] && rfid.uid.uidByte[2] == keyTagUID[2] && rfid.uid.uidByte[3] == keyTagUID[3]) {
        open();
      } else {
        Serial.print("Access denied, UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

      rfid.PICC_HaltA();       // halt PICC
      rfid.PCD_StopCrypto1();  // stop encryption on PCD
    }
  }
}

void Wifi_setup(void) {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void open() {
  door_servo.write(0);
  delay(5000);
  door_servo.write(180);
  delay(1000);
}

void close() {
  door_servo.write(180);
  delay(1000);
}