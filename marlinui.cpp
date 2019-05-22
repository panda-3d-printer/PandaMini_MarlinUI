#include "marlinui.h"
#include <lvgl>
#include "homescreen.h"
#include "menuscreen.h"
#include "splashscreen.h"
#include "menuscreen.h"
#include "printingscreen.h"
#include "temperaturescreen.h"
#include "motionscreen.h"
#include "extrudescreen.h"
#include "filescreen.h"
#include <langchinese.h>
#include <langenglish.h>
#include "themedark.h"
#include "resources.h"
#include "numberimputscreen.h"
//#include "themelight.h"

void marlinUiInit(void);
void marlinUiRun(void);

namespace ExtUI{
    //实现Marlin事件函数
    void onStartup            ()
    {
        //初始化界面
        marlinUiInit();
        MarlinUi::getInstance()->getSignalStartup            ().emit();
    }
    void onIdle               ()
    {
        //运行界面
        marlinUiRun();
        MarlinUi::getInstance()->getSignalIdle               ().emit();
    }

    void onMediaInserted      ()
    {
        MarlinUi::getInstance()->getSignalMediaInserted      ().emit();
    }

    void onMediaError         ()
    {
        MarlinUi::getInstance()->getSignalMediaError         ().emit();
    }
    void onMediaRemoved       ()
    {
        MarlinUi::getInstance()->getSignalMediaRemoved       ().emit();
    }

    void onPlayTone           (const uint16_t frequency, const uint16_t duration)
    {
        static PlayToneParam param;
        param.frequency = frequency;
        param.duration = duration;

        MarlinUi::getInstance()->getSignalPlayTone().emit(&param);
    }
    void onPrinterKilled      (PGM_P const msg)
    {
        static MesgParam param;
        param.msg = msg;
        MarlinUi::getInstance()->getSignalPrinterKilled      ().emit(&param);
    }

    void onPrintTimerStarted  ()
    {
        MarlinUi::getInstance()->getSignalPrintTimerStarted  ().emit();
    }

    void onPrintTimerPaused   ()
    {
        MarlinUi::getInstance()->getSignalPrintTimerPaused   ().emit();
    }

    void onPrintTimerStopped  ()
    {
        MarlinUi::getInstance()->getSignalPrintTimerStopped  ().emit();
    }

    void onFilamentRunout     (const ExtUI::extruder_t extruder)
    {
        static FilamentRunoutParam param;
        param.extruder = extruder;
        MarlinUi::getInstance()->getSignalFilamentRunout     ().emit(&param);
    }

    void onUserConfirmRequired(const char * const msg)
    {
        static MesgParam param;
        param.msg = msg;
        MarlinUi::getInstance()->getSignalUserConfirmRequired().emit(&param);
    }

    void onStatusChanged      (const char * const msg)
    {
        static MesgParam param;
        param.msg = msg;
        MarlinUi::getInstance()->getSignalStatusChanged      ().emit(&param);
    }

    void onFactoryReset       ()
    {
        MarlinUi::getInstance()->getSignalFactoryReset       ().emit();
    }

    void onStoreSettings      ()
    {
        MarlinUi::getInstance()->getSignalStoreSettings      ().emit();
    }

    void onLoadSettings       ()
    {
        MarlinUi::getInstance()->getSignalLoadSettings       ().emit();
    }

    /////////////////////////////新添加API///////////////////////////
    void onStoreSettings(char *buff) {
      // This is called when saving to EEPROM (i.e. M500). If the ExtUI needs
      // permanent data to be stored, it can write up to eeprom_data_size bytes
      // into buff.

      // Example:
      //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
      //  memcpy(buff, &myDataStruct, sizeof(myDataStruct));
    }

    void onLoadSettings(const char *buff) {
      // This is called while loading settings from EEPROM. If the ExtUI
      // needs to retrieve data, it should copy up to eeprom_data_size bytes
      // from buff

      // Example:
      //  static_assert(sizeof(myDataStruct) <= ExtUI::eeprom_data_size);
      //  memcpy(&myDataStruct, buff, sizeof(myDataStruct));
    }

