 #define BLYNK_TEMPLATE_ID "your_blynk_template_id" 
      #define BLYNK_TEMPLATE_NAME "" 
#define BLYNK_AUTH_TOKEN "your_token" 
#define BLYNK_PRINT Serial 
 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
 
#define DIGIT1_PIN    D0 
#define DIGIT2_PIN    D1 
#define SEGMENT_A_PIN D2 
#define SEGMENT_B_PIN D3 
#define SEGMENT_C_PIN D5 
#define SEGMENT_D_PIN D7 
#define SEGMENT_E_PIN D6 
#define SEGMENT_F_PIN D4 
#define SEGMENT_G_PIN D8 
#define BUTTON_PIN    D0 
#define BUTTON1_PIN   10 
#define IR_SENSOR_PIN 3 
 
const char* ssid = "your_wifi_name";       
const char* password = "";   
BlynkTimer timer; 
 
#include <ezTime.h> 
Timezone local; 
 
#include <Arduino.h> 
#include <ESP_Google_Sheet_Client.h> 
#include <GS_SDHelper.h> 
// Google Project ID 
#define PROJECT_ID "iot-datalogging-415010" 
// Service Account's client email 
#define CLIENT_EMAIL "iot-datalogging@iot-datalogging
415010.iam.gserviceaccount.com" 
// Service Account's private key 
const char PRIVATE_KEY[] PROGMEM = "your_private_key"; 
// The ID of the spreadsheet where you'll publish the data 
const char spreadsheetId[] = 
"spreadsheet_id"; 
  
void tokenStatusCallback(TokenInfo info); 
 
int pb1,flag1=0,cnt1=0; 
int pb2,flag2=0,cnt2=0; 
 
BLYNK_CONNECTED()  
{ Blynk.sendInternal("utc", "time");       
  Blynk.sendInternal("utc", "tz_rule"); 
  Blynk.sendInternal("rtc", "sync"); 
  Blynk.syncAll(); 
} 
 
BLYNK_WRITE(InternalPinUTC)  
{ String cmd = param[0].asStr(); 
  if (cmd == "time")  
  { const uint64_t utc_time = param[1].asLongLong(); 
    UTC.setTime(utc_time / 1000, utc_time % 1000); 
  }  
  else if (cmd == "tz_rule")  
  { String tz_rule = param[1].asStr(); 
    local.setPosix(tz_rule); 

  } 
} 
 
long ts; 
BLYNK_WRITE(InternalPinRTC) { ts = param.asLong(); } 
char tx2buf[100]; 
 
void localTime()  
{ sprintf(tx2buf,"Time:%d:%d:%d Day:%d/%d/%d", 
  local.hour(),local.minute(),local.second(), 
  local.day(),local.month(),local.year()); 
   
  Blynk.virtualWrite(V14,tx2buf); 
  Serial.println(tx2buf); 
} 
 
void initGoogleSheet() 
{ GSheet.printf("ESP Google Sheet Client v%s\n\n", 
ESP_GOOGLE_SHEET_CLIENT_VERSION); 
  GSheet.setTokenCallback(tokenStatusCallback); 
  GSheet.setPrerefreshSeconds(10 * 60); 

  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY); 
} 
 
void sendGoogleSheet() 
{ bool ready = GSheet.ready(); 
  FirebaseJson response; 
 
  Serial.println("\nAppend spreadsheet values..."); 
  Serial.println("----------------------------"); 
  FirebaseJson valueRange; 
 
  char jam    = local.hour(); 
  char minit  = local.minute(); 
  char hari = local.day(); 
  char bulan = local.month(); 
  char tahun = local.year(); 
 
  valueRange.add("majorDimension", "COLUMNS"); 
  valueRange.set("values/[0]/[0]", (int)jam); 
  valueRange.set("values/[1]/[0]", (int)minit); 
  valueRange.set("values/[2]/[0]", (int)hari); 
 
  valueRange.set("values/[3]/[0]", (int)bulan); 
 
  bool success = GSheet.values.append(&response /* returned response */, 
spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append 
*/, &valueRange /* data range to append */); 
  if (success) 
  { response.toString(Serial, true); 
    valueRange.clear(); 
  } 
  else 
  { Serial.println(GSheet.errorReason()); 
  } 
  Serial.println(); 
  Serial.println(ESP.getFreeHeap()); 
} 
 
