#include "themedark.h"
#include <lvstyle.hpp>

void ThemeDark::initTheme()
{
    static ThemeDark * themeDark = nullptr;
    if(!themeDark)
        themeDark = new ThemeDark();
}

void ThemeDark::setupTheme()
{
    //// 基本公共元素 ////////////////////////////

    auto titleBarBgColor    = LV_COLOR_HEX(0x1d2126); //标题栏背景色
    auto textColor          = LV_COLOR_HEX(0xffffff); //文字颜色
    auto stateBarBgColor    = LV_COLOR_HEX(0x040405); //状态栏背景色
    auto buttonAreaBgColor  = LV_COLOR_HEX(0x111417);
    auto buttonPrColor      = LV_COLOR_HEX(0x2d3238);
    auto buttonRelColor     = LV_COLOR_HEX(0x1d2126);
    auto buttonInaColor     = LV_COLOR_HEX(0x111417);
    auto processBarColor    = LV_COLOR_HEX(0x172845);
    auto processIndicColor  = LV_COLOR_HEX(0x2e77ff);

    auto redColor           = LV_COLOR_HEX(0xd54937);
    auto greenColor         = LV_COLOR_HEX(0x30e230);
    auto yellowColor        = LV_COLOR_HEX(0xef9c00);
    auto blueColor          = LV_COLOR_HEX(0x2e77ff);
    auto blueDarkColor      = LV_COLOR_HEX(0x172845);
    auto cyanColor          = LV_COLOR_HEX(0x00aeff);
    auto grayColor          = LV_COLOR_HEX(0x414345);

    //标题栏样式
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar) = lv_style_plain;
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar)->body.main_color = titleBarBgColor;
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar)->body.grad_color = titleBarBgColor;
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar)->text.color = textColor;
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar)->image.color = LV_COLOR_WHITE;
    THEME_STYLE_INCLASS(Theme_Screen_TitleBar)->image.intense = LV_OPA_100;

    //标题栏图标样式
//    THEME_STYLE(Theme_Screen_TitleIcon) = lv_style_plain;
//    THEME_STYLE(Theme_Screen_TitleIcon)->body.radius = 0;
//    THEME_STYLE(Theme_Screen_TitleIcon)->body.main_color = titleBarBgColor;
//    THEME_STYLE(Theme_Screen_TitleIcon)->body.grad_color = titleBarBgColor;
//    THEME_STYLE(Theme_Screen_TitleIcon)->text.color = textColor;

    //标题栏返回按钮
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack) = lv_style_btn_rel;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->body.main_color = titleBarBgColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->body.grad_color = titleBarBgColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->body.shadow.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->text.color = textColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->image.color = LV_COLOR_WHITE;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBack)->image.intense = LV_OPA_100;

    //标题文本
//    THEME_STYLE(Theme_Screen_TitleText) = lv_style_plain;
//    THEME_STYLE(Theme_Screen_TitleText)->body.radius = 0;
//    THEME_STYLE(Theme_Screen_TitleText)->body.main_color = titleBarBgColor;
//    THEME_STYLE(Theme_Screen_TitleText)->body.grad_color = titleBarBgColor;
//    THEME_STYLE(Theme_Screen_TitleText)->text.color = textColor;

    //状态栏样式
    THEME_STYLE_INCLASS(Theme_Screen_StateBar) = lv_style_plain;
    THEME_STYLE_INCLASS(Theme_Screen_StateBar)->body.main_color = stateBarBgColor;
    THEME_STYLE_INCLASS(Theme_Screen_StateBar)->body.grad_color = stateBarBgColor;
    THEME_STYLE_INCLASS(Theme_Screen_StateBar)->text.color = textColor; //白色文字
    THEME_STYLE_INCLASS(Theme_Screen_StateBar)->image.color = LV_COLOR_WHITE;
    THEME_STYLE_INCLASS(Theme_Screen_StateBar)->image.intense = LV_OPA_100;

    //状态栏信息文本样式