    void onConfigurationStoreWritten(bool success) {
      // This is called after the entire EEPROM has been written,
      // whether successful or not.
    }

    void onConfigurationStoreRead(bool success) {
      // This is called after the entire EEPROM has been read,
      // whether successful or not.
    }

}

MarlinUi * MarlinUi::s_instance = nullptr;

LVSignal & MarlinUi::getSignalStartup()
{
    return m_signalStartup;
}

LVSignal & MarlinUi::getSignalIdle()
{
    return m_signalIdle;
}

LVSignal & MarlinUi::getSignalMediaInserted()
{
    return m_signalMediaInserted;
}

LVSignal & MarlinUi::getSignalMediaError()
{
    return m_signalMediaError;
}

LVSignal & MarlinUi::getSignalMediaRemoved()
{
    return m_signalMediaRemoved;
}

LVSignal & MarlinUi::getSignalPlayTone()
{
    return m_signalPlayTone;
}

LVSignal & MarlinUi::getSignalPrinterKilled()
{
    return m_signalPrinterKilled;
}

LVSignal & MarlinUi::getSignalPrintTimerStarted()
{
    return m_signalPrintTimerStarted;
}

LVSignal & MarlinUi::getSignalPrintTimerPaused()
{
    return m_signalPrintTimerPaused;
}

LVSignal & MarlinUi::getSignalPrintTimerStopped()
{
    return m_signalPrintTimerStopped;
}

LVSignal & MarlinUi::getSignalFilamentRunout()
{
    return m_signalFilamentRunout;
}

LVSignal & MarlinUi::getSignalUserConfirmRequired()
{
    return m_signalUserConfirmRequired;
}

LVSignal & MarlinUi::getSignalStatusChanged()
{
    return m_signalStatusChanged;
}

LVSignal & MarlinUi::getSignalFactoryReset()
{
    return m_signalFactoryReset;
}

LVSignal & MarlinUi::getSignalStoreSettings()
{
    return m_signalStoreSettings;
}

LVSignal & MarlinUi::getSignalLoadSettings()
{
    return m_signalLoadSettings;
}

lv_font_t &MarlinUi::defaultFont() const
{
    return m_defaultFont;
}

MessageScreen *MarlinUi::messageScreen() const
{
    return m_messageScreen;
}

NumberImputScreen *MarlinUi::numberImputScreen() const
{
    return m_numberImputScreen;
}

Screen *MarlinUi::defaultScreen(){ return m_defaultScreen; }

Screen *MarlinUi::currentScreen(){ return m_currentScreen; }

LVSignal &MarlinUi::getSignalRemotePrinting()
{
    return m_signalRemotePrinting;
}

LVTask *MarlinUi::getRemotePrintingTask() const
{
    return m_remotePrintingTask;
}

MarlinUi::MarlinUi(uint16_t width, uint16_t height, uint16_t dpi)
    :m_width(width)
    ,m_height(height)
    ,m_dpi(dpi)
    ,m_defaultFont(*LV_FONT_DEFAULT)
{

}

MarlinUi * MarlinUi::getInstance(uint16_t width, uint16_t height,uint16_t dpi)
{
    //static MarlinUi * s_instance = nullptr;

    if(s_instance == nullptr)
    {
        s_instance = new MarlinUi(width,height,dpi);
    }
    return  s_instance;
}

MarlinUi::~MarlinUi()
{
    //TODO: 析构数据

    DELETE_PTR(m_splashScreen    );
    DELETE_PTR(m_homeScreen      );
    DELETE_PTR(m_menuScreen      );
    DELETE_PTR(m_printingScreen  );
    DELETE_PTR(m_fileScreen      );

    DELETE_PTR(m_positionTask    );
    DELETE_PTR(m_temperatureTask );
    DELETE_PTR(m_sdCardTask      );
    DELETE_PTR(m_endStopTask     );
    DELETE_PTR(m_errorTask       );
}