void tokenStatusCallback(TokenInfo info) 
{ if (info.status == token_status_error) 
  { GSheet.printf("Token info: type = %s, status = %s\n", 
GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str()); 
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str()); 
  } 

  else 
  { GSheet.printf("Token info: type = %s, status = %s\n", 
GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str()); 
  } 
} 
 
void cekInput() 
{ pb1 = digitalRead(D7); 
  pb2 = digitalRead(D8); 
   
  // if(pb1==0) { flag1=1; sendGoogleSheet(); cnt1++; } 
  // if(pb2==0) { flag2=1; sendGoogleSheet(); cnt2++; } 
  Serial.print(pb1); 
  Serial.println(pb2); 
} 
 
 
int i = 0; 
int k = 0; 
int j = 0; 
  
int Arduino_Pins[7] = {SEGMENT_A_PIN, SEGMENT_B_PIN, 
SEGMENT_C_PIN, SEGMENT_D_PIN, SEGMENT_E_PIN, 
SEGMENT_F_PIN, SEGMENT_G_PIN}; 
int Digit_Pins[2] = {DIGIT1_PIN, DIGIT2_PIN}; 
int Segment_Pins[10][7] = {{1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0, 0}, {1, 1, 0, 1, 
1, 0, 1}, {1, 1, 1, 1, 0, 0, 1}, 
                           {0, 1, 1, 0, 0, 1, 1}, {1, 0, 1, 1, 0, 1, 1}, {1, 0, 1, 1, 1, 1, 1}, {1, 
1, 1, 0, 0, 0, 0}, 
                           {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1}}; 
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50; 
unsigned long sujudDelay = 3000; // 5 seconds 
int irState = HIGH; 
int lastIrState = HIGH; 
unsigned long sujudStartTime = 0; 
int prostateCount = 0; 
int rakaatCount = 0; 
int initialInput = 0; 
int button1State = HIGH; 
int lastButton1State = HIGH; 
int button2State = HIGH; 
int lastButton2State = HIGH; 

int val = 1; 
int b = 10; 
int a; 
 
void setup() { 
  Serial.begin(115200); 
  delay(10); 
 
  timer.setInterval(60000L,localTime); 
  //timer.setInterval(10L,cekInput); 
 
  // Connect to Wi-Fi 
  Serial.println(); 
  Serial.print("Connecting to "); 
  Serial.println(ssid); 
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
 
  } 
 
  Serial.println(""); 
  Serial.println("WiFi connected"); 
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 
 
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password); 
 
   for (int i = 0; i < 7; ++i) { 
    pinMode(Arduino_Pins[i], OUTPUT); 
  } 
  for (int i = 0; i < 2; ++i) { 
    pinMode(Digit_Pins[i], OUTPUT); 
  } 
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(BUTTON1_PIN, INPUT_PULLUP); 
  pinMode(IR_SENSOR_PIN, INPUT); 
  initGoogleSheet(); 
} 
 
BLYNK_WRITE(V5) { 
  int buttonstate = param.asInt(); 
  if(buttonstate == HIGH) { 
    incrementNumber(); 
    updateDisplay(); 
     // Check if button1 is pressed 
    if (buttonstate == HIGH) { 
      initialInput++; 
      Serial.print("Initial Input: "); 
      Serial.println(initialInput); 
      Blynk.virtualWrite(V2, initialInput); 
    } 
  } 
} 
 
BLYNK_WRITE(V6) { 
  int buttonstate = param.asInt(); 
    if(buttonstate == HIGH) { 
    decrementNumber(); 
    updateDisplay(); 
    Serial.println(V6); 

    if (buttonstate == HIGH) { 
      initialInput--; 
      Serial.print("Initial Input: "); 
      Serial.println(initialInput); 
      Blynk.virtualWrite(V2, initialInput); 
    } 
  } 
} 
 
