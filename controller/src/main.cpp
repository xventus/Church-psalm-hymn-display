//
// vim: ts=4 et
// Church psalm hymn display, remote keyboard
// Copyright (c) 2019 Vanek
//
/// @file   main.cpp
/// @author Petr Vanek

/*
 *  Board based on ATmega328, 16MHz
 *
 *  PS/2 keyboard via USB
 *  TM1637 - 4 digits display
 *  APC220 as TX link
 *
 */


#include <Arduino.h>
#include <PS2KeyAdvanced.h>
#include <SevenSegmentTM1637.h>
#include "buffer.h"

 // display based on TM1637
#define DISPLAY_CLK 4 
#define DISPLAY_DIO 5
SevenSegmentTM1637    display(DISPLAY_CLK, DISPLAY_DIO);

// PS2 numeric key pad, or USB with PS/2 support
#define PS2_DATA  3  // D- 
#define PS2_IRQ   2  // D+
PS2KeyAdvanced keyboard;

// data buffer
Buff buff;

// predefined texts
const char text_init[] PROGMEM  = "init\0";
const char text_rdy[]  PROGMEM  = "rdy \0";
const char text_err1[] PROGMEM  = "Err1\0";
const char text_err2[] PROGMEM  = "Err2\0";

//  implementation

/**
 * @brief deletes the local display, displays a dot as a power indicator 
 * 
 */
void clearD() {
  display.printRaw((uint8_t)0, (uint8_t)0);
  display.printRaw((uint8_t)0, (uint8_t)1);
  display.printRaw((uint8_t)0, (uint8_t)2);
  display.printRaw((uint8_t)128, (uint8_t)3); 
}

/**
 * @brief digit display on LED
 * 
 * @param pos - position of the displayed character
 * @param value - numeric or ASCII value of the number
 * @param dot - show decimal point as separator
 * @param show - show or turn off position
 */
void showNumber(uint8_t pos, uint8_t value, bool dot, bool show) {

  if (!show) {
    display.printRaw((uint8_t)0, pos);
  }
  else {
    uint8_t dsp = 0;
    if (value > 9) {
      dsp = display.encode((char)(value));
    }
    else {
      dsp = display.encode((char)(value + 48));
    }
    if (dot) dsp += 128;
    display.printRaw(dsp, pos);
  }
}

/**
 * @brief display of the entered number
 * 
 * @param curspos - position of the entered number
 */
void displayD(uint8_t curspos) {
 
  auto zerocnt = 0;
 
  for (auto o = 0; o < MAX_DIGIT; o++) {
    auto cn = buff.getNumber(o);
    if (cn == 0 || cn == 48) {
      zerocnt++;
    }
    else {
      break;
    }
  }

  if ((zerocnt == MAX_DIGIT && curspos == MAX_DIGIT) || curspos == 0) {
    clearD();
    return;
  }

  auto cn = buff.getNumber(MAX_DIGIT - 1);
  if (cn == 0 || cn == 48) {
    showNumber(0, 0, false, false);
  }

  for (auto i = 0; i < MAX_DIGIT; i++) {

    if (curspos == MAX_DIGIT) {
      // show all
      auto con = buff.getNumber(MAX_DIGIT - 1);
      if (con == 0 || con == 48) {
        // last position is zero - do not display it
        showNumber(i + 1, buff.getNumber(i), false, true);
      }
      else {
        showNumber(i, buff.getNumber(i), (i == MAX_DIGIT - 2) ? true : false, true);
      }
    }
    else {
      // moving position
      /*
         pos = 1 -->  disp 3 from buffer 0
         pos = 2 -->  disp 3 from buffer 1, disp 2 from buffer 0
         pos = 3 -->  disp 3 from buffer 2, disp 2 from buffer 1, disp 1 from buffer 0
      */
      showNumber((MAX_DIGIT - 1) - i, buff.getNumber(curspos - 1 - i), false, true);
      if (curspos == (i + 1)) break;
    }
  }
  
}


/**
 * @brief default settings & program start
 * 
 */
void setup() {
 
  // initialization of the LED display
  display.begin();
  display.setBacklight(100);
  display.print(text_init);

  // initialization of the serial line for RF communication, optionally for RS 485
  Serial.begin(9600);

  // PS2 / USB keyboard initialisation
  keyboard.begin(PS2_DATA, PS2_IRQ);
  delay(1000);
  keyboard.echo();              // ping keyboard to see if there
  delay(6);
  auto c = keyboard.read();
  if ((c & 0xFF) == PS2_KEY_ECHO
    || (c & 0xFF) == PS2_KEY_BAT) {
    display.print(text_rdy);   // keyboard found, display "rdy"
    }
  else
    if ((c & 0xFF) == 0) {
      display.print(text_err1); // the keyboard response is 0, not ready? "Err1" is displayed
    }
    else
    {
      display.print(text_err2); // unsupported answer, "Err2" is displayed
    }

  buff.clear();
  delay(500);
  clearD();
  keyboard.setLock(PS2_LOCK_NUM);
}



/**
 * @brief Repeat till you're blue in the face  ;-)
 * 
 */
void loop() {

  // prevent to switch numeric off
  if (!(keyboard.getLock() && PS2_LOCK_NUM)) {
    keyboard.setLock(PS2_LOCK_NUM);
  }


  if (keyboard.available()) {
    // read the next key
    auto c = keyboard.read();
    if (buff.push(c)) {
      auto status = c >> 8;
      // continuous display result

      if (buff.isExec()) {
        displayD(buff.getCursorPos());
        
        // send data to the remote display
        // Here there is no sophisticated way of checking errors during 
        // transmission and for compatibility with old displays the sequence 
        // of numbers is sent several times over. The receiver then accepts 
        // two consecutive sequences as legitimate data. 
        // Something like CRC would be useful in the future, but there is no time.  

        for (uint8_t c = 0;c < 3;c++) {
          for (uint8_t b = 0;b < MAX_DIGIT;b++) {
            Serial.write((uint8_t)buff.getNumber(b));  // Here the characters are sent over the serial line. 
          }
          delay(40);
        }

        buff.copyToShadow();
      }
      else {
        if (buff.getCursorPos() == 1) {
          clearD();
        }

        displayD(buff.getCursorPos());
      }

      // wait for release key
      while (true) {
        c = keyboard.read();
        if ((c >> 8) != status) break;
      }

    }
  }
}

/*EOF*/