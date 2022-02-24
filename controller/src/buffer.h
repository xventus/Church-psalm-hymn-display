//
// vim: ts=4 et
// Copyright (c) 2001 - 2021 Petr Vanek
//
/// @file   buffer.h
/// @author Petr Vanek

#pragma once

#include <inttypes.h>
#include <string.h>
#include <PS2KeyAdvanced.h>   // CONSTANTS SUCH AS PS2_KEY_XXX

// Defines the maximum number of digits. 
// Three digits are used to display the song and the fourth digit to display the verse.
#define MAX_DIGIT  4


/**
 * @brief Wrapping up the data work. 
 * The values of the individual digits (positions) 
 * take on either numeric values 0-9 in case they are hymn numbers.  
 * Or ASCII numeric values if it is a psalm number. 
 * 
 */
class Buff {

public:

    /**
     * @brief Construct a new Buff object
     * 
     */
    Buff() {
        clear();
    }

    /**
     * @brief hymn buffer initialization
     * 
     * @param ball  - if true - shadow will be init too
     */
    void init(bool ball) {
        memset(numbuffer, 0, sizeof(uint8_t) * MAX_DIGIT);
        num_position = 0;
        if (ball == true) memset(numbuffer_shadow, 0, sizeof(uint8_t) * MAX_DIGIT);
    }

    
    /**
     * @brief Get value from position
     * 
     * @param pos 
     * @return uint8_t 
     */
    uint8_t getNumber(uint8_t pos) {
        return numbuffer[pos];
    }

    /**
     * @brief inserting individual numbers from the keyboard, or operations over the keyboard keys
     * 
     * @param cc - key code
     * @return true - Accepted key
     * @return false - Ignored key
     */
    bool push(int cc) {

        auto rc = true;
        auto k = cc & 0xFF;
        auto s = cc >> 8;
        auto c = -1;

        if (s & 0x80) return false;

        if (k == PS2_KEY_KP0) c = 0;
        if (k == PS2_KEY_KP1) c = 1;
        if (k == PS2_KEY_KP2) c = 2;
        if (k == PS2_KEY_KP3) c = 3;
        if (k == PS2_KEY_KP4) c = 4;
        if (k == PS2_KEY_KP5) c = 5;
        if (k == PS2_KEY_KP6) c = 6;
        if (k == PS2_KEY_KP7) c = 7;
        if (k == PS2_KEY_KP8) c = 8;
        if (k == PS2_KEY_KP9) c = 9;


        if (c <= 9 && c >= 0) {
            //numeric add
            numbuffer[num_position] = c;    //assign number to right position
            if (num_position < MAX_DIGIT) num_position++;    //next position if still not end
        }
        else if (k == PS2_KEY_BS) {
            //BACKSPACE - delete all info
            init(true);
        }
        else if (k == PS2_KEY_KP_TIMES) {
            //ASTERIX - send zero to display
            init(true);
            num_position = MAX_DIGIT;
        }
        else if (k == PS2_KEY_KP_DIV) {
            // SLASH - send psalm
            if (num_position == 0) {
                //copy buffer and prepare only chart part
                memcpy(numbuffer, txtbuffer_shadow, MAX_DIGIT);
                num_position = MAX_DIGIT;

                // conversion to psalm number 
                for (uint8_t b = 0;b < MAX_DIGIT;b++) {
                    numbuffer[b] += 48; // ASCII '0' // last number must bealways zero !!!!
                }
            }
            else {
                shiftDigit((MAX_DIGIT - 1) - num_position);
                num_position = MAX_DIGIT;
                memcpy(txtbuffer_shadow, numbuffer, MAX_DIGIT);

                // conversion to psalm number 
                for (uint8_t b = 0;b < MAX_DIGIT;b++) {
                    numbuffer[b] += 48; // ASCII '0' // last number must be always zero !!!!
                }
            }

        }
        else if (k == PS2_KEY_KP_MINUS) {
            // MINUS- 
            //decrement last position
            if (num_position == 0)
            {//copy buffer and prepare only chart part
                if (numbuffer_shadow[MAX_DIGIT - 1] <= 0) numbuffer_shadow[MAX_DIGIT - 1] = 9;
                else
                    numbuffer_shadow[MAX_DIGIT - 1] --;

                memcpy(numbuffer, numbuffer_shadow, MAX_DIGIT);
                num_position = MAX_DIGIT;  //final
            }
        }
        else if (k == PS2_KEY_KP_PLUS) {
            // PLUS+ 
            if (num_position == 0) {
                //copy buffer and prepare only chart part
                numbuffer_shadow[MAX_DIGIT - 1] ++;
                if (numbuffer_shadow[MAX_DIGIT - 1] > 9) numbuffer_shadow[MAX_DIGIT - 1] = 0;
                memcpy(numbuffer, numbuffer_shadow, MAX_DIGIT);
                num_position = MAX_DIGIT;  //final 
            }
        }
        else if (k == PS2_KEY_KP_ENTER) {
            //ENTER - send to dispaly
            if (num_position == 0) {
                //copy buffer and prepare only chart part
                memcpy(numbuffer, numbuffer_shadow, MAX_DIGIT);
                num_position = MAX_DIGIT;
            }
            else {
                shiftDigit((MAX_DIGIT - 1) - num_position);
                num_position = MAX_DIGIT;
            }
        }
        else if (k == PS2_KEY_KP_DOT) {
            // DOT . 
            if (num_position == 0)
            {//copy buffer and prepare only chart part
                memcpy(numbuffer, numbuffer_shadow, MAX_DIGIT);
                num_position = MAX_DIGIT - 1;   //position set to dot (last digit)
            }
            else
            {
                shiftDigit((MAX_DIGIT - 1) - num_position);
                num_position = MAX_DIGIT - 1;
            }
        }
        else {
            // skip  - unknown code   
            rc = false;
        }
        return rc;
    }