//    THEME_STYLE(Theme_Screen_StateText) = lv_style_plain;
//    THEME_STYLE(Theme_Screen_StateText)->body.main_color = stateBarBgColor;
//    THEME_STYLE(Theme_Screen_StateText)->body.grad_color = stateBarBgColor;
//    THEME_STYLE(Theme_Screen_StateText)->text.color = textColor; //白色文字


    //按钮区域样式
    THEME_STYLE_INCLASS(Theme_screen_ButtonArea) = lv_style_plain;
    THEME_STYLE_INCLASS(Theme_screen_ButtonArea)->body.main_color = buttonAreaBgColor;
    THEME_STYLE_INCLASS(Theme_screen_ButtonArea)->body.grad_color = buttonAreaBgColor;
    THEME_STYLE_INCLASS(Theme_screen_ButtonArea)->text.color = textColor; //白色文字
    THEME_STYLE_INCLASS(Theme_screen_ButtonArea)->image.color = LV_COLOR_WHITE;
    THEME_STYLE_INCLASS(Theme_screen_ButtonArea)->image.intense = LV_OPA_100;


    //功能按钮按下样式
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr) = lv_style_btn_pr;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->body.main_color = buttonRelColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->body.grad_color = buttonRelColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->body.border.width = 5;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->text.color = textColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->body.border.color = buttonPrColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->image.color = LV_COLOR_WHITE;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonPr)->image.intense = LV_OPA_100;

    //功能按钮弹起样式
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel) = lv_style_btn_rel;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->body.main_color = buttonRelColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->body.grad_color = buttonRelColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->text.color = textColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->image.color = LV_COLOR_WHITE;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRel)->image.intense = LV_OPA_100;


    //功能按钮禁用样式
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna) = lv_style_btn_ina;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->body.main_color = buttonInaColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->body.grad_color = buttonInaColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->text.color = buttonRelColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->image.color = LV_COLOR_GRAY;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonIna)->image.intense = LV_OPA_100;

    //按钮红色样式
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRed) = lv_style_btn_rel;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRed)->body.main_color = redColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRed)->body.grad_color = redColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRed)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonRed)->body.radius = 0;

    //按钮绿色样式
    THEME_STYLE_INCLASS(Theme_Screen_ButtonGreen)= lv_style_btn_rel;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonGreen)->body.main_color = greenColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonGreen)->body.grad_color = greenColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonGreen)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonGreen)->body.radius = 0;

    //按钮蓝色样式
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBlue)= lv_style_btn_rel;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBlue)->body.main_color = blueColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBlue)->body.grad_color = blueColor;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBlue)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_Screen_ButtonBlue)->body.radius = 0;

    //功能按钮图标样式
//    THEME_STYLE(Theme_Screen_ButtonImg) = lv_style_plain;
//    THEME_STYLE(Theme_Screen_ButtonImg)->body.main_color = buttonRelColor;
//    THEME_STYLE(Theme_Screen_ButtonImg)->body.grad_color = buttonRelColor;
//    THEME_STYLE(Theme_Screen_ButtonImg)->body.radius = 0;
//    THEME_STYLE(Theme_Screen_ButtonImg)->text.color = textColor;

    //功能按钮文本样式
