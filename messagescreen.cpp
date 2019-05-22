#include "messagescreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"
#include <lvgl/lv_core/lv_refr.h>

static lv_res_t onButClicked (struct _lv_obj_t * obj)
{
    LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
    if(but)
    {
        MarlinUi::getInstance()->messageScreen()->setHideByUser(true);
        MarlinUi::getInstance()->messageScreen()->setResult(but->getFreeNumber());
        MarlinUi::getInstance()->messageScreen()->hide();
    }
}

uint32_t MessageScreen::result() const
{
    return m_result;
}

void MessageScreen::setResult(const uint32_t &result)
{
    m_result = result;
}

MessageFunc MessageScreen::messageFunc() const
{
    return m_messageFunc;
}

void MessageScreen::setMessageFunc(const MessageFunc &action)
{
    m_messageFunc = action;
}

void MessageScreen::onPrinterKilled(LVSignal *signal)
{
    //获取消息
    MesgParam * param = static_cast<MesgParam *>(signal->param());

    LV_LOG_INFO("onButPrintFileClicked");

    //TODO:准备提示界面
    setTitle("Printer Killed");
    setMessage(param->msg);
    show();
    labMesg()->setLongMode(LV_LABEL_LONG_BREAK);

    //刷新界面
    lv_refr_now();
}

bool MessageScreen::hideByUser() const
{
    return m_hideByUser;
}

void MessageScreen::setHideByUser(bool hideByUser)
{
    m_hideByUser = hideByUser;
}

MessageScreen::MessageScreen()
    : Screen()
{
    using std::placeholders::_1;
    m_printerKilledSlot.setSlotFunc(std::bind(&MessageScreen::onPrinterKilled,this,_1));
    connect(&MarlinUi::getInstance()->getSignalPrinterKilled(),&m_printerKilledSlot);
}

MessageScreen::~MessageScreen()
{

}

bool MessageScreen::initScreen()
{
    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 6;
    const int16_t titleBarHeight = 32;
    const int16_t stateBarHeight = 26;

    const int16_t butAreaMargin = 10;

    const int16_t fontSize = 16;
    const int16_t iconSize = 24;

    LVObject * reference = nullptr; //布局参考对象

    {
        //标题栏
        LVObject * titleBar = new LVObject(this,nullptr);
        titleBar->setStyle(THEME_STYLE_GET(Theme_Screen_TitleBar));
        titleBar->setSize(screenWidth,titleBarHeight);
        titleBar->align(LV_ALIGN_IN_TOP_LEFT);
        reference = titleBar;

        //标题返回按钮
        LVButton * butBack = new LVButton(titleBar,nullptr);
        butBack->setSize(titleBarHeight,titleBarHeight);
        butBack->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butBack->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butBack->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
        butBack->setAction(LV_BTN_ACTION_CLICK,onButClicked);
        butBack->setFreeNumber(MR_Back);
        LVImage * imgBack = new LVImage(butBack,nullptr);
        imgBack->setSrc(&back_24);

        //标题
        labTitle()->setParent(titleBar);
        labTitle()->align(butBack,LV_ALIGN_OUT_RIGHT_MID);
        labTitle()->setAutoRealign(true);

        //标题图标
        LVImage * imgIcon = new LVImage(titleBar,nullptr);
        imgIcon->setSrc(&next_24);
        imgIcon->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));
        imgIcon->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
    }

    {
        //功能区域

        m_butArea = new LVObject(this,nullptr);
        m_butArea->align(LV_ALIGN_IN_TOP_LEFT,0,titleBarHeight);
        m_butArea->setSize(screenWidth,screenHeight - titleBarHeight);
        m_butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));

        //显示消息

        labMesg()->setParent(m_butArea);
        labMesg()->align(LV_ALIGN_CENTER,0,-m_butArea->getHeight()/5);
        labMesg()->setAutoRealign(true);

        //显示按钮
        m_contButs = new LVContainer(m_butArea,nullptr);
        m_contButs->setFit(true,true);
        m_contButs->setAutoRealign(true);
        m_contButs->setLayout(LV_LAYOUT_ROW_M);
        m_contButs->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));
        m_contButs->align(LV_ALIGN_CENTER,0,m_butArea->getHeight()/8);


    }

    LV_LOG_INFO(__FUNCTION__);

    return true;
}

