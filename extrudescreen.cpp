#include "extrudescreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"

using std::placeholders::_1;

//选择运动距离
lv_res_t ExtrudeScreen::onButMMClicked (struct _lv_obj_t * obj)
{
    LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));

    if(but&&m_selectHit&&m_but05mm&&m_but1mm&&m_but10mm&&m_but50mm)
    {
        //互斥选中状态
        m_but05mm->setChecked(but == m_but05mm);
        m_but1mm->setChecked(but == m_but1mm);
        m_but10mm->setChecked(but == m_but10mm);
        m_but50mm->setChecked(but == m_but50mm);

        //更新提示位置
        m_selectHit->align(but,LV_ALIGN_IN_TOP_MID);

        //记录当前选中的单位
        storeSelectUnit(but);
    }

    return LV_RES_OK;
}


lv_res_t ExtrudeScreen::onButMoveClicked (struct _lv_obj_t * obj)
{
    if(ExtUI::isMoving())
    {
        //提示正在移动中
        showBubble("System is moving, busy!",500);
    }

    if(!ExtUI::canMove(ExtUI::E0))
    {
        showBubble("E can not move",500);
    }
    else
    {
        LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
        //点击
        if(but == m_butENegF)
        {
            LV_LOG_INFO("-E fast Clicked ");
            ExtUI::setFeedrate_mm_s(fastSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::E0) - m_unit ,ExtUI::E0);
        }

        if(but == m_butENegS)
        {
            LV_LOG_INFO("-E slow Clicked ");
            ExtUI::setFeedrate_mm_s(slowSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::E0) - m_unit ,ExtUI::E0);
        }

        if(but == m_butEPosS)
        {
            LV_LOG_INFO("+E slow Clicked ");
            ExtUI::setFeedrate_mm_s(slowSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::E0) + m_unit ,ExtUI::E0);
        }

        if(but == m_butEPosF)
        {
            LV_LOG_INFO("+E fast Clicked ");
            ExtUI::setFeedrate_mm_s(fastSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::E0) + m_unit ,ExtUI::E0);
        }
    }

    return LV_RES_OK;
}


void ExtrudeScreen::setButtonUnit(LVButton *but, LVLabel *lab, float unit)
{
    //保存单位到按钮中
    uint32_t freenum;
    memcpy(&freenum,&unit,sizeof(float));
    but->setFreeNumber(freenum);

    //更新按钮单位值
    char tempStr [10];
    sprintf(tempStr,"%smm",ftos(unit));
    lab->setText(tempStr);
}

float ExtrudeScreen::getButtonUnit(LVButton *but)
{
    float unit;
    uint32_t freenum = but->getFreeNumber();
    memcpy(&unit,&freenum,sizeof(float));
    return unit;
}

void ExtrudeScreen::storeSelectUnit(LVButton *but)
{
    m_unit = getButtonUnit(but);
}

void ExtrudeScreen::restoreSelectUnit()
{
    m_but05mm->setChecked(m_unit == getButtonUnit(m_but05mm));
    m_but1mm->setChecked(m_unit == getButtonUnit(m_but1mm));
    m_but10mm->setChecked(m_unit == getButtonUnit(m_but10mm));
    m_but50mm->setChecked(m_unit == getButtonUnit(m_but50mm));

    LVButton * but = nullptr;
    if(m_but05mm->checked()) but = m_but05mm;
    else if(m_but1mm->checked()) but = m_but1mm;
    else if(m_but10mm->checked()) but = m_but10mm;
    else if(m_but50mm->checked()) but = m_but50mm;

    if(but == nullptr)
    {
        but = m_but05mm;
        m_unit = getButtonUnit(m_but05mm);
    }

    m_selectHit->align(but,LV_ALIGN_IN_TOP_MID);
}

float ExtrudeScreen::getUnit() const
{
    return m_unit;
}

void ExtrudeScreen::setUnit(float unit)
{
    m_unit = unit;
}