BLYNK_WRITE(V9){ 
  int buttonstate = param.asInt(); 
  if(buttonstate == HIGH) { 
  bool ready = GSheet.ready(); 
  FirebaseJson response; 
 
  Serial.println("\nAppend spreadsheet values..."); 
  Serial.println("----------------------------"); 
  FirebaseJson valueRange; 
 
  char jam   = local.hour(); 
  char minit = local.minute(); 
  char hari  = local.day(); 
  char bulan = local.month(); 
  
  valueRange.add("majorDimension", "COLUMNS"); 
  valueRange.set("values/[0]/[0]", (int)jam); 
  valueRange.set("values/[1]/[0]", (int)minit); 
  valueRange.set("values/[2]/[0]", (int)hari); 
  valueRange.set("values/[3]/[0]", (int)bulan); 
  valueRange.set("values/[4]/[0]", "Subuh"); 
 
 
  bool success = GSheet.values.append(&response /* returned response */, 
spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append 
*/, &valueRange /* data range to append */); 
  if (success) 
  { response.toString(Serial, true); 
    valueRange.clear(); 
  } 
  else 
  { Serial.println(GSheet.errorReason()); 
  } 
  Serial.println(); 
  Serial.println(ESP.getFreeHeap()); 
 
    subuh(); 
    updateDisplay(); 
    initialInput = 2; 
    Serial.print("Initial Input: "); 
    Serial.println(initialInput); 
    Blynk.virtualWrite(V2, initialInput); 
  } 
} 
 
BLYNK_WRITE(V10){ 
  int buttonstate = param.asInt(); 
  if(buttonstate == HIGH) { 
  bool ready = GSheet.ready(); 
  FirebaseJson response; 
 
  Serial.println("\nAppend spreadsheet values..."); 
  Serial.println("----------------------------"); 
  FirebaseJson valueRange; 

  char jam   = local.hour(); 
  char minit = local.minute(); 
  char hari  = local.day(); 
  char bulan = local.month(); 
  
 
  valueRange.add("majorDimension", "COLUMNS"); 
  valueRange.set("values/[0]/[0]", (int)jam); 
  valueRange.set("values/[1]/[0]", (int)minit); 
  valueRange.set("values/[2]/[0]", (int)hari); 
  valueRange.set("values/[3]/[0]", (int)bulan); 
  valueRange.set("values/[4]/[0]", "Zohor"); 
 
 
  bool success = GSheet.values.append(&response /* returned response */, 
spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append 
*/, &valueRange /* data range to append */); 
  if (success) 
  { response.toString(Serial, true); 
    valueRange.clear(); 
  } 
  else 

  { Serial.println(GSheet.errorReason()); 
  } 
  Serial.println(); 
  Serial.println(ESP.getFreeHeap()); 
 
    zohor(); 
    updateDisplay(); 
    initialInput = 4; 
    Serial.print("Initial Input: "); 
    Serial.println(initialInput); 
    Blynk.virtualWrite(V2, initialInput); 
  } 
} 
 
BLYNK_WRITE(V11){ 
  int buttonstate = param.asInt(); 
  if(buttonstate == HIGH) { 
  bool ready = GSheet.ready(); 
  FirebaseJson response; 
 
  Serial.println("\nAppend spreadsheet values..."); 

  Serial.println("----------------------------"); 
  FirebaseJson valueRange; 
 
  char jam    = local.hour(); 
  char minit  = local.minute(); 
  char hari = local.day(); 
  char bulan = local.month(); 
  
 
  valueRange.add("majorDimension", "COLUMNS"); 
  valueRange.set("values/[0]/[0]", (int)jam); 
  valueRange.set("values/[1]/[0]", (int)minit); 
  valueRange.set("values/[2]/[0]", (int)hari); 
  valueRange.set("values/[3]/[0]", (int)bulan); 
  valueRange.set("values/[4]/[0]", "Asar"); 
 
 
  bool success = GSheet.values.append(&response /* returned response */, 
spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append 
*/, &valueRange /* data range to append */); 
  if (success) 
  { response.toString(Serial, true); 

    valueRange.clear(); 
  } 
  else 
  { Serial.println(GSheet.errorReason()); 
  } 
  Serial.println(); 
  Serial.println(ESP.getFreeHeap()); 
 
    asar(); 
    updateDisplay(); 
    initialInput = 4; 
    Serial.print("Initial Input: "); 
    Serial.println(initialInput); 
    Blynk.virtualWrite(V2, initialInput); 
  } 
} 
 
