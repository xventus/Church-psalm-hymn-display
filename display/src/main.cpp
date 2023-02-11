//
// vim: ts=4 et
// Church psalm hymn display, display unit
// Copyright (c) 2019 Petr Vanek, petr@fotoventus.cz
//
/// @file   main.cpp
/// @author Petr Vanek

/*
 based on board:  LILYGO® TTGO VGA32 V1.4 Controller PS/2 Mouse And Keyboard Graphics Library Game Engine And ANSI/VT Terminal For The ESP32
 Enviroment: ESP32 DEV Module
 L
 ibrary:   fabgle from http://www.fabglib.org/
*/

#include <stdio.h>

#include <fabgl.h>

#include "defs.h"          // constants and definitions
#include "configuration.h" // EEPROM config
#include "fonts.h"         // font glyphs
#include "net_buffer.h"    // buffer operations
#include "convertors.h"    // CSY char convertor, virtual keys to number
#include "strings_def.h"   // strings

// TODO: 
//  - move strings to program memory!!!
//  - divide the program into functional units
//  - fill in missing characters in the national alphabet

/************************************************/
// signletons

// VGA controller
fabgl::VGA16Controller VGAController;

// PS/2 keyboard as local controller
fabgl::PS2Controller PS2Controller;

// HW serial for APC220/RS485 - used only one WAY communication - RX used only
// used with remote controller via APC220 or RS485
HardwareSerial hwSerial_1(1);
NetBuffer nb;
Configuration conf;

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief display clear and set default color
 *
 */