float ExtrudeScreen::fastSpeed() const
{
    return m_fastSpeed;
}

void ExtrudeScreen::setFastSpeed(float fastSpeed)
{
    m_fastSpeed = fastSpeed;
}

float ExtrudeScreen::slowSpeed() const
{
    return m_slowSpeed;
}

void ExtrudeScreen::setSlowSpeed(float slowSpeed)
{
    m_slowSpeed = slowSpeed;
}

ExtrudeScreen::ExtrudeScreen()
{
    //设置按钮默认单位
    m_butUnits[0] = 0.5f;
    m_butUnits[1] = 1.0f;
    m_butUnits[2] = 10.0f;
    m_butUnits[3] = 50.0f;

    m_unit = 0.5f;

    m_fastSpeed = 40.0f; //!< 快速度
    m_slowSpeed = 15.0f; //!< 慢速度
}

bool ExtrudeScreen::initScreen()
{

    //初始化挤出界面

    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 6;
    const int16_t titleBarHeight = 32;
    const int16_t stateBarHeight = 26;

    const int16_t butMMWidth = 54;
    const int16_t butMMHeight = 36;

    const int16_t butMoveWidth = 80;
    const int16_t butMoveHeight = 20;

    const int16_t butAreaMargin = 10;

    const int16_t butLoadSize = 75;

    const int16_t arcWidrh = 20;
    const int16_t arcSize = 160;

    const int16_t labPosWidth = 80;

    const int16_t chartWidth = 180;
    const int16_t chartHeight = 140;

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
        butBack->setAction(LV_BTN_ACTION_CLICK,onButBackHomeClicked);
        LVImage * imgBack = new LVImage(butBack,nullptr);
        imgBack->setSrc(&back_24);

        //标题
        LVLabel* labTitle = new LVLabel(titleBar,nullptr);
        labTitle->setText(Lang_ExtrudeScreen_LabTitle);
        labTitle->align(butBack,LV_ALIGN_OUT_RIGHT_MID);

        //设置按钮
        LVButton * butSetting = new LVButton(titleBar,nullptr);
        butSetting->setSize(titleBarHeight,titleBarHeight);
        butSetting->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butSetting->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butSetting->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
        //butHistory->setAction(LV_BTN_ACTION_CLICK,onButBackHomeClicked);
        LVImage * imgSetting = new LVImage(butSetting,nullptr);
        imgSetting->setSrc(&setting2_24);
        imgSetting->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));

        //温度
        LVButton * butTemp = new LVButton(titleBar,butSetting);
        butTemp->setSize(titleBarHeight,titleBarHeight);
        butTemp->align(butSetting,LV_ALIGN_OUT_LEFT_TOP,-spacing,0);
        butTemp->setAction([](struct _lv_obj_t *)->lv_res_t{
            MarlinUi::getInstance()->showTemperatureScreen();
        },LV_BTN_ACTION_CLICK);
        LVImage * imgTemp = new LVImage(butTemp,nullptr);
        imgTemp->setSrc(&temp_24);
        imgTemp->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
    }

    {
        //功能区域

        LVObject * butArea = new LVObject(this,nullptr);
        butArea->align(LV_ALIGN_IN_TOP_LEFT,0,titleBarHeight);
        butArea->setSize(screenWidth,screenHeight - titleBarHeight - stateBarHeight);
        butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));


        styleDisBut = (THEME_STYLE_GET(Theme_screen_Cyan));
        styleDisBut->body.empty = true;
        styleDisBut->body.border.width = 1;

        styleDisButSelect = (THEME_STYLE_GET(Theme_screen_Cyan));
        styleDisButSelect->text.color = LV_COLOR_WHITE;

        styleHit = (THEME_STYLE_GET(Theme_screen_Cyan));
        styleHit->body.empty = true;

        //选中标识
        m_selectHit = new LVObject(butArea,nullptr);
        m_selectHit->setSize(right_8_14.header.w + butMMWidth + left_8_14.header.w+spacing,butMMHeight);
        m_selectHit->setStyle(styleHit);
        LVImage * imgRightHit = new LVImage(m_selectHit,nullptr);
        imgRightHit->setSrc(&right_8_14);
        imgRightHit->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));
        imgRightHit->align(LV_ALIGN_IN_LEFT_MID,0,-imgRightHit->getWidth()/2);
        LVImage * imgLeftHit = new LVImage(m_selectHit,nullptr);
        imgLeftHit->setSrc(&left_8_14);
        imgLeftHit->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));
        imgLeftHit->align(LV_ALIGN_IN_RIGHT_MID,0,-imgLeftHit->getWidth()/2);

        //速度按钮
        m_but05mm = new LVButton(butArea,nullptr);
        m_but05mm->setSize(butMMWidth,butMMHeight);
        m_but05mm->setToggle(true);
        m_but05mm->setStyle(LV_BTN_STYLE_TGL_PR,styleDisBut);
        m_but05mm->setStyle(LV_BTN_STYLE_REL,styleDisBut);
        m_but05mm->setStyle(LV_BTN_STYLE_TGL_REL,styleDisButSelect);
        m_but05mm->setStyle(LV_BTN_STYLE_PR,styleDisButSelect);
        m_but05mm->align(LV_ALIGN_IN_TOP_LEFT,safeMargin+right_8_14.header.w+spacing/2,butAreaMargin);
        m_but05mm->setAction(std::bind(&ExtrudeScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_lab05mm = new LVLabel(m_but05mm,nullptr);
        m_lab05mm->setText("0.5mm");
        m_lab05mm->setAlign(LV_LABEL_ALIGN_CENTER);
        //布局提示
        m_selectHit->align(m_but05mm,LV_ALIGN_IN_TOP_MID);

        m_but1mm  = new LVButton(butArea,m_but05mm);
        m_but1mm->align(m_but05mm,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        m_but1mm->setAction(std::bind(&ExtrudeScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_lab1mm = new LVLabel(m_but1mm,m_lab05mm);
        m_lab1mm->setText("1mm");

        m_but10mm = new LVButton(butArea,m_but05mm);
        m_but10mm->align(m_but1mm,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        m_but10mm->setAction(std::bind(&ExtrudeScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_lab10mm = new LVLabel(m_but10mm,m_lab05mm);
        m_lab10mm->setText("10mm");

        m_but50mm = new LVButton(butArea,m_but05mm);
        m_but50mm->align(m_but10mm,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        m_but50mm->setAction(std::bind(&ExtrudeScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_lab50mm = new LVLabel(m_but50mm,m_lab05mm);
        m_lab50mm->setText("50mm");


        styleArcRed = THEME_STYLE_GET(Theme_screen_Red);
        styleArcRed->line.width = arcWidrh;

        styleButTransp = &lv_style_plain;
        styleButTransp->body.empty = true; //按钮全透明
        styleButTransp->body.border.width = 0;
        styleButTransp->image.color = LV_COLOR_WHITE;
        styleButTransp->image.intense = LV_OPA_100;

        //先绘制底部的圆弧
        //快速 fast
        LVArc * arcENegF = new LVArc(butArea,nullptr);
        arcENegF->setSize(arcSize,arcSize);
        arcENegF->setAngles(135,225);
        arcENegF->align(LV_ALIGN_CENTER);
        arcENegF->setStyle(LV_ARC_STYLE_MAIN,styleArcRed);

        LVArc * arcEPosF = new LVArc(butArea,arcENegF);
        arcEPosF->setAngles(315,45);

        //慢速 slow
        LVArc * arcENegS = new LVArc(butArea,arcENegF);
        arcENegS->setSize(arcSize-arcWidrh*2-spacing,arcSize-arcWidrh*2-spacing);
        arcENegS->setAngles(135,225);
        arcENegS->align(LV_ALIGN_CENTER);

        LVArc * arcEPosS = new LVArc(butArea,arcENegS);
        arcEPosS->setAngles(315,45);

        LVImage * imgExtrude = new LVImage(arcENegF,nullptr);
        imgExtrude->setSrc(&extrude_24);
        imgExtrude->align(LV_ALIGN_CENTER);

        //实际的运动按钮
        m_butENegF = new LVButton(butArea,nullptr);
        m_butENegF->setSize(butMoveWidth,butMoveHeight);
        m_butENegF->setStyle(LV_BTN_STYLE_PR,styleButTransp);
        m_butENegF->setStyle(LV_BTN_STYLE_REL,styleButTransp);
        m_butENegF->align(arcENegF,LV_ALIGN_IN_TOP_MID);
        m_butENegF->setAction(std::bind(&ExtrudeScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgENegF = new LVImage(m_butENegF,nullptr);
        imgENegF->setSrc(&upCornor_20_10);

        m_butENegS = new LVButton(butArea,m_butENegF);
        m_butENegS->align(arcENegS,LV_ALIGN_IN_TOP_MID);
        m_butENegS->setAction(std::bind(&ExtrudeScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgENegS = new LVImage(m_butENegS,nullptr);
        imgENegS->setSrc(&upCornor_20_10);

        LVLabel * labENeg = new LVLabel(butArea,nullptr);
        labENeg->setText("-E");
        labENeg->align(m_butENegS,LV_ALIGN_OUT_BOTTOM_MID,0,spacing/2);
        labENeg->setStyle(styleArcRed);

        m_butEPosF = new LVButton(butArea,m_butENegF);
        m_butEPosF->align(arcEPosF,LV_ALIGN_IN_BOTTOM_MID);
        m_butEPosF->setAction(std::bind(&ExtrudeScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgEPosF = new LVImage(m_butEPosF,nullptr);
        imgEPosF->setSrc(&downCornor_20_10);

        m_butEPosS = new LVButton(butArea,m_butEPosF);
        m_butEPosS->align(arcEPosS,LV_ALIGN_IN_BOTTOM_MID);
        m_butEPosS->setAction(std::bind(&ExtrudeScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgEPosS = new LVImage(m_butEPosS,nullptr);
        imgEPosS->setSrc(&downCornor_20_10);

        LVLabel * labEPos = new LVLabel(butArea,nullptr);
        labEPos->setText("+E");
        labEPos->align(m_butEPosS,LV_ALIGN_OUT_TOP_MID,0,-spacing/2);
        labEPos->setStyle(styleArcRed);

        LVButton * butUnload = new LVButton(butArea,nullptr);
        butUnload->setSize(butLoadSize,butLoadSize);
        butUnload->align(LV_ALIGN_IN_TOP_RIGHT,-spacing*2,spacing);
        butUnload->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
        butUnload->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
        butUnload->setAction([this](struct _lv_obj_t * obj)->lv_res_t{
            //耗材卸载
            ExtUI::enqueueCommands_P("M702 \n");
        },LV_BTN_ACTION_CLICK);
        LVImage * imgUnload = new LVImage(butUnload,nullptr);
        imgUnload->setSrc(&loadFilament_24);
        LVLabel * labUnload = new LVLabel(butUnload,nullptr);
        labUnload->setText("Unload");

        LVButton * butLoad = new LVButton(butArea,nullptr);
        butLoad->setSize(butLoadSize,butLoadSize);
        butLoad->align(LV_ALIGN_IN_BOTTOM_RIGHT,-spacing*2,-spacing);
        butLoad->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
        butLoad->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
        butLoad->setAction([this](struct _lv_obj_t * obj)->lv_res_t{
            //耗材加载
            ExtUI::enqueueCommands_P("M701 \n");
        },LV_BTN_ACTION_CLICK);
        LVImage * imgLoad = new LVImage(butLoad,nullptr);
        imgLoad->setSrc(&loadFilament_24);
        LVLabel * labLoad = new LVLabel(butLoad,nullptr);
        labLoad->setText("Load");

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
        stateMesg->setText(Lang_ExtrudeScreen_MesgHit); //"Connected!"
        stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);

        //喷嘴温度
        m_labHeatend = new LVLabel(stateBar,nullptr);
        m_labHeatend->setText("999");
        m_labHeatend->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
        m_labHeatend->setValue(ExtUI::getActualTemp_celsius(ExtUI::E0));

        //喷嘴温度图标
        m_imgHeatend = new LVImage(stateBar,nullptr);
        m_imgHeatend->setSrc(&heatend_20_12);
        m_imgHeatend->align(m_labHeatend,LV_ALIGN_OUT_LEFT_MID,-spacing,0);

        m_labEPos = new LVLabel(stateBar,nullptr);
        m_labEPos->setText("E: ");
        m_labEPos->setWidth(labPosWidth);
        m_labEPos->align(m_imgHeatend,LV_ALIGN_IN_LEFT_MID,-labPosWidth,0);

    }

    //初始化温度查询任务
    m_queryTask = new ScreenTask(this,1000);//任务启停由屏幕显隐控制
    m_queryTask->setTaskFunc([=](){
        //更新热床/喷嘴温度
        m_labHeatend->setValue(ExtUI::getActualTemp_celsius(ExtUI::E0));
        //更新图标颜色
        if(ExtUI::getTargetTemp_celsius(ExtUI::E0) > 5)
            m_imgHeatend->setStyle(THEME_STYLE_GET(Theme_screen_Red));
        else
            m_imgHeatend->setStyle(nullptr); //白色

        char tempStr[16];
        //查询运动轴位置
        sprintf(tempStr,"E: %s",ftos(ExtUI::getAxisPosition_mm(ExtUI::E0)) );
        m_labEPos->setText(tempStr);

    });
    m_queryTask->startAndRun();

    LV_LOG_INFO(__FUNCTION__);

    return true;

}

void ExtrudeScreen::onLangChanged()
{
}

void ExtrudeScreen::onThemeChanged()
{
}



bool ExtrudeScreen::beforeShow()
{
    bool wasInites = isInited();
    bool ret = Screen::beforeShow();
    if(ret)
    {
        //恢复按钮单位
        setButtonUnit(m_but05mm,m_lab05mm,m_butUnits[0]);
        setButtonUnit(m_but1mm ,m_lab1mm ,m_butUnits[1]);
        setButtonUnit(m_but10mm,m_lab10mm,m_butUnits[2]);
        setButtonUnit(m_but50mm,m_lab50mm,m_butUnits[3]);

        restoreSelectUnit();
    }

    return ret;
}

void ExtrudeScreen::beforeCleanScreen()
{
    //保存按钮的设定单位
    m_butUnits[0] = getButtonUnit(m_but05mm);
    m_butUnits[1] = getButtonUnit(m_but1mm);
    m_butUnits[2] = getButtonUnit(m_but10mm);
    m_butUnits[3] = getButtonUnit(m_but50mm);
}


void ExtrudeScreen::afterCleanScreen()
{
    m_selectHit = nullptr;

    m_but05mm = nullptr;
    m_but1mm = nullptr;
    m_but10mm = nullptr;
    m_but50mm = nullptr;

    m_lab05mm = nullptr;
    m_lab1mm = nullptr;
    m_lab10mm = nullptr;
    m_lab50mm = nullptr;

    m_butENegF = nullptr;
    m_butENegS = nullptr;
    m_butEPosF = nullptr;
    m_butEPosS = nullptr;

    m_imgHeatend = nullptr;
    m_labHeatend = nullptr;
    m_labEPos = nullptr;
}
