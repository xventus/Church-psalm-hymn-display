

//
// vim: ts=4 et
// Copyright (c) 2022 Petr Vanek, petr@fotoventus.cz
//
/// @file   strings_def.h
/// @author Petr Vanek

#pragma once

#include <Arduino.h>
#include <inttypes.h>
#include <string.h>


const char text_KANCIONAL[] PROGMEM  = "KANCION\xc1L\0";
const char text_SLOKA[] PROGMEM  = "SLOKA\0";
const char text_HELLO[] PROGMEM  = "www.paxvobis.cz\0";
const char text_PSALM[] PROGMEM  = "Odpove\xef \x9e\x61lmu:\0";
const char text_START_1[] PROGMEM  = " MiB obsazen\xfd\x63h\n\0";
const char text_START_2[] PROGMEM  = " MiB voln\xfd\x63h\n\n\0";
const char text_START_3[] PROGMEM  = " z\xe1znam\xf9 \x9e\x61lm\xf9 \n\0";
const char text_START_4[] PROGMEM  = " z\xe1znam\xf9 z kancion\xe1lu\n\n\0";
const char text_START_5[] PROGMEM  = "SD karta nenalezena\n\n\0";
const char text_START_6[] PROGMEM  = "PS/2 kl\xe1vesnice: \0";
const char text_START_OK[] PROGMEM  = "OK\0";
const char text_START_NA[] PROGMEM  = "N/A\0";
const char text_START_7[] PROGMEM  = "\nkancion\xe1l text: \0";
const char text_START_8[] PROGMEM  = "\nschema: \0";
const char text_ON[] PROGMEM  = "ZAPNUTO\0";
const char text_OFF[] PROGMEM  = "VYPNUTO\0";
const char text_ERROR[] PROGMEM  = "Chyba:\n\0";
const char text_NOTFOUND[] PROGMEM  = "\nsoubor nenalezen\0";
const char text_TWOZEROS[] PROGMEM  = "00\0";
const char text_ZERO[] PROGMEM  = "0\0";
const char text_SLASH[] PROGMEM  ="/\0";

const char text_psalms[] PROGMEM  = "zalmy\0";      // dir name 
const char text_hymns[] PROGMEM  = "kancional\0";   // dir name
const char text_file_txt[] PROGMEM  = ".txt\0";     // file extension
const char text_rb_txt[] PROGMEM  = "rb\0";         // rb