void MarlinUi::showSplashScreen()
{
    if(m_splashScreen)
        m_splashScreen->show();
    else
        LV_LOG_INFO("m_splashScreen not init");
}

void MarlinUi::initMarlinUI()
{
    initScreens();
    initTasks();
}

void MarlinUi::showHomeScreen()
{
    //加载主界面
    if(m_homeScreen)
        m_homeScreen->show();
    else
        LV_LOG_INFO("m_homeScreen not init");
}

void MarlinUi::showMenuScreen()
{
    //加载菜单界面
    if(m_menuScreen)
        m_menuScreen->show();
    else
        LV_LOG_INFO("m_menuScreen not init");
}

void MarlinUi::showPrintingScreen()
{
    //加载打印界面
    if(m_printingScreen)
        m_printingScreen->show();
    else
        LV_LOG_INFO("m_printingScreen not init");
}

void MarlinUi::showTemperatureScreen()
{
    //加载温度界面
    if(m_temperatureScreen)
        m_temperatureScreen->show();
    else
        LV_LOG_INFO("m_temperatureScreen not init");
}

void MarlinUi::showMotionScreen()
{
    //加载运动界面
    if(m_motionScreen)
        m_motionScreen->show();
    else
        LV_LOG_INFO("m_movementScreen not init");
}

void MarlinUi::showExtrudeScreen()
{
    //加载挤出界面
    if(m_extrudeScreen)
        m_extrudeScreen->show();
    else
        LV_LOG_INFO("m_extrudeScreen not init");
}

void MarlinUi::showFileScreen()
{
    //加载挤出界面
    if(m_fileScreen)
        m_fileScreen->show();
    else
        LV_LOG_INFO("m_fileScreen not init");
}

void MarlinUi::showMessageScreen()
{
    //加载消息提示界面
    if(m_messageScreen)
        m_messageScreen->show();
    else
        LV_LOG_INFO("m_messageScreen not init");
}

void MarlinUi::showNumberInputScreen()
{
    //加载消息提示界面
    if(m_numberImputScreen)
        m_numberImputScreen->show();
    else
        LV_LOG_INFO("m_numberImputScreen not init");
}

uint16_t MarlinUi::width() const
{
    //printf("Size %d %d",m_width,m_height);
    return m_width;
}

void MarlinUi::setWidth(uint16_t width)
{
    m_width = width;
}

uint16_t MarlinUi::height() const
{
    return m_height;
}

void MarlinUi::setHeight(uint16_t height)
{
    m_height = height;
}

uint16_t MarlinUi::dpi() const
{
    return m_dpi;
}

void MarlinUi::setDpi(uint16_t dpi)
{
    m_dpi = dpi;
}

SplashScreen *MarlinUi::splashScreen(){ return m_splashScreen; }

HomeScreen *MarlinUi::homeScreen(){ return m_homeScreen; }

TemperatureScreen *MarlinUi::temperatureScreen()
{
    return m_temperatureScreen;
}

MenuScreen *MarlinUi::menuScreen(){ return m_menuScreen; }

PrintingScreen *MarlinUi::printingScreen(){ return m_printingScreen; }

FileScreen *MarlinUi::fileScreen(){ return m_fileScreen; }

