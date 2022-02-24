

//
// vim: ts=4 et
// Copyright (c) 2001 - 2021 Petr Vanek
//
/// @file   configuration.h
/// @author Petr Vanek

#pragma once

#include <inttypes.h>
#include <string.h>
#include <EEPROM.h>

/**
 * @brief configuration object wrapping EEPROM storing the display configuration
 * 
 */
class Configuration {

#define EEPROM_SIZE 2
  
  public:

    /**
     * @brief ctor 
     * 
     */
    Configuration() {
    }

    /**
     * @brief initialization of work with EEPROM
     * 
     */
    void init() {
       EEPROM.begin(EEPROM_SIZE);
    }
    
    /**
     * @brief Loading the current valid configuration
     * 
     */
    void refresh() {
      _colorSchema = EEPROM.read(0);
      _cantioMode = EEPROM.read(1);
    }
    
    /**
     * @brief Saves a valid colour scheme
     * 
     * @param newschema - numeric ID of color scheme
     */
    void setColorSchema(uint8_t newschema) {
      _colorSchema = newschema;
      EEPROM.write(0, newschema);
      EEPROM.commit();
    }

    /**
     * @brief settings for displaying verses of hymn
     * 
     * @param newmode 
     */
    void setCantiMode(uint8_t newmode) {
      _cantioMode = newmode;
      EEPROM.write(1, newmode);
      EEPROM.commit();
    }

    /**
     * @brief Get the Color Schema value
     * 
     * @return uint8_t 
     */
    uint8_t getColorSchema() {
      return _colorSchema;
    }

    /**
     * @brief Get the verses of hymn mode
     * 
     * @return uint8_t 
     */
     uint8_t getCantiMode() {
      return _cantioMode;
    }
    
  private:
    uint8_t _colorSchema = 0;    ///< color schema
    uint8_t _cantioMode = 0;     ///< verses of hymn mode
  
};


