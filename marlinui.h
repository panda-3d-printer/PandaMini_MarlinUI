#ifndef MARLINUI_H
#define MARLINUI_H

#include <stdint.h>
#include <lv_conf.h>
#include <lvsignal.hpp>
#include <ui_api.h>
#include <lvgl/lv_fonts/lv_font_builtin.h>
#include <lvgl/lv_core/lv_obj.h>
#include "messagescreen.h"
#include <lvmath.h>

class LVObject;
class LVTask;

class Screen;
class HomeScreen;
class SplashScreen;
class MenuScreen;
class PrintingScreen;
class TemperatureScreen;
class MotionScreen;
class ExtrudeScreen;
class FileScreen;
class MessageScreen;
class NumberImputScreen;
class KilledScreen;
class CheckScreen;


//class Margin
//{
//public:
//    int16_t left = 5;
//    int16_t right = 5;
//    int16_t top = 5;
//    int16_t bottom = 5;
//};

/**
 * @brief 信号的参数
 */
struct PlayToneParam
{
    PlayToneParam(){}
    PlayToneParam(const uint16_t frequency, const uint16_t duration)
        :frequency(frequency),duration(duration)
    {}
    uint16_t frequency;
    uint16_t duration;
};

struct FilamentRunoutParam
{
    FilamentRunoutParam(){}
    FilamentRunoutParam(const ExtUI::extruder_t extruder)
        :extruder(extruder)
    {}

    ExtUI::extruder_t extruder;
};

struct MesgParam
{
    MesgParam(){}
    MesgParam(const char * const msg)
        :msg(msg)
    {}
    const char * msg;
};


///////  一些常用的按钮函数  ////////////////////////////////////////
//返回主界面
lv_res_t onButBackHomeClicked (struct _lv_obj_t * obj);
//返回菜单界面
lv_res_t onButBackMenuClicked (struct _lv_obj_t * obj);


/**
 * @brief Marlin 的 Ui ,
 * 它不显示任何具体GUI
 * 但管理着许多的屏幕界面,
 * 协调之间的相互逻辑关系
 *
 * 运行一些常规的基础任务
 * 刷新温度
 * 刷新坐标
 * 刷新SD卡状态
 * 刷新错误状态
 *
 * 由于界面需要适应多尺寸屏幕,采用占比分配大小,同时有最小最大尺寸限制
 * 默认屏幕尺寸
 * 4.3寸 480 X 272
 * 3.5寸 480 x 320
 * 3.2寸 320 x 240
 * 2.8寸 320 x 240
 * 2.4寸
 *
 */
class MarlinUi
{
    LV_MEMAORY_FUNC
protected:

    static MarlinUi * s_instance; //NOTE:将静态变量移动到静态函数中

    uint16_t m_width ;  //!< 屏幕像素宽度
    uint16_t m_height;  //!< 屏幕像素高度
    uint16_t m_dpi;     //!< 屏幕1英寸像素个数

    //Margin m_margin;    //!< 屏幕安全边界

    ///////////// 字体 //////////////////
    lv_font_t & m_defaultFont;

    ///////////界面屏幕/////////////////

    SplashScreen * m_splashScreen = nullptr; //!< 启动欢迎屏 [splash]

    HomeScreen * m_homeScreen = nullptr;    //!< 主界面 [home]
    TemperatureScreen * m_temperatureScreen = nullptr; //!< 温度界面 [temp]
    MotionScreen * m_motionScreen = nullptr;    //!< 运动界面 [motion]
    ExtrudeScreen * m_extrudeScreen = nullptr;     //!< 挤出界面 [extrude]
    FileScreen * m_fileScreen = nullptr;        //!< 文件浏览界面 [file]

    MenuScreen * m_menuScreen = nullptr;    //!< 菜单界面 [menu]
    CheckScreen * m_checkScreen = nullptr;  //!< 检查界面[check]
    PrintingScreen * m_printingScreen = nullptr;   //!< 打印界面 [printing]

    MessageScreen * m_messageScreen = nullptr; //!< 信息确认界面 [message]
    NumberImputScreen * m_numberImputScreen = nullptr; //!< 数字输入界面 [numberImput]


    /////////// 界面管理 /////////////////////
    Screen * m_defaultScreen = nullptr; //!< 默认的屏幕
    Screen * m_currentScreen = nullptr; //!< 当前活动的屏幕

