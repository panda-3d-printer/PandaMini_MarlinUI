/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/************
 * ui_api.h *
 ************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <thread>

#define FORCE_INLINE inline
#define PGM_P char *

//#include "../../inc/MarlinConfig.h"

namespace ExtUI
{
//public:
  enum axis_t     : uint8_t { X, Y, Z };
  enum extruder_t : uint8_t { E0, E1, E2, E3, E4, E5 };
  enum heater_t   : uint8_t { H0, H1, H2, H3, H4, H5, BED };
  enum fan_t      : uint8_t { FAN0, FAN1, FAN2, FAN3, FAN4, FAN5 };

  static constexpr uint8_t extruderCount = 2 ;//EXTRUDERS;
  static constexpr uint8_t hotendCount   = 2 ;//HOTENDS;
  static constexpr uint8_t fanCount      = 4 ;//FAN_COUNT;

  static bool isMoving(){ return false; }
  static bool isAxisPositionKnown(const axis_t){ return false; }
  static bool canMove(const axis_t){ return false; }
  static bool canMove(const extruder_t){ return false; }
  static void enqueueCommands_P(PGM_P const){}

  /**
   * Getters and setters
   * Should be used by the EXTENSIBLE_UI to query or change Marlin's state.
   */
  static PGM_P getFirmwareName_str(){ return "Marlin V2"; }

  float* targetTemps(); //NOTE:测试函数

  static float getActualTemp_celsius(const heater_t){ return rand()%100+1; }
  static float getActualTemp_celsius(const extruder_t){ return rand()%100+1; }
  static float getTargetTemp_celsius(const heater_t h){ return targetTemps()[h]; }
  static float getTargetTemp_celsius(const extruder_t e){ return targetTemps()[e]; }
  static float getTargetFan_percent(const fan_t){ return 11.1; }
  static float getActualFan_percent(const fan_t){ return 11.1; }
  static float getAxisPosition_mm(const axis_t){ return -999.111; }
  static float getAxisPosition_mm(const extruder_t){ return 999.111; }
  static float getAxisSteps_per_mm(const axis_t){ return 11.1; }
  static float getAxisSteps_per_mm(const extruder_t){ return 11.1; }
  static float getAxisMaxFeedrate_mm_s(const axis_t){ return 11.1; }
  static float getAxisMaxFeedrate_mm_s(const extruder_t){ return 11.1; }
  static float getAxisMaxAcceleration_mm_s2(const axis_t){ return 11.1; }
  static float getAxisMaxAcceleration_mm_s2(const extruder_t){ return 11.1; }
  static float getMinFeedrate_mm_s(){ return 999.999; }
  static float getMinTravelFeedrate_mm_s(){ return 11.1; }
  static float getPrintingAcceleration_mm_s2(){ return 11.1; }
  static float getRetractAcceleration_mm_s2(){ return 11.1; }
  static float getTravelAcceleration_mm_s2(){ return 11.1; }
  static float getFeedrate_percent(){ return 11.1; }
  static uint8_t getProgress_percent(){ static uint32_t percent = 0; ++percent; if(percent>100) percent = 0;  return percent; }
  static uint32_t getProgress_seconds_elapsed(){ static uint32_t time = 999999;  return --time; }

  #if HAS_LEVELING
    bool getLevelingActive();
    void setLevelingActive(const bool);
    #if HAS_MESH
      typedef float (&bed_mesh_t)[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
      bool getMeshValid();
      bed_mesh_t getMeshArray();
      void setMeshPoint(const uint8_t xpos, const uint8_t ypos, const float zval);
      void onMeshUpdate(const uint8_t xpos, const uint8_t ypos, const float zval);
    #endif
  #endif

  #ifdef HOST_PROMPT_SUPPORT
    void setHostResponse(const uint8_t);
  #endif

  #ifdef PRINTCOUNTER
    char* getTotalPrints_str(char buffer[21]);
    char* getFinishedPrints_str(char buffer[21]);
    char* getTotalPrintTime_str(char buffer[21]);
    char* getLongestPrint_str(char buffer[21]);
    char* getFilamentUsed_str(char buffer[21]);
  #endif

  static void setTargetTemp_celsius(const float v, const heater_t h)
  {
      targetTemps()[h] = v;
  }

  static void setTargetTemp_celsius(const float v, const extruder_t e)
  {
      targetTemps()[e] = v;
  }

  static void setTargetFan_percent(const float, const fan_t){}
  static void setAxisPosition_mm(const float, const axis_t){}
  static void setAxisPosition_mm(const float, const extruder_t){}
  static void setAxisSteps_per_mm(const float, const axis_t){}
  static void setAxisSteps_per_mm(const float, const extruder_t){}
  static void setAxisMaxFeedrate_mm_s(const float, const axis_t){}
  static void setAxisMaxFeedrate_mm_s(const float, const extruder_t){}
  static void setAxisMaxAcceleration_mm_s2(const float, const axis_t){}
  static void setAxisMaxAcceleration_mm_s2(const float, const extruder_t){}
  static void setFeedrate_mm_s(const float){}
  static void setMinFeedrate_mm_s(const float){}
  static void setMinTravelFeedrate_mm_s(const float){}
  static void setPrintingAcceleration_mm_s2(const float){}
  static void setRetractAcceleration_mm_s2(const float){}
  static void setTravelAcceleration_mm_s2(const float){}
  static void setFeedrate_percent(const float){}
  static void setUserConfirmed(void){}

  #ifdef LIN_ADVANCE
    float getLinearAdvance_mm_mm_s(const extruder_t);
    void setLinearAdvance_mm_mm_s(const float, const extruder_t);
  #endif

  #ifdef JUNCTION_DEVIATION
    float getJunctionDeviation_mm();
    void setJunctionDeviation_mm(const float);
  #else
    static float getAxisMaxJerk_mm_s(const axis_t){return 11.1f; }
    static float getAxisMaxJerk_mm_s(const extruder_t){return 11.1f; }
    static void setAxisMaxJerk_mm_s(const float, const axis_t){}
    static void setAxisMaxJerk_mm_s(const float, const extruder_t){}
  #endif

  static extruder_t getActiveTool(){ return E0; }
  static void setActiveTool(const extruder_t, bool no_move){}

  #ifdef BABYSTEPPING
    int16_t mmToWholeSteps(const float mm, const axis_t axis);

    bool babystepAxis_steps(const int16_t steps, const axis_t axis);
    void smartAdjustAxis_steps(const int16_t steps, const axis_t axis, bool linked_nozzles);
  #endif

  #ifdef HAS_HOTEND_OFFSET
    float getNozzleOffset_mm(const axis_t, const extruder_t);
    void setNozzleOffset_mm(const float, const axis_t, const extruder_t);
    void normalizeNozzleOffset(const axis_t axis);
  #endif

  #ifdef HAS_BED_PROBE
    float getZOffset_mm();
    void setZOffset_mm(const float);
  #endif

  #ifdef ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t);
    void setAxisBacklash_mm(const float, const axis_t);

    float getBacklashCorrection_percent();
    void setBacklashCorrection_percent(const float);

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm();
      void setBacklashSmoothing_mm(const float);
    #endif
  #endif

  #if HAS_FILAMENT_SENSOR
    bool getFilamentRunoutEnabled();
    void setFilamentRunoutEnabled(const bool);

    #if FILAMENT_RUNOUT_DISTANCE_MM > 0
      float getFilamentRunoutDistance_mm();
      void setFilamentRunoutDistance_mm(const float);
    #endif
  #endif

  /**
   * Delay and timing routines
   * Should be used by the EXTENSIBLE_UI to safely pause or measure time
   * safe_millis must be called at least every 1 sec to guarantee time
   * yield should be called within lengthy loops
   */
  #ifdef __SAM3X8E__
    uint32_t safe_millis();
  #else
    static FORCE_INLINE uint32_t safe_millis() { return 1111; } // TODO: Implement for AVR
  #endif

  static void delay_us(unsigned long us){}
  static void delay_ms(unsigned long ms){}
  static void yield(){ }

  /**
   * Media access routines
   *
   * Should be used by the EXTENSIBLE_UI to operate on files
   */
  static bool isMediaInserted(){return true; }
  static bool isPrintingFromMediaPaused(){return true; }
  static bool isPrintingFromMedia(){return true; }
  static bool isPrinting(){return false; }

  static void printFile(const char *filename){}
  static void stopPrint(){}
  static void pausePrint(){}
  static void resumePrint(){}

  class FileList {
    private:
      uint16_t num_files;

    public:
      FileList(){}
      void refresh(){}
      bool seek(const uint16_t, const bool skip_range_check = false){return true; }

      const char *longFilename(){ return "longfilename"; }
      const char *shortFilename(){ return "shortfilename"; }
      const char *filename(){ return "filename"; }
      bool isDir(){ return true; }

      void changeDir(const char * const dirname){}
      void upDir(){}
      bool isAtRootDir(){ return true; }
      uint16_t count(){ return 0; }
  };

  /**
   * Event callback routines
   *
   * Should be declared by EXTENSIBLE_UI and will be called by Marlin
   */
  void onStartup();
  void onIdle();
  void onMediaInserted();
  void onMediaError();
  void onMediaRemoved();
  void onPlayTone(const uint16_t frequency, const uint16_t duration);
  void onPrinterKilled(PGM_P const msg);
  void onPrintTimerStarted();
  void onPrintTimerPaused();
  void onPrintTimerStopped();
  void onFilamentRunout(const ExtUI::extruder_t extruder);
  void onUserConfirmRequired(const char * const msg);
  void onStatusChanged(const char * const msg);
  void onFactoryReset();
  void onStoreSettings();
  void onLoadSettings();
}


/**
 * Helper macros to increment or decrement a value. For example:
 *
 *   UI_INCREMENT_BY(TargetTemp_celsius, 10, E0)
 *
 * Expands to:
 *
 *   setTargetTemp_celsius(getTargetTemp_celsius(E0) + 10, E0);
 *
 * Or, in the case where a constant increment is desired:
 *
 *   constexpr float increment = 10;
 *
 *   UI_INCREMENT(TargetTemp_celsius, E0)
 *
 */
#define UI_INCREMENT_BY(method, inc, ...) ExtUI::set ## method(ExtUI::get ## method (__VA_ARGS__) + inc, ##__VA_ARGS__)
#define UI_DECREMENT_BY(method, inc, ...) ExtUI::set ## method(ExtUI::get ## method (__VA_ARGS__) - inc, ##__VA_ARGS__)

#define UI_INCREMENT(method, ...) UI_INCREMENT_BY(method, increment, ##__VA_ARGS__)
#define UI_DECREMENT(method, ...) UI_DECREMENT_BY(method, increment, ##__VA_ARGS__)