    /**
     * @brief Question if I can send data from the buffer over the serial line
     * 
     * @return true  - data ready to send
     * @return false  - not yet
     */
    bool isExec() {
        return (num_position == MAX_DIGIT);
    }

    /**
     * @brief Get the Cursor Position
     * 
     * @return uint8_t 
     */
    uint8_t getCursorPos() {
        return num_position;
    }

    /**
     * @brief stores only numeric values not ASCII for TEXT display
     * Used for quick switching between hymns and psalm
     * 
     */
    void copyToShadow() {
        bool bfnd = false;
      
        for (uint8_t b = 0;b < MAX_DIGIT;b++) {
            if (numbuffer[b] > 9) {
                bfnd = true;
                break;
            }
        }

        if (bfnd == false) {
            memcpy(numbuffer_shadow, numbuffer, MAX_DIGIT);
        }

        init(false);
    }

    /**
     * @brief  shift of individual digits in the buffer when entering 
     * 
     * @param shi 
     */
    void shiftDigit(uint8_t shi) {
        for (uint8_t c = 0; c < shi; c++) {
            for (uint8_t p = (MAX_DIGIT - 1);p > 0;p--) {
                numbuffer[p] = numbuffer[p - 1];
            }
            numbuffer[c] = 0;
        }
    }

    /**
     * @brief delete all buffers 
     * 
     */
    void clear() {
        num_position = 0;
        memset(numbuffer, 0, MAX_DIGIT);
        memset(numbuffer_shadow, 0, MAX_DIGIT);
        memset(txtbuffer, 0, MAX_DIGIT);
        memset(txtbuffer_shadow, 0, MAX_DIGIT);
    }

private:
  
    uint8_t  numbuffer[MAX_DIGIT];              ///< working buffer for storing input numbers for hymns 
    uint8_t  numbuffer_shadow[MAX_DIGIT];       ///< shadow hymn buffer 
    uint8_t  txtbuffer[MAX_DIGIT];              ///< psalm working buffer
    uint8_t  txtbuffer_shadow[MAX_DIGIT];       ///< psalm shadow buffer
    uint8_t  num_position = 0;                  //>> actual digit position

};