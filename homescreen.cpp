#include "homescreen.h"
#include "marlinui.h"
#include <lvgl>
#include "theme.h"
#include <multilang.h>
#include <resources.h>


// 按键功能函数
/**
 * @brief 工具按钮按下
 */
lv_res_t onToolButClocked(_lv_obj_t * obj)
{
    MarlinUi::getInstance()->showMenuScreen();
    return LV_RES_OK;
}

/**
 * @brief 打印按钮按下
 * @param obj
 * @return
 */
lv_res_t onPrintButClocked(_lv_obj_t * obj)
{
    MarlinUi::getInstance()->showFileScreen();
    return LV_RES_OK;
}

/**
 * @brief 打温度钮按下
 * @param obj
 * @return
 */
lv_res_t onTempButClocked(_lv_obj_t * obj)
{
    MarlinUi::getInstance()->showTemperatureScreen();
    return LV_RES_OK;
}

/**
 * @brief onMoveButClocked
 * @param obj
 * @return
 */
lv_res_t onMoveButClocked(_lv_obj_t * obj)
{
    MarlinUi::getInstance()->showMotionScreen();
    return LV_RES_OK;
}

lv_res_t onExtrudeButClocked(_lv_obj_t * obj)
{
    MarlinUi::getInstance()->showExtrudeScreen();
    return LV_RES_OK;
}

HomeScreen::HomeScreen()
    :Screen()
{
}