BLYNK_WRITE(V12){ 
  int buttonstate = param.asInt(); 
  if(buttonstate == HIGH) { 
  bool ready = GSheet.ready(); 

  FirebaseJson response; 
 
  Serial.println("\nAppend spreadsheet values..."); 
  Serial.println("----------------------------"); 
  FirebaseJson valueRange; 
 
  char jam   = local.hour(); 
  char minit = local.minute(); 
  char hari  = local.day(); 
  char bulan = local.month(); 
  
 
  valueRange.add("majorDimension", "COLUMNS"); 
  valueRange.set("values/[0]/[0]", (int)jam); 
  valueRange.set("values/[1]/[0]", (int)minit); 
  valueRange.set("values/[2]/[0]", (int)hari); 
  valueRange.set("values/[3]/[0]", (int)bulan); 
  valueRange.set("values/[4]/[0]", "Maghrib"); 

  bool success = GSheet.values.append(&response /* returned response */, 
spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append 
*/, &valueRange /* data range to append */); 
  if (success) 
  { response.toString(Serial, true); 
    valueRange.clear(); 
  } 
  else 
  { Serial.println(GSheet.errorReason()); 
  } 
  Serial.println(); 
  Serial.println(ESP.getFreeHeap()); 
 
    maghrib(); 
    updateDisplay(); 
    initialInput = 3; 
    Serial.print("Initial Input: "); 
    Serial.println(initialInput); 
    Blynk.virtualWrite(V2, initialInput); 
  } 
} 

BLYNK_WRITE(V13){ 
  int buttonstate = param.asInt(); 
  if(buttonstate == HIGH) { 
    bool ready = GSheet.ready(); 
  FirebaseJson response; 
 
  Serial.println("\nAppend spreadsheet values..."); 
  Serial.println("----------------------------"); 
  FirebaseJson valueRange; 
 
  char jam    = local.hour(); 
  char minit  = local.minute(); 
  char hari = local.day(); 
  char bulan = local.month(); 
  
 
  valueRange.add("majorDimension", "COLUMNS"); 
  valueRange.set("values/[0]/[0]", (int)jam); 
  valueRange.set("values/[1]/[0]", (int)minit); 
  valueRange.set("values/[2]/[0]", (int)hari); 
  valueRange.set("values/[3]/[0]", (int)bulan); 

  valueRange.set("values/[4]/[0]", "Isyak"); 
 
 
  bool success = GSheet.values.append(&response /* returned response */, 
spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append 
*/, &valueRange /* data range to append */); 
  if (success) 
  { response.toString(Serial, true); 
    valueRange.clear(); 
  } 
  else 
  { Serial.println(GSheet.errorReason()); 
  } 
  Serial.println(); 
  Serial.println(ESP.getFreeHeap()); 
    isyak(); 
    updateDisplay(); 
    initialInput = 4; 
    Serial.print("Initial Input: "); 
    Serial.println(initialInput); 
    Blynk.virtualWrite(V2, initialInput); 
  } 

} 
 
void decrementNumber() { 
  i--; 
  if (i < 0) { 
    k--; 
    if (k < 0) { 
      k = 9; 
    } 
    i = 9; 
  } 
} 
 
