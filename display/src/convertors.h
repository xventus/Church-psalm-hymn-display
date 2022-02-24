//
// vim: ts=4 et
// Copyright (c) 2001 - 2021 Petr Vanek
//
/// @file   convertor.h
/// @author Petr Vanek

#pragma once

#include <inttypes.h>
#include <fabgl.h>

/**
 * @brief converts ASCII value to position in glyph table
 * 
 * @param chr -  earch character
 * @return unsigned char position in the table
 */
uint8_t charConvertor(uint8_t chr)
{

    auto decodedto = chr;

    if (chr < 32) // to zero
    {
        decodedto = 0;              //
    }
    else if (chr >= 32 && chr < 126) // space to tilda
    {
        decodedto = chr - 32;       // 
    }
    else
    {

        // Czech character decoder
        switch (chr)
        {
        case 0xcc:
            decodedto = 96;
            break; // Ě
        case 0x8a:
            decodedto = 97;
            break; // Š
        case 0xc8:
            decodedto = 98;
            break; // Č
        case 0xd8:
            decodedto = 99;
            break; // Ř
        case 0x8e:
            decodedto = 100;
            break; // Ž
        case 0xdd:
            decodedto = 101;
            break; // Ý
        case 0xc1:
            decodedto = 102;
            break; // Á
        case 0xcd:
            decodedto = 103;
            break; // Í
        case 0xc9:
            decodedto = 104;
            break; // É
        case 0xda:
            decodedto = 105;
            break; // Ú
        case 0xd9:
            decodedto = 106;
            break; // Ů
        case 0x8d:
            decodedto = 107;
            break; // Ť
        case 0xec:
            decodedto = 108;
            break; // ě
        case 0x9a:
            decodedto = 109;
            break; // š
        case 0xe8:
            decodedto = 110;
            break; // č
        case 0xf8:
            decodedto = 111;
            break; // ř
        case 0x9e:
            decodedto = 112;
            break; // ž
        case 0xfd:
            decodedto = 113;
            break; // ý
        case 0xe1:
            decodedto = 114;
            break; // á
        case 0xed:
            decodedto = 115;
            break; // í
        case 0xe9:
            decodedto = 116;
            break; // é
        case 0xfa:
            decodedto = 117;
            break; // ú
        case 0xf9:
            decodedto = 118;
            break; // ů
        case 0x9d:
            decodedto = 119;
            break; // ť
        case 0xcf:
            decodedto = 120;
            break; // Ď
        case 0xef:
            decodedto = 121;
            break; // ď
        case 0xf2:
            decodedto = 122;
            break; // ň
        case 0xd2:
            decodedto = 123;
            break; // Ň
        case 0xf3:  // Unfortunately I don't have it in the glyph set I'm switching to without diacritics
            decodedto = 79; // 111 - 32
            break; // ó

        case 0xd3: // Unfortunately I don't have it in the glyph set I'm switching to without diacritics
            decodedto = 47; // 79 - 32
            break; // Ó
            
        default:
            decodedto = 0;
            break;
        }
    }

    if (decodedto > 123)
    {
        // prevent out of table
        decodedto = 0;
    }

    return decodedto;
}

/**
 * @brief converts virtual key codes to numeric or ASCII values + special commands
 * 
 * @param vk  - scan code
 * @return int - value
 */
int virtualKeyToNumber(VirtualKey vk) {
  
  if (vk == fabgl::VK_KP_MULTIPLY)
    return 201; // *
  if (vk == fabgl::VK_KP_MINUS)
    return 202; // -
  if (vk == fabgl::VK_KP_PLUS)
    return 203; // +
  if (vk == fabgl::VK_KP_ENTER)
    return 200; // ENTER
  if (vk == fabgl::VK_KP_PERIOD)
    return 204; // . DOT
  if (vk == fabgl::VK_KP_DIVIDE)
    return 205; //  /  SLASH

  if (vk == fabgl::VK_KP_0)
    return 0;
  if (vk == fabgl::VK_KP_1)
    return 1;
  if (vk == fabgl::VK_KP_2)
    return 2;
  if (vk == fabgl::VK_KP_3)
    return 3;
  if (vk == fabgl::VK_KP_4)
    return 4;
  if (vk == fabgl::VK_KP_5)
    return 5;
  if (vk == fabgl::VK_KP_6)
    return 6;
  if (vk == fabgl::VK_KP_7)
    return 7;
  if (vk == fabgl::VK_KP_8)
    return 8;
  if (vk == fabgl::VK_KP_9)
    return 9;

  if (vk == fabgl::VK_0)
    return '0';
  if (vk == fabgl::VK_1)
    return '1';
  if (vk == fabgl::VK_2)
    return '2';
  if (vk == fabgl::VK_3)
    return '3';
  if (vk == fabgl::VK_4)
    return '4';
  if (vk == fabgl::VK_5)
    return '5';
  if (vk == fabgl::VK_6)
    return '6';
  if (vk == fabgl::VK_7)
    return '7';
  if (vk == fabgl::VK_8)
    return '8';
  if (vk == fabgl::VK_9)
    return '9';

  return -1;
}