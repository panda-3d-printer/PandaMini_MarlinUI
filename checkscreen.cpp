#include "checkscreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "extuiextend.h"
#include "lvslot.hpp"

using std::placeholders::_1;

CheckScreen::CheckScreen()
{
}

//bool CheckScreen::beforeShow()
//{
//}

//void CheckScreen::afterHide()
//{
//}

void CheckScreen::onLangChanged()
{
}

void CheckScreen::onThemeChanged()
{
}

bool CheckScreen::initScreen()
{
    //初始化挤出界面

    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 6;
    const int16_t titleBarHeight = 32;
    const int16_t stateBarHeight = 26;

    const int16_t checkButWidth = 80;
    const int16_t checkButHeight = 36;

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
        butBack->setAction(LV_BTN_ACTION_CLICK,onButBackMenuClicked);
        LVImage * imgBack = new LVImage(butBack,nullptr);
        imgBack->setSrc(&back_24);

        //标题
        LVLabel* labTitle = new LVLabel(titleBar,nullptr);
        labTitle->setText("开关检测");
        labTitle->align(butBack,LV_ALIGN_OUT_RIGHT_MID);
    }

    {
        //功能区域

        LVObject * butArea = new LVObject(this,nullptr);
        butArea->align(LV_ALIGN_IN_TOP_LEFT,0,titleBarHeight);
        butArea->setSize(screenWidth,screenHeight - titleBarHeight - stateBarHeight);
        butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));


        //限位开关状态按钮

        lv_style_copy(m_activeStyle,&lv_style_btn_pr);
        m_activeStyle->body.main_color = LV_COLOR_MAKE(0xbf,0x32,0x49);
        m_activeStyle->body.grad_color = LV_COLOR_MAKE(0xbf,0x32,0x49);
        m_activeStyle->body.border.width = 0;
        m_activeStyle->body.radius = 0;
        m_activeStyle->text.color = LV_COLOR_MAKE(0x02,0xa1,0xed);

        lv_style_copy(m_inactiveStyle,&lv_style_btn_rel);
        m_inactiveStyle->body.main_color = LV_COLOR_MAKE(0x17,0x28,0x45);
        m_inactiveStyle->body.grad_color = LV_COLOR_MAKE(0x17,0x28,0x45);
        m_inactiveStyle->body.border.width = 0;
        m_inactiveStyle->body.radius = 0;
        m_inactiveStyle->text.color = LV_COLOR_MAKE(0x02,0xa1,0xed);

        LVLabel * labXmin = new LVLabel(butArea,nullptr);
        labXmin->setText("X_MIN");
        labXmin->align(butArea,LV_ALIGN_IN_TOP_LEFT,safeMargin+spacing*2,spacing*5);
        butXmin = new LVButton(butArea,nullptr);
        butXmin->setStyle(LV_BTN_STYLE_PR,m_activeStyle);
        butXmin->setStyle(LV_BTN_STYLE_REL,m_inactiveStyle);
        butXmin->setStyle(LV_BTN_STYLE_TGL_REL,m_activeStyle);
        butXmin->setStyle(LV_BTN_STYLE_TGL_PR,m_inactiveStyle);
        butXmin->setSize(checkButWidth,checkButHeight);
        butXmin->setToggle(true);
        butXmin->setClick(false);
        butXmin->align(labXmin,LV_ALIGN_OUT_RIGHT_MID,spacing);
        butXmin->setChecked(ExtUI::getEndstopState(ExtUI::XMIN));
        labXminState = new LVLabel(butXmin,nullptr);
        labXminState->setText(butXmin->checked()?"已触发":"未触发");

        LVLabel * labXmax = new LVLabel(butArea,nullptr);
        labXmax->setText("X_MAX");
        labXmax->align(butXmin,LV_ALIGN_OUT_RIGHT_MID,spacing*4);
        butXmax = new LVButton(butArea,butXmin);
        butXmax->align(labXmax,LV_ALIGN_OUT_RIGHT_MID,spacing);
        butXmax->setChecked(ExtUI::getEndstopState(ExtUI::XMAX));
        labXmaxState = new LVLabel(butXmax,nullptr);
        labXmaxState->setText(butXmax->checked()?"已触发":"未触发");

        LVLabel * labYmin = new LVLabel(butArea,nullptr);
        labYmin->setText("Y_MIN");
        labYmin->align(labXmin,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing*6);
        butYmin = new LVButton(butArea,butXmin);
        butYmin->setChecked(ExtUI::getEndstopState(ExtUI::YMIN));
        butYmin->align(labYmin,LV_ALIGN_OUT_RIGHT_MID,spacing);
        labYminState = new LVLabel(butYmin,nullptr);
        labYminState->setText(butYmin->checked()?"已触发":"未触发");

        LVLabel * labYmax = new LVLabel(butArea,nullptr);
        labYmax->setText("Y_MAX");
        labYmax->align(butYmin,LV_ALIGN_OUT_RIGHT_MID,spacing*4);
        butYmax = new LVButton(butArea,butXmin);
        butYmax->align(labYmax,LV_ALIGN_OUT_RIGHT_MID,spacing);
        butYmax->setChecked(ExtUI::getEndstopState(ExtUI::YMAX));
        labYmaxState = new LVLabel(butYmax,nullptr);
        labYmaxState->setText(butYmax->checked()?"已触发":"未触发");

        LVLabel * labZmin = new LVLabel(butArea,nullptr);
        labZmin->setText("Z_MIN");
        labZmin->align(labYmin,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing*6);
        butZmin = new LVButton(butArea,butXmin);
        butZmin->align(labZmin,LV_ALIGN_OUT_RIGHT_MID,spacing);
        butZmin->setChecked(ExtUI::getEndstopState(ExtUI::ZMIN));
        labZminState = new LVLabel(butZmin,nullptr);
        labZminState->setText(butZmin->checked()?"已触发":"未触发");

        LVLabel * labZmax = new LVLabel(butArea,nullptr);
        labZmax->setText("Z_MAX");
        labZmax->align(butZmin,LV_ALIGN_OUT_RIGHT_MID,spacing*4);
        butZmax = new LVButton(butArea,butXmin);
        butZmax->setChecked(ExtUI::getEndstopState(ExtUI::ZMAX));
        butZmax->align(labZmax,LV_ALIGN_OUT_RIGHT_MID,spacing);
        labZmaxState = new LVLabel(butZmax,nullptr);
        labZmaxState->setText(butZmax->checked()?"已触发":"未触发");

    }

    {
        //状态栏
        LVObject * stateBar = new LVObject(this,nullptr);
        stateBar->setSize(screenWidth,stateBarHeight);
        stateBar->align(LV_ALIGN_IN_BOTTOM_LEFT,0,0);
        //状态栏样式
        stateBar->setStyle(THEME_STYLE_GET(Theme_Screen_StateBar));

        //状态栏信息
        LVLabel * stateMesg = new LVLabel(stateBar,nullptr);
        stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
        stateMesg->setText("状态每秒刷新"); //"Connected!"
        stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
    }

    ScreenTask * triggerSyncTask = new ScreenTask(this,250);
    triggerSyncTask->setTaskFunc([=](){
    	bool state = ExtUI::getEndstopState(ExtUI::XMIN);
        butXmin->setChecked(state);
        labXminState->setText(state?"已触发":"未触发");

        state = ExtUI::getEndstopState(ExtUI::XMAX);
        butXmax->setChecked(ExtUI::getEndstopState(ExtUI::XMAX));
        labXmaxState->setText(state?"已触发":"未触发");

        state = ExtUI::getEndstopState(ExtUI::YMIN);
        butYmin->setChecked(ExtUI::getEndstopState(ExtUI::YMIN));
        labYminState->setText(state?"已触发":"未触发");

        state = ExtUI::getEndstopState(ExtUI::YMAX);
        butYmax->setChecked(ExtUI::getEndstopState(ExtUI::YMAX));
        labYmaxState->setText(state?"已触发":"未触发");

        state = ExtUI::getEndstopState(ExtUI::ZMIN);
        butZmin->setChecked(ExtUI::getEndstopState(ExtUI::ZMIN));
        labZminState->setText(state?"已触发":"未触发");

        state = ExtUI::getEndstopState(ExtUI::ZMAX);
        butZmax->setChecked(ExtUI::getEndstopState(ExtUI::ZMAX));
        labZmaxState->setText(state?"已触发":"未触发");
    });

    return true;
}