//    THEME_STYLE(Theme_Screen_ButtonText) = lv_style_plain;
//    THEME_STYLE(Theme_Screen_ButtonText)->body.main_color = buttonRelColor;
//    THEME_STYLE(Theme_Screen_ButtonText)->body.grad_color = buttonRelColor;
//    THEME_STYLE(Theme_Screen_ButtonText)->body.radius = 0;
//    THEME_STYLE(Theme_Screen_ButtonText)->text.color = textColor;

    //气泡信息
    THEME_STYLE_INCLASS(Theme_screen_TextBubble) = lv_style_plain;
    THEME_STYLE_INCLASS(Theme_screen_TextBubble)->body.main_color = LV_COLOR_MAKE(0x17,0x28,0x45);
    THEME_STYLE_INCLASS(Theme_screen_TextBubble)->body.grad_color = LV_COLOR_MAKE(0x17,0x28,0x45);
    THEME_STYLE_INCLASS(Theme_screen_TextBubble)->text.color = textColor;
    THEME_STYLE_INCLASS(Theme_screen_TextBubble)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_screen_TextBubble)->body.border.width = 0;

    //进度条背景样式
    THEME_STYLE_INCLASS(Theme_screen_ProcessBar) = lv_style_plain;
    THEME_STYLE_INCLASS(Theme_screen_ProcessBar)->body.main_color = processBarColor;
    THEME_STYLE_INCLASS(Theme_screen_ProcessBar)->body.grad_color = processBarColor;
    THEME_STYLE_INCLASS(Theme_screen_ProcessBar)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessBar)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessBar)->text.color = textColor;

    //进度条前景样式
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic) = lv_style_plain;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.main_color = processIndicColor;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.grad_color = processIndicColor;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.radius = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.padding.hor = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.padding.ver = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->body.padding.inner = 0;
    THEME_STYLE_INCLASS(Theme_screen_ProcessIndic)->text.color = textColor;

    //图片颜色
    THEME_STYLE_INCLASS(Theme_screen_Red) = lv_style_plain;       //红色图片样式
    THEME_STYLE_INCLASS(Theme_screen_Red)->image.color = redColor;
    THEME_STYLE_INCLASS(Theme_screen_Red)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_Red)->body.main_color = redColor;
    THEME_STYLE_INCLASS(Theme_screen_Red)->body.grad_color = redColor;
    THEME_STYLE_INCLASS(Theme_screen_Red)->body.border.color = redColor;
    THEME_STYLE_INCLASS(Theme_screen_Red)->text.color = redColor;
    THEME_STYLE_INCLASS(Theme_screen_Red)->line.color = redColor;
    THEME_STYLE_INCLASS(Theme_screen_Red)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_Red)->body.shadow.width = 0;

    THEME_STYLE_INCLASS(Theme_screen_Green) = lv_style_plain;     //绿色图片样式
    THEME_STYLE_INCLASS(Theme_screen_Green)->image.color = greenColor;
    THEME_STYLE_INCLASS(Theme_screen_Green)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_Green)->body.main_color = greenColor;
    THEME_STYLE_INCLASS(Theme_screen_Green)->body.grad_color = greenColor;
    THEME_STYLE_INCLASS(Theme_screen_Green)->body.border.color = greenColor;
    THEME_STYLE_INCLASS(Theme_screen_Green)->text.color = greenColor;
    THEME_STYLE_INCLASS(Theme_screen_Green)->line.color = greenColor;
    THEME_STYLE_INCLASS(Theme_screen_Green)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_Green)->body.shadow.width = 0;

    THEME_STYLE_INCLASS(Theme_screen_Yellow) = lv_style_plain;     //暗绿色图片样式
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->image.color = yellowColor;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->body.main_color = yellowColor;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->body.grad_color = yellowColor;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->body.border.color = yellowColor;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->text.color = yellowColor;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->line.color = yellowColor;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_Yellow)->body.shadow.width = 0;

    THEME_STYLE_INCLASS(Theme_screen_Blue) = lv_style_plain;      //蓝色图片样式
    THEME_STYLE_INCLASS(Theme_screen_Blue)->image.color = blueColor;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->body.main_color = blueColor;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->body.grad_color = blueColor;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->body.border.color = blueColor;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->text.color = blueColor;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->line.color = blueColor;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_Blue)->body.shadow.width = 0;

    THEME_STYLE_INCLASS(Theme_screen_BlueDark) = lv_style_plain;  //暗蓝色图片样式
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->image.color = blueDarkColor;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->body.main_color = blueDarkColor;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->body.grad_color = blueDarkColor;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->body.border.color = blueDarkColor;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->text.color = blueDarkColor;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->line.color = blueDarkColor;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_BlueDark)->body.shadow.width = 0;

    THEME_STYLE_INCLASS(Theme_screen_Cyan) = lv_style_plain;      //青色色图片样式
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->image.color = cyanColor;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->body.main_color = cyanColor;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->body.grad_color = cyanColor;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->body.border.color = cyanColor;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->text.color = cyanColor;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->line.color = cyanColor;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_Cyan)->body.shadow.width = 0;

    THEME_STYLE_INCLASS(Theme_screen_Gray) = lv_style_plain;      //灰色图片样式
    THEME_STYLE_INCLASS(Theme_screen_Gray)->image.color = grayColor;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->image.intense = LV_OPA_100;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->body.main_color = grayColor;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->body.grad_color = grayColor;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->body.border.color = grayColor;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->text.color = grayColor;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->line.color = grayColor;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->body.border.width = 0;
    THEME_STYLE_INCLASS(Theme_screen_Gray)->body.shadow.width = 0;

}
