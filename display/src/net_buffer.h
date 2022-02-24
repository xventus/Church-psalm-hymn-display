
//
// vim: ts=4 et
// Copyright (c) 2019 Petr Vanek, petr@fotoventus.cz
//
/// @file   net_buffer.h
/// @author Petr Vanek

#pragma once

#include <inttypes.h>
#include <string.h>

class NetBuffer
{

#define VW_MAX_MESSAGE_LEN 5
#define MAX_DIGIT 4

public:
    NetBuffer()
    {
        init();
    }

    void init()
    {
        stepnum = 0;
        hello = true;
        number = false;
        changed = false;
    }

    void shiftnumber(uint8_t shi)
    {
        for (uint8_t c = 0; c < shi; c++)
        {
            for (uint8_t p = (MAX_DIGIT - 1); p > 0; p--)
            {
                buf[p] = buf[p - 1];
            }
            buf[c] = 0;
        }
    }

    void seyHello()
    {
        init();
        memset(oldb, 0, VW_MAX_MESSAGE_LEN);
        memset(buf, 0, VW_MAX_MESSAGE_LEN);
        changed = true;
    }

    void addByteToParse(uint8_t incomingByte)
    {
        changed = false;

        /*
        if (vk == fabgl::VK_KP_MULTIPLY)   return 201;  // *
        if (vk == fabgl::VK_KP_MINUS)       return 202;  // -
        if (vk == fabgl::VK_KP_PLUS)        return 203;  // +
        if (vk == fabgl::VK_KP_ENTER )      return 200;  // ENTER
        if (vk == fabgl::VK_KP_PERIOD)      return 204;  // . DOT
        if (vk == fabgl::VK_KP_DIVIDE)     return 205;   // /  SLASH
        */

        if (incomingByte >= 200 && incomingByte <= 210)
        {
            // special command

            if (incomingByte == 203)
            {
                // increment last position

                if (stepnum == 0)
                {
                    // copy buffer and prepare only chart part
                    oldb[MAX_DIGIT - 1]++;
                    if (oldb[MAX_DIGIT - 1] > 9)
                        oldb[MAX_DIGIT - 1] = 0;
                    memcpy(buf, oldb, MAX_DIGIT);
                    stepnum = 0;
                    changed = true;
                    number = true;
                    hello = false;
                }
            }

            if (incomingByte == 202)
            {
                // decrement last position

                if (stepnum == 0)
                {
                    // copy buffer and prepare only chart part
                    if (oldb[MAX_DIGIT - 1] <= 0)
                        oldb[MAX_DIGIT - 1] = 9;
                    else
                        oldb[MAX_DIGIT - 1]--;

                    memcpy(buf, oldb, MAX_DIGIT);
                    stepnum = 0;
                    changed = true;
                    number = true;
                    hello = false;
                }
            }

            if (incomingByte == 204)
            {
                // dot
                if (stepnum == 0)
                {
                    // copy buffer and prepare only chart part
                    memcpy(buf, oldb, VW_MAX_MESSAGE_LEN);
                    stepnum = MAX_DIGIT - 1; // position set to dot (last digit)
                }
                else
                {
                    shiftnumber((MAX_DIGIT - 1) - stepnum);
                    stepnum = MAX_DIGIT - 1;
                }
            }

            if (incomingByte == 201)
            {
                init();
                memset(oldb, 0, VW_MAX_MESSAGE_LEN);
                memset(buf, 0, VW_MAX_MESSAGE_LEN);
                changed = true;
            }

            if (incomingByte == 200)
            {
                if (stepnum == 0)
                    return; // ignore

                // ENTER
                shiftnumber((MAX_DIGIT - 1) - stepnum);

                bool det = false;
                for (uint8_t b = 0; b < MAX_DIGIT; b++)
                {
                    if (buf[b] > 9)
                        det = true;
                }

                if (det)
                    memset(buf, 0, VW_MAX_MESSAGE_LEN); // mixed data

                stepnum = 0;

                hello = true;
                for (auto i = 0; i < VW_MAX_MESSAGE_LEN; i++)
                {
                    if (buf[i] != 0)
                    {
                        hello = false;
                        break;
                    }
                }

                changed = true;
                number = true;
                memcpy(oldb, buf, VW_MAX_MESSAGE_LEN);
                memset(buf, 0, VW_MAX_MESSAGE_LEN);
            }

            if (incomingByte == 205)
            {
                //  SLASH text mode
                shiftnumber((MAX_DIGIT - 1) - stepnum);
                stepnum = 0;
                for (uint8_t b = 0; b < MAX_DIGIT; b++)
                {
                    if (buf[b] < 10)
                        buf[b] += 48; // ASCII '0' // last number must bealways zero !!!!
                }

                memcpy(oldb, buf, VW_MAX_MESSAGE_LEN);
                memset(buf, 0, VW_MAX_MESSAGE_LEN);

                hello = false;
                changed = true;
                number = false;
            }
        }
        else if (incomingByte < 10)
        {
            // numeric mode
            number = true;
            buf[stepnum] = incomingByte;
            stepnum++;

            if (stepnum >= 4)
            {
                // detect if all input is zero
                stepnum = 0;
                hello = true;
                for (auto i = 0; i < VW_MAX_MESSAGE_LEN; i++)
                {
                    if (buf[i] != 0)
                    {
                        hello = false;
                        break;
                    }
                }

                if (memcmp(oldb, buf, VW_MAX_MESSAGE_LEN))
                {
                    // COPY ONLY IF DATA CHANGED!
                    memcpy(oldb, buf, VW_MAX_MESSAGE_LEN);
                    changed = true;
                }
                
                memset(buf, 0, VW_MAX_MESSAGE_LEN);
            }
        }
        else if (incomingByte > 46 && incomingByte < 58)
        {
            // INCOMMING NUMBER CODED AS ASCII   XXX with last symbol is slash 47 '/'
            // valid ascii 48 (0) to 57 (9)
            number = false;
            buf[stepnum] = incomingByte;
            stepnum++;

            if (stepnum >= 4)
            {
                stepnum = 0;
                hello = true;
                for (auto i = 0; i < 3; i++)
                {
                    if (buf[i] != 48)
                    {
                        hello = false;
                        break;
                    }
                }

                stepnum = 0;
                if (memcmp(oldb, buf, VW_MAX_MESSAGE_LEN))
                {
                    memcpy(oldb, buf, VW_MAX_MESSAGE_LEN);

                    changed = true;
                    if (number == true)
                    {
                        number = false; // switch to text mode
                    }
                }
                memset(buf, 0, VW_MAX_MESSAGE_LEN);
            }
        }
    }

    void cancelChange() { changed = false; }
    bool isHello() { return hello; }
    bool isText() { return !number; }
    bool isNumber() { return number; }
    bool isChanged() { return changed; }
    uint8_t *getBuffer() { return &oldb[0]; }

private:
#define MAX_DIGIT 4
#define MAX_FILENAME 8
    uint8_t stepnum = 0;
    bool hello = false;  // show hello
    bool number = false; // number mode - cancional
    bool changed = false;
    
    // message from keyboard
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t oldb[VW_MAX_MESSAGE_LEN];
};