bool MessageScreen::beforeShow()
{
    bool ret = Screen::beforeShow();
    if(ret)
    {
        //更新消息和按钮

        if(m_contButs)
        {
            if( m_button0&&
                m_labBut0){
                m_button0->setParent(m_contButs);
                m_button0->setSize(m_butWidth,m_butHeight);
                m_button0->setFit(true,false);
                m_labBut0->setParent(
                m_button0);
                m_button0->setAction(LV_BTN_ACTION_CLICK,onButClicked);
            }

            if( m_button1&&
                m_labBut1){
                m_button1->setParent(m_contButs);
                m_button1->setSize(m_butWidth,m_butHeight);
                m_button1->setFit(true,false);
                m_labBut1->setParent(
                m_button1);
                m_button1->setAction(LV_BTN_ACTION_CLICK,onButClicked);
            }

            if( m_button2&&
                m_labBut2){
                m_button2->setParent(m_contButs);
                m_button2->setSize(m_butWidth,m_butHeight);
                m_button2->setFit(true,false);
                m_labBut2->setParent(
                m_button2);
                m_button2->setAction(LV_BTN_ACTION_CLICK,onButClicked);
            }

            //重布局按钮
            m_contButs->realign();
        }
    }

    return ret;
}

void MessageScreen::afterHide()
{
    //执行消息动作
    if(m_messageFunc != nullptr && m_hideByUser)
    {
        m_messageFunc(m_result);
    }

    //重置数据
    setResult(MR_Unknow);
    clearAllBut();
    setHideByUser(false);
}

void MessageScreen::onLangChanged()
{

}

void MessageScreen::onThemeChanged()
{
}

void MessageScreen::setTitle(char *title)
{
    labTitle()->setText(title);
}

void MessageScreen::setTitle(uint16_t titleId)
{
    labTitle()->setText(titleId);
}

void MessageScreen::setMessage(const char *mesg)
{
    labMesg()->setText(mesg);
}

void MessageScreen::setMessage(uint16_t mesgId)
{
    labMesg()->setText(mesgId);
}

LVLabel *MessageScreen::labTitle()
{
    if(m_labTitle == nullptr)
        m_labTitle = new LVLabel(this,nullptr);
    return m_labTitle;
}

LVLabel *MessageScreen::labMesg()
{
    if(m_labMesg == nullptr)
        m_labMesg = new LVLabel(this,nullptr);
    return m_labMesg;
}

LVButton *MessageScreen::button0()
{
    if( m_button0 == nullptr){
        m_button0 = new LVButton(this,nullptr);
        m_button0->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBlue));
        m_button0->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBlue));
    }
    return m_button0;
}

LVButton *MessageScreen::button1()
{
    if( m_button1 == nullptr){
        m_button1 = new LVButton(this,nullptr);
        m_button1->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBlue));
        m_button1->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBlue));
    }
    return m_button1;
}

LVButton *MessageScreen::button2()
{
    if( m_button2 == nullptr){
        m_button2 = new LVButton(this,nullptr);
        m_button2->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBlue));
        m_button2->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBlue));
    }
    return m_button2;
}

LVLabel *MessageScreen::labBut0()
{
    if(m_labBut0 == nullptr)
        m_labBut0 = new LVLabel(this,nullptr);
    return m_labBut0;
}

LVLabel *MessageScreen::labBut1()
{
    if(m_labBut1 == nullptr)
        m_labBut1 = new LVLabel(this,nullptr);
    return m_labBut1;
}

LVLabel *MessageScreen::labBut2()
{
    if(m_labBut2 == nullptr)
        m_labBut2 = new LVLabel(this,nullptr);
    return m_labBut2;
}

void MessageScreen::setButton0(char *text, uint32_t resultId)
{
    button0()->setFreeNumber(resultId);
    labBut0()->setText(text);
}

void MessageScreen::setButton0(uint16_t textId, uint32_t resultId)
{
    button0()->setFreeNumber(resultId);
    labBut0()->setText(textId);
}

void MessageScreen::setButton1(char *text, uint32_t resultId)
{
    button1()->setFreeNumber(resultId);
    labBut1()->setText(text);
}

void MessageScreen::setButton1(uint16_t textId, uint32_t resultId)
{
    button1()->setFreeNumber(resultId);
    labBut1()->setText(textId);
}

void MessageScreen::setButton2(char *text, uint32_t resultId)
{
    button2()->setFreeNumber(resultId);
    labBut2()->setText(text);
}

void MessageScreen::setButton2(uint16_t textId, uint32_t resultId)
{
    button2()->setFreeNumber(resultId);
    labBut2()->setText(textId);
}

void MessageScreen::clearAllBut()
{

    m_button0 = nullptr;
    m_button1 = nullptr;
    m_button2 = nullptr;

    m_labBut0 = nullptr;
    m_labBut1 = nullptr;
    m_labBut2 = nullptr;

    m_messageFunc = nullptr;
}

void MessageScreen::afterCleanScreen()
{
    m_labTitle = nullptr;
    m_labMesg = nullptr;
}