bool HomeScreen::initScreen()
{

    /*
     * 实现主界面
     *

    ==============================================
    =                                            =
    =  000000000000   11111111111   22222222222  =
    =  000000000000   11111111111   22222222222  =
    =  000000000000   11111111111   22222222222  =
    =  000000000000   11111111111   22222222222  =
    =  000000000000   11111111111   22222222222  =
    =                                            =
    =  333333333333   4444444444444444444444444  =
    =  333333333333   4444444444444444444444444  =
    =  333333333333   4444444444444444444444444  =
    =  333333333333   4444444444444444444444444  =
    =  333333333333   4444444444444444444444444  =
    =                                            =
    = ..........................5555..6666..7777 =
    = ..........................5555..6666..7777 =
    ==============================================

    */

    //构建主界面

    int screenWidth = this->width();
    int screenHeight = this->height();
    int screenDpi = this->dpi();

    //计算出各区域大小
    int16_t safeMargin = 6;
    int16_t spacing = 4;
    int16_t barHeight = 26;

    int16_t butAreaWidth = screenWidth;
    int16_t butAreaHeight = screenHeight - barHeight;

    int16_t butWidth = 100;
    int16_t butHeight = 100;

    int16_t fontSize = 16;
    int16_t iconSize = 24;

    //主界面按钮区域
    {
        LVObject * butArea = new LVObject(this,nullptr);
        butArea->setPos(0,0);
        butArea->setSize(butAreaWidth,butAreaHeight);
        butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));

        //移动按钮
        LVButton * butMove = new LVButton(butArea,nullptr);
        butMove->setSize(butWidth,butHeight);
        butMove->alignOrigo(LV_ALIGN_IN_TOP_MID,0,safeMargin + butHeight / 2);
        butMove->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
        butMove->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
        butMove->setAction(LV_BTN_ACTION_CLICK,onMoveButClocked);
        //移动按钮图标
        LVImage * imgMove = new LVImage(butMove,nullptr);
        imgMove->setSize(iconSize,iconSize);
        imgMove->align(LV_ALIGN_CENTER,0,0);
        imgMove->setSrc(&move_24);
        //移动按钮文本
        labMove = new LVLabel(butMove,nullptr);
        labMove->setText(Lang_HomeScreen_ButMove); //"Move"


        //预热按钮
        LVButton * butPreheat = new LVButton(butArea,butMove);
        butPreheat->align(butMove,LV_ALIGN_OUT_LEFT_TOP,-spacing,0);
        butPreheat->setAction(LV_BTN_ACTION_CLICK,onTempButClocked);
        //预热按钮图标
        LVImage * imgPreheat = new LVImage(butPreheat,nullptr);
        imgPreheat->setSize(iconSize,iconSize);
        imgPreheat->align(LV_ALIGN_CENTER,0,0);
        imgPreheat->setSrc(&temp_24);
        //预热按钮文本
        labPreheat = new LVLabel(butPreheat,nullptr);
        labPreheat->setText(Lang_HomeScreen_ButPreHeat);//"Preheat"


        //挤出按钮
        LVButton * butExtrude = new LVButton(butArea,butMove);
        butExtrude->align(butMove,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butExtrude->setAction(LV_BTN_ACTION_CLICK,onExtrudeButClocked);
        //挤出按钮图标
        LVImage * imgExtrude = new LVImage(butExtrude,nullptr);
        imgExtrude->setSize(iconSize,iconSize);
        imgExtrude->align(LV_ALIGN_CENTER,0,0);
        imgExtrude->setSrc(&extrude_24);
        //挤出按钮文本
        labExtrude = new LVLabel(butExtrude,nullptr);
        labExtrude->setText(Lang_HomeScreen_ButExtrude); //"Extrude"

        //工具按钮
        LVButton * butTool = new LVButton(butArea,butMove);
        butTool->align(butPreheat,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        butTool->setAction(LV_BTN_ACTION_CLICK,onToolButClocked);
        //工具按钮图标
        LVImage * imgTool = new LVImage(butTool,nullptr);
        imgTool->setSize(iconSize,iconSize);
        imgTool->align(LV_ALIGN_CENTER,0,0);
        imgTool->setSrc(&tool_24);
        //工具按钮文本
        labTool = new LVLabel(butTool,nullptr);
        labTool->setText(Lang_HomeScreen_ButTool);//"Tool"

        //打印按钮
        LVButton * butPrint = new LVButton(butArea,butMove);
        butPrint->setSize(butWidth*2 + spacing , butHeight);
        butPrint->align(butTool,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        butPrint->setAction(LV_BTN_ACTION_CLICK,onPrintButClocked);
        //打印按钮图标
        LVImage * imgPrint = new LVImage(butPrint,nullptr);
        imgPrint->setSize(iconSize,iconSize);
        imgPrint->align(LV_ALIGN_CENTER,0,0);
        imgPrint->setSrc(&print_24);
        //打印按钮文本
        labPrint = new LVLabel(butPrint,nullptr);
        labPrint->setText(Lang_HomeScreen_ButPrint);//"Print"

    }

    //主界面消息区
    {
        //状态栏
        LVObject * stateBar = new LVObject(this,nullptr);
        stateBar->setSize(screenWidth,barHeight);
        stateBar->align(LV_ALIGN_IN_BOTTOM_LEFT,0,0);
        stateBar->setStyle(THEME_STYLE_GET(Theme_Screen_StateBar));

        //状态栏信息
        stateMesg = new LVLabel(stateBar,nullptr);
        stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
        stateMesg->setText(Lang_HomeScreen_MsgConnected); //"Connected!"
        stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);

        //热床温度
        labHeatbed = new LVLabel(stateBar,nullptr);
        labHeatbed->setText("999");
        labHeatbed->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
        labHeatbed->setValue((int16_t)ExtUI::getActualTemp_celsius(ExtUI::BED));

        //热床温度图标
        imgHeatbed = new LVImage(stateBar,nullptr);
        imgHeatbed->setSrc(&heatbed_20_12);
        imgHeatbed->align(labHeatbed,LV_ALIGN_OUT_LEFT_MID,-spacing,0);

        //喷嘴温度
        labHeatend = new LVLabel(stateBar,nullptr);
        labHeatend->setText("999");
        labHeatend->align(imgHeatbed,LV_ALIGN_OUT_LEFT_MID,-safeMargin*2,0);
        labHeatend->setValue((int16_t)ExtUI::getActualTemp_celsius(ExtUI::E0));

        //喷嘴温度图标
        imgHeatend = new LVImage(stateBar,nullptr);
        imgHeatend->setSrc(&heatend_20_12);
        imgHeatend->align(labHeatend,LV_ALIGN_OUT_LEFT_MID,-spacing,0);

    }


    //温度查询任务
    m_tempQueryTask = new ScreenTask(this);
    m_tempQueryTask->setTaskFunc([&](){
        //更新热床/喷嘴温度
        labHeatbed->setValue((int16_t)ExtUI::getActualTemp_celsius(ExtUI::BED));
        labHeatend->setValue((int16_t)ExtUI::getActualTemp_celsius(ExtUI::E0));

        //更新图标状态颜色
        if(ExtUI::getTargetTemp_celsius(ExtUI::BED) > 5)
            imgHeatbed->setStyle(THEME_STYLE_GET(Theme_screen_Yellow));
        else
            imgHeatbed->setStyle(nullptr); //白色

        if(ExtUI::getTargetTemp_celsius(ExtUI::E0) > 5)
            imgHeatend->setStyle(THEME_STYLE_GET(Theme_screen_Red));
        else
            imgHeatend->setStyle(nullptr); //白色
    });
    m_tempQueryTask->startAndRun();

    LV_LOG_INFO(__FUNCTION__);

    return true;
}

void HomeScreen::onLangChanged()
{

    //更新界面语言

}

void HomeScreen::onThemeChanged()
{
}


void HomeScreen::afterCleanScreen()
{
    labMove = nullptr;
    labPreheat = nullptr;
    labExtrude = nullptr;
    labTool = nullptr;
    labPrint = nullptr;
    stateMesg = nullptr;

    labHeatbed = nullptr;
    labHeatend = nullptr;

    imgHeatbed = nullptr;
    imgHeatend = nullptr;
}
