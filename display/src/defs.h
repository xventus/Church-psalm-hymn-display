//
// vim: ts=4 et
// Copyright (c) 2001 - 2021 Petr Vanek
//
/// @file   defs.h
/// @author Petr Vanek

#pragma once

#include <inttypes.h>
#include <string.h>

// HARDWARE

/*
 brief description of HW & SW:
 LILYGO® TTGO VGA32 V1.4 Controller PS/2 Mouse And Keyboard Graphics Library Game Engine And ANSI/VT Terminal For The ESP32
 

 Board hardware - pin out

 VGA:   RED   21/22
        GREEN 18/19
        BLUE  4/5
        VSYNC 15
        HSYNC 23

  SD:   CS    13
        MOSI  12
        MISO  2
        CLK   14

  PS/2 keyboard:  CLK   33
                  Data  32

  PS/2 mouse:     CLK   26
                  Data  27


  HEAD:  34
         2
         39
         13
         GND
         14
         3V3
         12
  SND: IO25

*/

/*
 RX can be mapped on any pin
 TX pin must be GPIO0 to GPIO31.
*/

/*
 * 5V  - BLACK
 * GND - Yellow
 * DATA - RED = APC220 - TXD
 *
 * */

#define APC220_RX1 27 // (Data PS/2)  RED
#define APC220_TX1 14 //  not used from header

// SD CARD interface
#define SD_MISO 2
#define SD_MOSI 12
#define SD_CLK  14
#define SD_CS   13 

// buffer defs. 
#define MAX_BUFFER_SIZE 300 // max text len


// SD card
#define FORMAT_ON_FAIL false
#define SDCARD_MOUNT_PATH "/SD"

// colors
#define PENCOLOR 0xf0, 0xc3, 0x2a
#define PENCOLOR2 0xff, 0xff, 0xff   // color number for then song number
#define BRUSHCOLOR 0x00, 0x00, 128

#define PENCOLOR_1 0xff, 0xff, 0xff
#define BRUSHCOLOR_1 0x00, 0x00, 0x00

#define PENCOLOR_2 0x00, 0x00, 0x00
#define BRUSHCOLOR_2 0xff, 0xff, 0xff

/*display constants*/
const auto fontwidth = 120;
const auto charwidth = 130;
const auto fontheight = 184;
const auto fontcomma = 10;
const auto digitmovepos = 80; // move + distance  possition if only  3 digits
const auto leftside = 40;
const auto topsidetxt = 0;
const auto leftsidetxt = 5;
const auto charwidthtxt = 28;
const auto charheighttxt = 31;
const auto heightincrementcorrection = charheighttxt - 3;
const auto withlabel = true;

#define ERROR_DELAY 50000 // how long error message will be dispalyed

// VGA output setting
#define VGAMODE "\"monitor\" 40 800 840 968 1056 300 301 303 314 -HSync -VSync DoubleScan FrontPorchBegins"