void MarlinUi::initScreens()
{
    //NOTE: 必须首先设置好字体
    lv_font_add(&microsoft_yahei_chinese_16,&microsoft_yahei_ascii_16);
    lv_font_add(&microsoft_yahei_symbol_16,&microsoft_yahei_ascii_16);

    //初始化界面语言
    LangChinese::initLang();
    LangEnglish::initLang();

    //设置界面语言
    MultiLang::langSet(LANG_ZH);

    //初始化主题
    ThemeDark::initTheme();
    //激活黑色主题
    Theme::activeTheme(Theme_Dark);

    //TODO:实现UI初始化
    m_splashScreen = new SplashScreen("");
    m_homeScreen = new HomeScreen();
    m_splashScreen->setNextScreen(m_homeScreen);

    m_menuScreen = new MenuScreen();
    m_printingScreen = new PrintingScreen();
    m_temperatureScreen = new TemperatureScreen();
    m_motionScreen = new MotionScreen();
    m_extrudeScreen = new ExtrudeScreen();
    m_fileScreen = new FileScreen();

    m_messageScreen = new MessageScreen();
    m_numberImputScreen = new NumberImputScreen();


    //初始化任务
    m_remotePrintingTask = new LVTask(1000);
    m_remotePrintingTask->setTaskFunc([this](){
        //检测远程打印的类型
        //BUG: 运动状态也会触发信号的产生 ExtUI::isPrinting()的判断不准确
        //if(ExtUI::isPrinting() && !ExtUI::isPrintingFromMedia())
        //    m_signalRemotePrinting.emit();
    });
    m_remotePrintingTask->start();

    //关联些信号槽

    //关联事件提示气泡
    //TODO:为气泡消息优化多语言
    m_slotNotifyBubble.setSlotFunc([&](LVSignal* signal){
//        if(signal == &m_signalStartup)             {} else
//        if(signal == &m_signalIdle)                {} else
        if(signal == &m_signalMediaInserted)       { Screen::showBubble("Media Inserted"); } else
        if(signal == &m_signalMediaError)          { Screen::showBubble("Media Error"); } else
        if(signal == &m_signalMediaRemoved)        { Screen::showBubble("Media Removed"); } else
        if(signal == &m_signalPlayTone)            { /* TODO:添加蜂鸣器 */ } else
//        if(signal == &m_signalPrinterKilled)       {} else
        if(signal == &m_signalPrintTimerStarted)   { Screen::showBubble("Printing Started"); } else
        if(signal == &m_signalPrintTimerPaused)    { Screen::showBubble("Printing Paused"); } else
        if(signal == &m_signalPrintTimerStopped)   { Screen::showBubble("Printing Stopped"); } else
        if(signal == &m_signalFilamentRunout)      { Screen::showBubble("Filament Runout"); } else
//        if(signal == &m_signalUserConfirmRequired) {} else
        if(signal == &m_signalStatusChanged)       {  MesgParam * param = (MesgParam *)signal->param() ;
                                                      Screen::showBubble(param->msg); } else

        if(signal == &m_signalFactoryReset)        { Screen::showBubble("Factory Reset"); } else
        if(signal == &m_signalStoreSettings)       { Screen::showBubble("Store Settings"); } else
        if(signal == &m_signalLoadSettings)        { Screen::showBubble("Load Settings"); }
    });
//    connect(&m_signalStartup            ,&m_slotNotifyBubble);
//    connect(&m_signalIdle               ,&m_slotNotifyBubble);
    connect(&m_signalMediaInserted      ,&m_slotNotifyBubble);
    connect(&m_signalMediaError         ,&m_slotNotifyBubble);
    connect(&m_signalMediaRemoved       ,&m_slotNotifyBubble);
    connect(&m_signalPlayTone           ,&m_slotNotifyBubble);
//    connect(&m_signalPrinterKilled      ,&m_slotNotifyBubble);
    connect(&m_signalPrintTimerStarted  ,&m_slotNotifyBubble);
    connect(&m_signalPrintTimerPaused   ,&m_slotNotifyBubble);
    connect(&m_signalPrintTimerStopped  ,&m_slotNotifyBubble);
    connect(&m_signalFilamentRunout     ,&m_slotNotifyBubble);
//    connect(&m_signalUserConfirmRequired,&m_slotNotifyBubble);
    connect(&m_signalStatusChanged      ,&m_slotNotifyBubble);
    connect(&m_signalFactoryReset       ,&m_slotNotifyBubble);
    connect(&m_signalStoreSettings      ,&m_slotNotifyBubble);
    connect(&m_signalLoadSettings       ,&m_slotNotifyBubble);

}

void MarlinUi::initTasks()
{
    //TODO:添加UI任务
}


//返回主界面
lv_res_t onButBackHomeClicked (struct _lv_obj_t * obj)
{
    MarlinUi::getInstance()->showHomeScreen();
    return LV_RES_OK;
}