void incrementNumber (){ 
  i++; 
      if (i == 10) { 
        k = k + 1; 
        i = 0; 
        if (k == 10) { 
          k = 0; 
        } 

      } 
} 
 
void subuh (){ 
  i = 2; 
  k = 0; 
 
} 
 
void zohor (){ 
  i = 4; 
  k = 0; 
} 
 
void asar (){ 
  i = 4; 
  k = 0; 
} 
 
void maghrib (){ 
  i = 3; 

  k = 0; 
} 
 
void isyak (){ 
  i = 4; 
  k = 0; 
} 
 
void updateDisplay() { 
  for (j = 0; j < 7; j++) { 
    digitalWrite(Arduino_Pins[j], Segment_Pins[i][j]); 
  } 
  digitalWrite(Digit_Pins[0], HIGH); 
  delay(1); 
  digitalWrite(Digit_Pins[0], LOW); 
 
  for (j = 0; j < 7; j++) { 
    digitalWrite(Arduino_Pins[j], Segment_Pins[k][j]); 
  } 
  digitalWrite(Digit_Pins[1], HIGH); 
  delay(1); 
 
  digitalWrite(Digit_Pins[1], LOW); 
} 
 
void loop() { 
for (int n = 0; n < 500; n++) { 
    updateDisplay(); 
 
    int reading = digitalRead(IR_SENSOR_PIN); 
    unsigned long currentTime = millis(); 
 
    if (reading != lastIrState) { 
      lastDebounceTime = currentTime; 
    } 
 
    if ((currentTime - lastDebounceTime) > debounceDelay) { 
      if (reading != irState) { 
        irState = reading; 
 
        if (irState == LOW) { 
          sujudStartTime = currentTime; 
        } else { 

          if ((currentTime - sujudStartTime) > sujudDelay) { 
            prostateCount++; 
            Serial.print("Sujud: "); 
            Serial.println(prostateCount); 
 
            if (prostateCount % 2 == 0) { 
              rakaatCount++; 
              Serial.print("Rakaat Count: "); 
              Serial.println(rakaatCount); 
 
              // Update display based on initial input 
              if(initialInput <= 10){ 
                initialInput = initialInput - val; 
                i = (initialInput) ; 
                k = 0; 
                Serial.println(initialInput); 
                Blynk.virtualWrite(V2, initialInput); 
              } 
 
              if(initialInput > 10){ 
                initialInput = initialInput - val; 

                a = (initialInput - b); 
                k = 1; 
                i = a ; 
                Serial.println(initialInput); 
                Blynk.virtualWrite(V2, initialInput); 
                if(initialInput == 10) { 
                  k = 1; 
                  i = 0; 
                } 
              } 
              updateDisplay(); 
            } 
          } 
        } 
      } 
    } 
 
    lastIrState = reading; 
 
    // Read button states 
    button1State = digitalRead(BUTTON_PIN); 

    button2State = digitalRead(BUTTON1_PIN); 
 
    // Check if button1 is pressed 
    if (button1State == LOW && lastButton1State == HIGH) { 
      initialInput++; 
      Serial.print("Initial Input: "); 
      Serial.println(initialInput); 
      Blynk.virtualWrite(V2, initialInput); 
    } 
 
    // Check if button2 is pressed 
    if (button2State == LOW && lastButton2State == HIGH) { 
      initialInput--; 
      Serial.print("Initial Input: "); 
      Serial.println(initialInput); 
      Blynk.virtualWrite(V2, initialInput); 
    } 
 
    lastButton1State = button1State; 
    lastButton2State = button2State; 
 
    if (digitalRead(BUTTON_PIN) == HIGH) { 
      while (digitalRead(BUTTON_PIN) == HIGH) { 
        delay(100); 
      } 
      incrementNumber(); 
    } 
 
    if (digitalRead(BUTTON1_PIN) == HIGH) { 
      while (digitalRead(BUTTON1_PIN) == HIGH) { 
        delay(100); 
      } 
      decrementNumber(); 
    } 
  } 
 
 
  Blynk.run(); 
  timer.run(); 
}