void clear()
{
  Canvas cv(&VGAController);
  if (conf.getColorSchema() == 0)
  {
    cv.setPenColor(PENCOLOR);
    cv.setBrushColor(BRUSHCOLOR);
  }

  if (conf.getColorSchema() == 1)
  {
    cv.setPenColor(PENCOLOR_1);
    cv.setBrushColor(BRUSHCOLOR_1);
  }

  if (conf.getColorSchema() == 2)
  {
    cv.setPenColor(PENCOLOR_2);
    cv.setBrushColor(BRUSHCOLOR_2);
  }

  cv.clear();
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief display number at position
 *
 * @param x
 * @param y
 * @param number
 * @param fill
 */
void displayDigit(int x, int y, byte number, bool fill)
{
  Canvas cv(&VGAController);
  if (number > 10)
    return;
  cv.setGlyphOptions(GlyphOptions().FillBackground(fill).DoubleWidth(0));
  cv.drawGlyph(x, y, fontwidth, fontheight, &fontBB[0 + number][0]);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief displays the glyph in the given size at the position
 * 
 * @param big  switches the size set 
 * @param x    position x
 * @param y    position y 
 * @param chr  desired character
 */
void displayChar(byte big, int x, int y, unsigned char chr)
{
  Canvas cv(&VGAController);
  auto decodedto = charConvertor(chr);

  if (big == 2)
  {
    cv.setGlyphOptions(GlyphOptions().FillBackground(false).DoubleWidth(1));
    cv.drawGlyph(leftsidetxt + x, topsidetxt + y, 24, 32, &fontB[decodedto][0]);
  }
  else if (big == 1)
  {
    cv.setGlyphOptions(GlyphOptions().FillBackground(false).DoubleWidth(2));
    cv.drawGlyph(leftsidetxt + x, topsidetxt + y, 24, 32, &fontB[decodedto][0]);
    cv.setGlyphOptions(GlyphOptions().FillBackground(false).DoubleWidth(3));
    cv.drawGlyph(leftsidetxt + x, topsidetxt + y + 32, 24, 32, &fontB[decodedto][0]);
  }
  else
  {
    cv.setGlyphOptions(GlyphOptions().FillBackground(false).DoubleWidth(0));
    cv.drawGlyph(leftsidetxt + x, topsidetxt + y, 24, 32, &fontB[decodedto][0]);
  }
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief prints a string at a given position in a given size
 * 
 * @param big switches the size set 
 * @param str string to print
 * @param pos 
 * @param yzero 
 * @param correction 
 * @return int 
 */
int printAtPos(byte big, const char *str, int pos, int yzero, bool correction)
{

  int x = pos;      // x possition of char
  int y = 0;        // y possiont of char
  int line = 0;     // line counter
  byte counter = 0; // char counter

  if (str == nullptr)
    return 0;
  Canvas cv(&VGAController);

  auto maxperline = (big == 1 || big == 2) ? (cv.getWidth() / (charwidthtxt * 2)) : (cv.getWidth() / charwidthtxt);
  auto linesperpage = (big == 1) ? (cv.getHeight() / (charheighttxt * 2)) : (cv.getHeight() / charheighttxt);
  if (big == 1) {
    linesperpage++;
  }
  counter = pos / (charwidthtxt + ((big == 1 || big == 2) ? charwidthtxt : 0));

  if (yzero != 0)
  {
    auto a = charheighttxt;
    a += ((big == 1) ? heightincrementcorrection : 0);
    y = a * yzero;
    linesperpage = linesperpage - yzero;
  }

  const char *st = str;
  auto cntline = (strlen(st) / maxperline) + 1;
  auto movy = charheighttxt;
  movy += ((big == 1) ? heightincrementcorrection : 0);
  auto ycorrection = (correction) ? ((linesperpage / cntline) * movy / cntline) : 0;
  if (cntline >= 7 && big == 2)
    ycorrection = 0;
  if (correction && cntline < 3)
  {
    // only y position of first line
    y += movy;
  }

  while (*st != 0)
  {

    if (*st == '\n')
    {
      // new line
      line++;
      x = 0;
      y += charheighttxt;
      y += ((big == 1) ? heightincrementcorrection : 0);
      counter = 0;
      st++;
      continue;
    }

    displayChar(big, x, y, *st);
    counter++;

    if (counter >= maxperline)
    {
      // move to next line if number of chars per line was exceeded
      line++;

      if (linesperpage <= line)
      {
        break;
      }

      // sets new position of char on the next line
      x = 0;
      y += charheighttxt;
      y += ((big == 1) ? heightincrementcorrection : 0);
      y += ycorrection;
      counter = 0;
    }
    else
    {
      x += charwidthtxt;
      x += ((big == 1 || big == 2) ? charwidthtxt : 0);
    }

    st++;
  }

  return x;
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief print from the begining
 * 
 * @param big 
 * @param str 
 * @param correction 
 */
void printAt(byte big, const char *str, bool correction)
{
   printAtPos(big, str, 0, 0, correction);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Printing the number
 * 
 * @param one 
 * @param two 
 * @param big 
 * @param str 
 * @param correction 
 */
void printAtWithNumber(int one, int two, byte big, const char *str, bool correction)
{
  String buffer;
  auto pos = 0;
  Canvas cv(&VGAController);
  if (one < 10)
    buffer = text_TWOZEROS;
  else if (one < 100)
    buffer = text_ZERO;
  buffer += String(one);

  cv.setPenColor(PENCOLOR2);
  pos = printAtPos(2, buffer.c_str(), 0, 0, false);
  if (two != 0)
  {
    pos = printAtPos(2, text_SLASH, pos, 0, false);
    buffer = String(two);
    pos = printAtPos(2, buffer.c_str(), pos, 0, false);
  }

  if (conf.getColorSchema() == 0)
  {
    cv.setPenColor(PENCOLOR);
  }

  if (conf.getColorSchema() == 1)
  {
    cv.setPenColor(PENCOLOR_1);
  }

  if (conf.getColorSchema() == 2)
  {
    cv.setPenColor(PENCOLOR_2);
  };
  printAtPos(big, str, 0, 1, true);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief 
 * 
 * @param xp 
 * @param yp 
 * @param str 
 */
void printText(int xp, int yp, const char *str)
{

  int x = xp;       // x possition of char
  int y = yp;       // y possiont of char
  int line = 0;     // line counter
  byte counter = 0; // char counter

  if (str == nullptr)
    return;
  Canvas cv(&VGAController);

  auto maxperline = (cv.getWidth() / charwidthtxt);
  auto linesperpage = (cv.getHeight() / charheighttxt);

  const char *st = str;
  while (*st != 0)
  {
    displayChar(false, x, y, *st);
    counter++;

    if (counter >= maxperline)
    {
      // move to next line if number of chars per line was exceeded
      line++;

      if (linesperpage <= line)
      {
        break;
      }

      // sets new position of char on the next line
      x = xp;
      y += charheighttxt;
      counter = 0;
    }
    else
    {
      x += charwidthtxt;
    }

    st++;
  }
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief  display contnet of buffer - number type
 * 
 * @param buf 
 */
void displayNumberBuffer(uint8_t buf[])
{

  // with label
  bool b3digitonly = false;
  Canvas cv(&VGAController);
  cv.clear();

  if (buf[3] == 0)
    b3digitonly = true;

  auto movepos = cv.getWidth() / (((fontwidth + 10) * (b3digitonly) ? 3 : 4));
  auto toppos = (cv.getHeight() - fontheight) / 2;

  if (withlabel)
  {
    toppos += charheighttxt;
  }

  auto lleftside = 0;
  if (b3digitonly)
  {
    lleftside = cv.getWidth() - (3 * charwidth + (movepos / 2));
  }
  else
  {
    lleftside = cv.getWidth() - (4 * charwidth + (movepos / 2));
  }
  lleftside = lleftside / 2;

  auto xx = lleftside;

  for (int x = 0; x < 4; x++)
  {

    auto npositionvalue = buf[x];

    // filter uknown digit values
    if (npositionvalue > 9)
      continue;

    // shift left last digit
    if (!b3digitonly && x == 3)
    {
      xx += movepos / 2; // digitmovepos;
    }

    // hide last digit, when only 3 digits needs to be shown
    if (b3digitonly && x > 2)
      continue;

    // show number on desired position
    if (b3digitonly)
      displayDigit(xx + (x * (charwidth + (movepos / 3))), toppos, npositionvalue, true); // 3 digits only
    else
      displayDigit(xx + (x * charwidth), toppos, npositionvalue, true); // 4 digits with comma
  }

  if (!b3digitonly)
  {
    // add comma
    displayDigit(lleftside + (charwidth * 3), toppos, fontcomma, false);
    if (withlabel)
    {
      printText(lleftside + (charwidth * 3) + movepos / 2, 10, text_SLOKA);

      printText(lleftside + movepos / 2, 10, text_KANCIONAL);
    }
  }
  else
  {
    if (withlabel)
    {
      auto txtcenter = (cv.getWidth() / 2) - ((charwidthtxt * 9) / 2);
      
      printText(txtcenter, 10, text_KANCIONAL );
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Welcome screen, displayed in case there is nothing to display
 * 
 */
void printHello()
{
  Canvas cv(&VGAController);
  cv.clear();
  auto txtcenter = (cv.getWidth() / 2) - ((charwidthtxt * 15) / 2);
  printText(txtcenter, cv.getHeight() - charheighttxt - 10, text_HELLO);
  auto h = cv.getHeight() - charheighttxt - 20;
  cv.drawLine(0, h, cv.getWidth() - 1, h);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief the main display of the Psalm text
 * 
 * @param txt text to print
 */
void printPsalm(const char *txt)
{
  Canvas cv(&VGAController);
  cv.clear();
  printAtPos(2, text_PSALM, 0, 0, false);
  printAtPos(2, txt, 0, 2, true);
  auto h = charheighttxt + charheighttxt / 2;
  cv.drawLine(0, h, cv.getWidth() - 1, h);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief initial system test, configuration and connected peripherals are displayed
 * 
 * @param sdcard 
 */
void initialTest(bool sdcard)
{
  String txt;
  int64_t total, used;

  if (sdcard)
  {
    FileBrowser::getFSInfo(fabgl::DriveType::SDCard, 0, &total, &used);

    txt += String(int(used / (1024 * 1024)));
    txt += text_START_1;
    txt += String(int((total - used) / (1024 * 1024)));
    txt += text_START_2; 

    auto dir = FileBrowser();
    dir.setDirectory(SDCARD_MOUNT_PATH);

    const auto psalmdir = text_psalms;
    const auto cancioaldir = text_hymns;
    dir.changeDirectory(psalmdir);
    auto t1 = dir.count() - 1;
    dir.setDirectory(SDCARD_MOUNT_PATH);
    dir.changeDirectory(cancioaldir);
    auto t2 = dir.count() - 1;

    txt += String(t1);
    txt += text_START_3; 
    txt += String(t2);
    txt += text_START_4;
  }
  else
  {
    txt += text_START_5;
  }
  txt += text_START_6; 
  txt += PS2Controller.keyboard()->isKeyboardAvailable() ? text_START_OK : text_START_NA;

  txt += text_START_7; 
  txt += (conf.getCantiMode()) ? text_ON : text_OFF;
  txt += text_START_8;
  txt += String(conf.getColorSchema());

  clear();
  printAt(false, txt.c_str(), false);
}
//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief the text of the Psalm is loaded from the SD card
 * 
 * @param name - the name of the file whose content is loaded into the global `buffer`
 * @return true  - found & loaded 
 * @return false 
 */
bool loadPsalm(const char *name)
{

  char buffer[MAX_BUFFER_SIZE];
  memset(&buffer[0], 0, MAX_BUFFER_SIZE);

  bool rc = false;
  auto dir = FileBrowser();

  memset(&buffer[0], 0, MAX_BUFFER_SIZE);
  sprintf(buffer, "%s/%s", SDCARD_MOUNT_PATH, text_psalms);
  dir.setDirectory(buffer);
  rc = dir.exists(name, false);
  memset(&buffer[0], 0, MAX_BUFFER_SIZE);

  if (!rc)
  {
    return rc;
  }

  auto cnt = 0;
  auto fp = dir.openFile(name, text_rb_txt);
  if (fp)
  {
    char c;
    while ((c = getc(fp)) != EOF)
    {
      if (c == 255)
        break;
      buffer[cnt] = c;
      cnt++;
      if (cnt >= MAX_BUFFER_SIZE)
        break;
    }
  }
  fclose(fp);
  printPsalm(&buffer[0]);
  rc = true;
  return rc;
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief 
 * 
 * @param one 
 * @param two 
 * @param directory 
 * @param name 
 * @return true 
 * @return false 
 */
bool loadCancional(int one, int two, const char *directory, const char *name)
{
  char buffer[MAX_BUFFER_SIZE];
  bool rc = false;
  auto dir = FileBrowser();

  memset(&buffer[0], 0, MAX_BUFFER_SIZE);
  sprintf(buffer, "%s/%s/%s", SDCARD_MOUNT_PATH, text_hymns, directory);
  dir.setDirectory(buffer);
  rc = dir.exists(name, false);
  memset(&buffer[0], 0, MAX_BUFFER_SIZE);

  if (!rc)
  {
    return rc;
  }

  auto cnt = 0;
  auto fp = dir.openFile(name, text_rb_txt);
  if (fp)
  {
    char c;
    while ((c = getc(fp)) != EOF)
    {
      if (c == 255)
        break;
      buffer[cnt] = c;
      cnt++;
      if (cnt >= MAX_BUFFER_SIZE)
        break;
    }
  }
  else
  {
    rc = false;
  }
  fclose(fp);
  clear();
  printAtWithNumber(one, two, false, &buffer[0], true);
  rc = true;
  return rc;
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief error message display
 * 
 * @param message 
 */
void errorMessage(const char *message)
{
  clear();
  String txt;
  txt = text_ERROR;
  txt += message;
  printAt(0, txt.c_str(), false);
}

//--------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief initializing the system and setting everything necessary
 * 
 */
void setup()
{
  conf.init();
  conf.refresh();
  Serial.begin(115200);

  hwSerial_1.begin(9600, SERIAL_8N1, APC220_RX1, APC220_TX1);

  // avoid garbage into the UART
  delay(500);

  VGAController.begin();
  VGAController.setResolution(VGAMODE);

  // PS/2 ony keyaboard
  PS2Controller.begin(PS2Preset::KeyboardPort0);

  // SD Card
  auto rc = FileBrowser::mountSDCard(FORMAT_ON_FAIL, SDCARD_MOUNT_PATH, 4, 16 * 1024, SD_MISO, SD_MOSI, SD_CLK, SD_CS);
  FileBrowser::setSDCardMaxFreqKHz(SDMMC_FREQ_DEFAULT);
  FileBrowser::remountSDCard();

  initialTest(rc);

  delay(5000);
  clear();
  printHello();
}

/**
 * @brief waiting on the keyboard - pressed key
 * 
 */
void keyboardWait()
{
  bool keyDown;
  auto keyboard = PS2Controller.keyboard();
  keyboard->getNextVirtualKey(&keyDown);
  while (keyDown)
  {
    keyboard->getNextVirtualKey(&keyDown);
  }
}

/**
 * @brief returns the value of the press key from the keyboard 
 * 
 * @return int 
 */
int keyboardupdate()
{

  if (!PS2Controller.keyboard()->isKeyboardAvailable())
  {
    return -1; // fabgl::VirtualKey::VK_NONE ;
  }

  bool keyDown;
  auto keyboard = PS2Controller.keyboard();
  VirtualKey vk = keyboard->getNextVirtualKey(&keyDown);
  return virtualKeyToNumber(vk);
}

/**
 * @brief main loop, running around until you lose your mind 
 * 
 * TODO:  eventually divide this toilet paper into separate functions
 */
void loop()
{


  bool changed = false;
  auto kbd = keyboardupdate();

  if (kbd > -1)
  {
    nb.addByteToParse(kbd);
    keyboardWait();
  }

  if (hwSerial_1.available())
  {
    auto c = hwSerial_1.read();
    nb.addByteToParse(c);
  }

  if (nb.isChanged())
  { 
    nb.cancelChange();
    if (nb.isHello())
    {
      clear();
      printHello();
    }
    else if (nb.isNumber())
    {

      /*detect configuration message 999 */
      if (   nb.getBuffer()[0] == 9 
          && nb.getBuffer()[1] == 9 
          && nb.getBuffer()[2] == 9)
      {

        if (nb.getBuffer()[3] == 0)
        {
          conf.setCantiMode(0);
          changed = true;
        }
        if (nb.getBuffer()[3] == 1)
        {
          conf.setCantiMode(1);
          changed = true;
        }

        if (nb.getBuffer()[3] == 2)
        {
          conf.setColorSchema(0);
          changed = true;
        }

        if (nb.getBuffer()[3] == 3)
        {
          conf.setColorSchema(1);
          changed = true;
        }

        if (nb.getBuffer()[3] == 4)
        {
          conf.setColorSchema(2);
          changed = true;
        }

        if (changed)
        {
          initialTest(true);
          delay(3000);

          nb.seyHello();
          clear();
          printHello();
          return;
        }
      }

      if (conf.getCantiMode() != 0)
      {

        String fileString;
        String dirString;
        dirString += (char)(nb.getBuffer()[0] + 48);
        dirString += (char)(nb.getBuffer()[1] + 48);
        dirString += (char)(nb.getBuffer()[2] + 48);

        fileString += (char)(nb.getBuffer()[3] + 48);
        fileString += text_file_txt;
        int one = nb.getBuffer()[0] * 100 + nb.getBuffer()[1] * 10 + nb.getBuffer()[2] * 1;
        int two = nb.getBuffer()[3];
        auto isOK = loadCancional(one, two, (char *)dirString.c_str(), (char *)fileString.c_str());
        if (!isOK)
        {
          displayNumberBuffer(nb.getBuffer());
        }
      }
      else
      {
        // only numeric mode
        displayNumberBuffer(nb.getBuffer());
      }
    }
    else if (nb.isText())
    {
      String fileString;
      fileString += (char)nb.getBuffer()[0];
      fileString += (char)nb.getBuffer()[1];
      fileString += (char)nb.getBuffer()[2];
      fileString += text_file_txt;
      auto isOK = loadPsalm((char *)fileString.c_str());
      if (!isOK)
      {
        fileString += text_NOTFOUND;
        errorMessage(fileString.c_str());
      }
    }
  }
}

/*EOF*/