    ///////////界面任务/////////////////
    // 位置 温度 SD状态 限位开关 错误状态
    LVTask * m_positionTask     = nullptr;
    LVTask * m_temperatureTask  = nullptr;
    LVTask * m_sdCardTask       = nullptr;
    LVTask * m_endStopTask      = nullptr;
    LVTask * m_errorTask        = nullptr;
    LVTask * m_remotePrintingTask = nullptr;

    ////////////Marlin Event Signal////////////

    LVSignal m_signalStartup;
    LVSignal m_signalIdle;
    LVSignal m_signalMediaInserted;
    LVSignal m_signalMediaError;
    LVSignal m_signalMediaRemoved;
    LVSignal m_signalPlayTone;
    LVSignal m_signalPrinterKilled;
    LVSignal m_signalPrintTimerStarted;
    LVSignal m_signalPrintTimerPaused;
    LVSignal m_signalPrintTimerStopped;
    LVSignal m_signalFilamentRunout;
    LVSignal m_signalUserConfirmRequired;
    LVSignal m_signalStatusChanged;
    LVSignal m_signalFactoryReset;
    LVSignal m_signalStoreSettings;
    LVSignal m_signalLoadSettings;

    //消息提示气泡槽
    LVSlot m_slotNotifyBubble;
    LVSignal m_signalRemotePrinting;

protected:
    /**
     * @brief MarlinUi
     * @param width  界面宽度
     * @param height 界面高度
     * @param dpi
     */
    MarlinUi(uint16_t width = LV_HOR_RES, uint16_t height = LV_VER_RES,uint16_t dpi = LV_DPI);

public:
    static MarlinUi *getInstance(uint16_t width = LV_HOR_RES, uint16_t height = LV_VER_RES,uint16_t dpi = LV_DPI);

    ~MarlinUi();

    /**
     * @brief 在对UI有任何操作之前需要先进行初始化
     */
    void initMarlinUI();

    /**
     * @brief 显示欢迎界面
     */
    void showSplashScreen();

    /**
     * @brief 显示主界面
     */
    void showHomeScreen();

    /**
     * @brief 显示工具菜单界面
     */
    void showMenuScreen();

    /**
     * @brief 显示打印界面
     */
    void showPrintingScreen();

    /**
     * @brief 显示温度界面
     */
    void showTemperatureScreen();

    /**
     * @brief 显示运动界面
     */
    void showMotionScreen();

    /**
     * @brief 显示挤出界面
     */
    void showExtrudeScreen();

    /**
     * @brief 显示文件界面
     */
    void showFileScreen();

    /**
     * @brief 显示消息界面
     */
    void showMessageScreen();

    /**
     * @brief 显示数字输入界面
     */
    void showNumberInputScreen();

    void showCheckScreen();

    uint16_t width() const;
    void setWidth(uint16_t width);

    uint16_t height() const;
    void setHeight(uint16_t height);

    uint16_t dpi() const;
    void setDpi(uint16_t dpi);

    //Margin margin(){ return m_margin;}

    SplashScreen * splashScreen();
    HomeScreen * homeScreen();
    TemperatureScreen * temperatureScreen();
    MenuScreen * menuScreen();
    PrintingScreen * printingScreen();
    FileScreen * fileScreen();

    MessageScreen *messageScreen() const;
    NumberImputScreen *numberImputScreen() const;

    Screen * defaultScreen();
    Screen * currentScreen();

    //////////////////// Event callback routines //////////////////////

    LVSignal & getSignalStartup();
    LVSignal & getSignalIdle();
    LVSignal & getSignalMediaInserted();
    LVSignal & getSignalMediaError();
    LVSignal & getSignalMediaRemoved();
    LVSignal & getSignalPlayTone();
    LVSignal & getSignalPrinterKilled();
    LVSignal & getSignalPrintTimerStarted();
    LVSignal & getSignalPrintTimerPaused();
    LVSignal & getSignalPrintTimerStopped();
    LVSignal & getSignalFilamentRunout();
    LVSignal & getSignalUserConfirmRequired();
    LVSignal & getSignalStatusChanged();
    LVSignal & getSignalFactoryReset();
    LVSignal & getSignalStoreSettings();
    LVSignal & getSignalLoadSettings();
    LVSignal & getSignalRemotePrinting();


    lv_font_t &defaultFont() const;

    LVTask *getRemotePrintingTask() const;

protected:
    void initScreens();
    void initTasks();

};

#endif // MARLINUI_H
