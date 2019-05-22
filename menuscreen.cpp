#include "menuscreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"

MenuScreen::MenuScreen()
{

}

bool MenuScreen::initScreen()
{
    //初始化界面

    /*
     * 实现菜单工具界面

    ==============================================
    +++XXXXXXX++++++++++++++++++++++++++++++++++++
    ++++++++++++++++++++++++++++++++++++++++++++++
    =                                            =
    =   0000000   1111111   222222222222222222   =
    =   0000000   1111111   222222222222222222   =
    =   0000000   1111111   222222222222222222   =
    =                                            =
    =   3333333   4444444   5555555   66666666   =
    =   3333333   4444444   5555555   66666666   =
    =   3333333   4444444   5555555   66666666   =
    =                                            =
    =   77777777777777777   888888888888888888   =
    =   77777777777777777   888888888888888888   =
    =                                            =
    ==============================================

    */

    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 4;
    const int16_t titleBarHeight = 32;

    const int16_t butSize = 74;
    const int16_t butHeight = 44;

    const int16_t fontSize = 16;
    const int16_t iconSize = 24;


    LVObject * reference = nullptr; //布局参考对象

    {
        //标题栏
        LVObject * titleBar = new LVObject(this,nullptr);
        titleBar->setStyle(THEME_STYLE_GET(Theme_Screen_TitleBar));
        titleBar->setSize(screenWidth,titleBarHeight);
        titleBar->align(LV_ALIGN_IN_TOP_LEFT,0,0);
        reference = titleBar;

        //标题返回按钮
        //标题返回按钮
        LVButton * butBack = new LVButton(titleBar,nullptr);
        butBack->setSize(titleBarHeight,titleBarHeight);
        butBack->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butBack->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butBack->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
        butBack->setAction(LV_BTN_ACTION_CLICK,onButBackHomeClicked);
        LVImage * imgBack = new LVImage(butBack,nullptr);
        imgBack->setSrc(&back_24);

        //标题
        labTitle = new LVLabel(titleBar,nullptr);
        labTitle->setText(Lang_MenuScreen_ButTitle);
        labTitle->align(butBack,LV_ALIGN_OUT_RIGHT_MID,0,0);

        //其他功能按钮

    }



    //按钮区域样式
    static LVStyle styleButArea(&lv_style_plain);
    styleButArea->body.radius = 0;
    styleButArea->body.main_color = LV_COLOR_MAKE(0x00,0x00,0x00); //TODO: 更改按钮区域颜色
    styleButArea->body.grad_color = LV_COLOR_MAKE(0x00,0x00,0x00);
    styleButArea->text.color = LV_COLOR_WHITE;
    styleButArea->body.border.width = 0;

    //按钮样式
    static LVStyle styleButRel(&lv_style_btn_rel);
    styleButRel->body.main_color = LV_COLOR_MAKE(0x1d,0x21,0x26);
    styleButRel->body.grad_color = LV_COLOR_MAKE(0x1d,0x21,0x26);
    styleButRel->body.radius = 0;
    styleButRel->text.color = LV_COLOR_WHITE;
    styleButRel->body.border.width = 0;

    static LVStyle styleButPr(styleButRel);
    styleButPr->body.border.color = LV_COLOR_MAKE(0x2d,0x32,0x38);
    styleButPr->body.border.width = 5;

    //按钮区域
    //TODO: 可以去掉这个对象,节省内存
    LVObject * butArea = new LVObject(this,nullptr);
    butArea->setSize(screenWidth,screenHeight - titleBarHeight);
    butArea->align(reference,LV_ALIGN_OUT_BOTTOM_LEFT);
    butArea->setStyle(styleButArea);

    //工具按钮////////////

    {
        //Gcode
        LVButton * butGCode = new LVButton(butArea,nullptr);
        butGCode->setSize(butSize,butSize);
        butGCode->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,spacing);
        butGCode->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butGCode->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgGCode = new LVImage(butGCode,nullptr);
        imgGCode->setSrc(&cmd_24);
        imgGCode->align(LV_ALIGN_CENTER);
        labGcode = new LVLabel(butGCode,nullptr);
        labGcode->setText(Lang_MenuScreen_ButGCode); //"GCode"
        labGcode->align(imgGCode,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butGCode;
    }


    {
        //调平
        LVButton * butLeveling = new LVButton(butArea,nullptr);
        butLeveling->setSize(butSize,butSize);
        butLeveling->align(reference,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butLeveling->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butLeveling->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgLeveling = new LVImage(butLeveling,nullptr);
        imgLeveling->setSrc(&levelling_24);
        imgLeveling->align(LV_ALIGN_CENTER);
        labLeveling = new LVLabel(butLeveling,nullptr);
        labLeveling->setText(Lang_MenuScreen_ButLeveling);//"Leveling"
        labLeveling->align(butLeveling,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butLeveling;
    }

    {
        //自定义
        LVButton * butCustom = new LVButton(butArea,nullptr);
        butCustom->setSize(butSize*2+spacing,butSize);
        butCustom->align(reference,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butCustom->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butCustom->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgCustom = new LVImage(butCustom,nullptr);
        imgCustom->setSrc(&custom_24);
        imgCustom->align(LV_ALIGN_CENTER);
        labCustom = new LVLabel(butCustom,nullptr);
        labCustom->setText(Lang_MenuScreen_ButCustom);//"Custom"
        labCustom->align(imgCustom,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butCustom;
    }

    {
        //设置
        LVButton * butSetting = new LVButton(butArea,nullptr);
        butSetting->setSize(butSize,butSize);
        butSetting->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,spacing+butSize+spacing);
        butSetting->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butSetting->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgSetting = new LVImage(butSetting,nullptr);
        imgSetting->setSrc(&setting_24);
        imgSetting->align(LV_ALIGN_CENTER);
        labSetting = new LVLabel(butSetting,nullptr);
        labSetting->setText(Lang_MenuScreen_ButSetting);//"Setting"
        labSetting->align(imgSetting,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butSetting;
    }

    {
        //检测
        LVButton * butCheck = new LVButton(butArea,nullptr);
        butCheck->setSize(butSize,butSize);
        butCheck->align(reference,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butCheck->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butCheck->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgCheck = new LVImage(butCheck,nullptr);
        imgCheck->setSrc(&check_24);
        imgCheck->align(LV_ALIGN_CENTER);
        labCheck = new LVLabel(butCheck,nullptr);
        labCheck->setText(Lang_MenuScreen_ButCheck);//"Check"
        labCheck->align(imgCheck,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butCheck;
    }

    {
        //连接
        LVButton * butConnect = new LVButton(butArea,nullptr);
        butConnect->setSize(butSize,butSize);
        butConnect->align(reference,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butConnect->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butConnect->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgConnect = new LVImage(butConnect,nullptr);
        imgConnect->setSrc(&connect_24);
        imgConnect->align(LV_ALIGN_CENTER);
        labConnect = new LVLabel(butConnect,nullptr);
        labConnect->setText(Lang_MenuScreen_ButConnect);//"Connect"
        labConnect->align(imgConnect,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butConnect;
    }

    {
        //关机
        LVButton * butShutdown = new LVButton(butArea,nullptr);
        butShutdown->setSize(butSize,butSize);
        butShutdown->align(reference,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butShutdown->setStyle(LV_BTN_STYLE_PR,styleButPr);
        butShutdown->setStyle(LV_BTN_STYLE_REL,styleButRel);
        LVImage * imgShutdown = new LVImage(butShutdown,nullptr);
        imgShutdown->setSrc(&shutdown_24);
        imgShutdown->align(LV_ALIGN_CENTER);
        labShutdown = new LVLabel(butShutdown,nullptr);
        labShutdown->setText(Lang_MenuScreen_ButShutDown);//"ShutDown"
        labShutdown->align(imgShutdown,LV_ALIGN_OUT_BOTTOM_MID,0,spacing);
        reference = butShutdown;
    }

    static LVStyle styleBut2Rel(&lv_style_btn_rel);
    styleBut2Rel->body.main_color = LV_COLOR_MAKE(0x17,0x28,0x45);
    styleBut2Rel->body.grad_color = LV_COLOR_MAKE(0x17,0x28,0x45);
    styleBut2Rel->body.radius = 0;
    styleBut2Rel->text.color = LV_COLOR_WHITE;

    static LVStyle styleBut2Pr(styleBut2Rel);
    styleBut2Pr->body.border.color = LV_COLOR_MAKE(0x2d,0x32,0x38);
    styleBut2Pr->body.border.width = 5;


    {
        //固件信息
        LVButton * butFirmware = new LVButton(butArea,nullptr);
        butFirmware->setSize(butSize+spacing+butSize,butHeight);
        butFirmware->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,spacing+butSize+spacing+butSize+spacing);
        butFirmware->setStyle(LV_BTN_STYLE_PR,styleBut2Pr);
        butFirmware->setStyle(LV_BTN_STYLE_REL,styleBut2Rel);
        butFirmware->setLayout(LV_LAYOUT_OFF);
        labFirmware = new LVLabel(butFirmware,nullptr);
        labFirmware->setText(Lang_MenuScreen_ButFirmware);//"Firmware"
        labFirmware->align(LV_ALIGN_IN_TOP_MID,0,spacing);
        LVLabel * labFirmwareVersion = new LVLabel(butFirmware,nullptr);
        labFirmwareVersion->setText("Marlin V2");
        labFirmwareVersion->align(labFirmware,LV_ALIGN_OUT_BOTTOM_MID,0,0);
        reference = butFirmware;
    }

    {
        //关于机器
        LVButton * butAbout = new LVButton(butArea,nullptr);
        butAbout->setSize(butSize+spacing+butSize,butHeight);
        butAbout->align(reference,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butAbout->setStyle(LV_BTN_STYLE_PR,styleBut2Pr);
        butAbout->setStyle(LV_BTN_STYLE_REL,styleBut2Rel);
        butAbout->setLayout(LV_LAYOUT_OFF);
        labAbout = new LVLabel(butAbout,nullptr);
        labAbout->setText(Lang_MenuScreen_ButAbout);//"About"
        labAbout->align(LV_ALIGN_IN_TOP_MID,0,spacing);
        LVLabel * labVersion = new LVLabel(butAbout,nullptr);
        labVersion->setText("PandaMini V2");
        labVersion->align(labAbout,LV_ALIGN_OUT_BOTTOM_MID,0,0);
        reference = butAbout;
    }

    LV_LOG_INFO(__FUNCTION__);

    return true;

}

void MenuScreen::onLangChanged()
{
    //TODO:完成语言切换

//    CHANGE_LABEL_LANG(labTitle)
//    CHANGE_LABEL_LANG(labGcode)
//    CHANGE_LABEL_LANG(labLeveling)
//    CHANGE_LABEL_LANG(labCustom)
//    CHANGE_LABEL_LANG(labSetting)
//    CHANGE_LABEL_LANG(labCheck)
//    CHANGE_LABEL_LANG(labConnect)
//    CHANGE_LABEL_LANG(labShutdown)
//    CHANGE_LABEL_LANG(labFirmware)
//    CHANGE_LABEL_LANG(labAbout)
}
