#include "motionscreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"


using std::placeholders::_1;

float getUnit(MotionUnit mu)
{
    switch (mu)
    {
        case MU_05mm: return 0.5f ; break;
        case MU_1mm : return 1.0f ; break;
        case MU_10mm: return 10.0f; break;
        case MU_50mm: return 50.f ; break;
        default: return 1.0f; break;
    }
}

//选择运动距离
lv_res_t MotionScreen::onButMMClicked (struct _lv_obj_t * obj)
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

lv_res_t MotionScreen::onButMoveClicked (struct _lv_obj_t * obj)
{

    if(ExtUI::isMoving())
    {
       //提示正在移动中
       showBubble("System is moving, busy!",500);
    }

    LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
    //点击
    if(but == m_butXPos)
    {
        LV_LOG_INFO("+X Clicked ");
        if(ExtUI::canMove(ExtUI::X))
        {
            ExtUI::setFeedrate_mm_s(slowSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::X) + m_unit ,ExtUI::X);
        }
        else
        {
            showBubble("+X can not move",500);
        }
    }

    if(but == m_butXNeg)
    {
        LV_LOG_INFO("-x Clicked ");
        if(ExtUI::canMove(ExtUI::X))
        {
            ExtUI::setFeedrate_mm_s(slowSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::X) - m_unit ,ExtUI::X);
        }
        else
        {
            showBubble("-X can not move",500);
        }
    }

    if(but == m_butYPos)
    {
        LV_LOG_INFO("+Y Clicked ");
        if(ExtUI::canMove(ExtUI::Y))
        {
            ExtUI::setFeedrate_mm_s(slowSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Y) + m_unit ,ExtUI::Y);
        }
        else
        {
            showBubble("+Y can not move",500);
        }

    }

    if(but == m_butYNeg)
    {
        LV_LOG_INFO("-Y Clicked ");
        if(ExtUI::canMove(ExtUI::Y))
        {
            ExtUI::setFeedrate_mm_s(slowSpeed());
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Y) - m_unit ,ExtUI::Y);
        }
        else
        {
            showBubble("-Y can not move",500);
        }
    }

    if(but == m_butZPos)
    {
        LV_LOG_INFO("+Z Clicked ");
        if(ExtUI::canMove(ExtUI::Z))
        {
            ExtUI::setFeedrate_mm_s(10.0f);
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Z) + m_unit ,ExtUI::Z);
        }
        else
        {
            showBubble("+Z can not move",500);
        }
    }

    if(but == m_butZNeg)
    {
        LV_LOG_INFO("-Z Clicked ");
        if(ExtUI::canMove(ExtUI::Z))
        {
            ExtUI::setFeedrate_mm_s(10.0f);
            ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::Z) - m_unit ,ExtUI::Z);
        }
        else
        {
            showBubble("-Z can not move",500);
        }
    }

    return LV_RES_OK;
}

lv_res_t MotionScreen::onButHomeClicked(_lv_obj_t *obj)
{
    LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));

    //点击
    if(but == m_butHomeX)
    {
        LV_LOG_INFO("x Home ");
        ExtUI::enqueueCommands_P("G28 X\n");
    }
    else if(but == m_butHomeY)
    {
        LV_LOG_INFO("Y Home ");
        ExtUI::enqueueCommands_P("G28 Y\n");
    }
    else if(but == m_butHomeZ)
    {
        LV_LOG_INFO("Z Home ");
        ExtUI::enqueueCommands_P("G28 Z\n");
    }
    else if(but == m_butHome)
    {
        LV_LOG_INFO("XYZ Home ");
        ExtUI::enqueueCommands_P("G28\n");
    }

    return LV_RES_OK;
}

float MotionScreen::unit() const
{
    return m_unit;
}

void MotionScreen::setUnit(float unit)
{
    if(m_unit != unit)
        m_unit = unit;
}

void MotionScreen::setUnit(MotionUnit mu)
{
    setUnit(getUnit(mu));
}

void MotionScreen::setButtonUnit(LVButton *but, LVLabel *lab, float unit)
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

float MotionScreen::getButtonUnit(LVButton *but)
{
    float unit;
    uint32_t freenum = but->getFreeNumber();
    memcpy(&unit,&freenum,sizeof(float));
    return unit;
}

void MotionScreen::storeSelectUnit(LVButton *but)
{
    m_unit = getButtonUnit(but);
}

void MotionScreen::restoreSelectUnit()
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

float MotionScreen::fastSpeed() const
{
    return m_fastSpeed;
}

void MotionScreen::setFastSpeed(float fastSpeed)
{
    m_fastSpeed = fastSpeed;
}

float MotionScreen::slowSpeed() const
{
    return m_slowSpeed;
}

void MotionScreen::setSlowSpeed(float slowSpeed)
{
    m_slowSpeed = slowSpeed;
}

MotionScreen::MotionScreen()
{
    //设置按钮默认单位
    m_butUnits[0] = 0.5f;
    m_butUnits[1] = 1.0f;
    m_butUnits[2] = 10.0f;
    m_butUnits[3] = 50.0f;

    m_unit = 0.5f;

    m_fastSpeed = 100.0f; //!< 快速度
    m_slowSpeed = 60.0f; //!< 慢速度
}

MotionScreen::~MotionScreen()
{
}

bool MotionScreen::initScreen()
{
    //初始化运动界面

    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 6;
    const int16_t titleBarHeight = 32;
    const int16_t stateBarHeight = 26;

    const int16_t butMMWidth = 54;
    const int16_t butMMHeight = 36;

    const int16_t butMoveWidth = 87;
    const int16_t butMoveHeight = 34;

    const int16_t butAreaMargin = 10;

    const int16_t labAxisOffset = 26;

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
        labTitle->setText(Lang_MotionScreen_LabTitle);
        labTitle->align(butBack,LV_ALIGN_OUT_RIGHT_MID);

        //关闭电机按钮
        styleMotoOff = &lv_style_plain;
        styleMotoOff->body.empty = true;

        LVButton * butMotorOff = new LVButton(titleBar,nullptr);
        butMotorOff->setSize(titleBarHeight,titleBarHeight);
        butMotorOff->setStyle(LV_BTN_STYLE_REL,styleMotoOff);
        butMotorOff->setStyle(LV_BTN_STYLE_PR,styleMotoOff);
        butMotorOff->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
        butMotorOff->setAction([](lv_obj_t * obj)->lv_res_t{
            ExtUI::enqueueCommands_P("M18\n");
            Screen::showBubble("Disable all steppers");
                               } ,LV_BTN_ACTION_CLICK);
        LVImage * imgMotorOff = new LVImage(butMotorOff,nullptr);
        imgMotorOff->setSrc(&motorOff_24);

        //调平按钮
        LVButton * butLeveling = new LVButton(titleBar,nullptr);
        butLeveling->setSize(titleBarHeight,titleBarHeight);
        butLeveling->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butLeveling->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butLeveling->align(butMotorOff,LV_ALIGN_OUT_LEFT_TOP,-spacing,0);
        LVImage * imgLeveling = new LVImage(butLeveling,nullptr);
        imgLeveling->setSrc(&levelling_24);
        imgLeveling->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));


        //Y回零按钮
        m_butHomeY = new LVButton(titleBar,nullptr);
        m_butHomeY->setSize(titleBarHeight,titleBarHeight);
        m_butHomeY->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        m_butHomeY->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        m_butHomeY->align(LV_ALIGN_CENTER);
        m_butHomeY->setAction(std::bind(&MotionScreen::onButHomeClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgHomeY = new LVImage(m_butHomeY,nullptr);
        imgHomeY->setSrc(&homeY_24);
        imgHomeY->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));

        //Z回零按钮
        m_butHomeZ = new LVButton(titleBar,nullptr);
        m_butHomeZ->setSize(titleBarHeight,titleBarHeight);
        m_butHomeZ->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        m_butHomeZ->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        m_butHomeZ->align(m_butHomeY,LV_ALIGN_OUT_RIGHT_TOP,spacing,0);
        m_butHomeZ->setAction(std::bind(&MotionScreen::onButHomeClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgHomeZ = new LVImage(m_butHomeZ,nullptr);
        imgHomeZ->setSrc(&homeZ_24);
        imgHomeZ->setStyle(THEME_STYLE_GET(Theme_screen_Red));

        //X回零按钮
        m_butHomeX = new LVButton(titleBar,nullptr);
        m_butHomeX->setSize(titleBarHeight,titleBarHeight);
        m_butHomeX->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        m_butHomeX->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        m_butHomeX->align(m_butHomeY,LV_ALIGN_OUT_LEFT_TOP,-spacing,0);
        m_butHomeX->setAction(std::bind(&MotionScreen::onButHomeClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgHomeX = new LVImage(m_butHomeX,nullptr);
        imgHomeX->setSrc(&homeX_24);
        imgHomeX->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
    }

    {
        //功能区域

        LVObject * butArea = new LVObject(this,nullptr);
        butArea->align(LV_ALIGN_IN_TOP_LEFT,0,titleBarHeight);
        butArea->setSize(screenWidth,screenHeight - titleBarHeight - stateBarHeight);
        butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));


        styleDisBut = THEME_STYLE_GET(Theme_screen_Cyan);
        styleDisBut->body.empty = true;
        styleDisBut->body.border.width = 1;

        styleDisButSelect = THEME_STYLE_GET(Theme_screen_Cyan);
        styleDisButSelect->text.color = LV_COLOR_WHITE;

        styleHit = THEME_STYLE_GET(Theme_screen_Cyan);
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
        m_but05mm->setAction(std::bind(&MotionScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_lab05mm = new LVLabel(m_but05mm,nullptr);
        m_lab05mm->setText("0.5mm");
        m_lab05mm->setAlign(LV_LABEL_ALIGN_CENTER);
        //布局提示
        m_selectHit->align(m_but05mm,LV_ALIGN_IN_TOP_MID);

        m_but1mm  = new LVButton(butArea,m_but05mm);
        m_but1mm->setAction(std::bind(&MotionScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_but1mm->align(m_but05mm,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        m_lab1mm = new LVLabel(m_but1mm,m_lab05mm);
        m_lab1mm->setText("1mm");

        m_but10mm = new LVButton(butArea,m_but05mm);
        m_but10mm->setAction(std::bind(&MotionScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_but10mm->align(m_but1mm,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        m_lab10mm = new LVLabel(m_but10mm,m_lab05mm);
        m_lab10mm->setText("10mm");

        m_but50mm = new LVButton(butArea,m_but05mm);
        m_but50mm->setAction(std::bind(&MotionScreen::onButMMClicked,this,_1),LV_BTN_ACTION_PR);
        m_but50mm->align(m_but10mm,LV_ALIGN_OUT_BOTTOM_LEFT,0,spacing);
        m_lab50mm = new LVLabel(m_but50mm,m_lab05mm);
        m_lab50mm->setText("50mm");


        //移动XY
        //styleArcRed = THEME_STYLE_GET(Theme_screen_Red);
        //styleArcRed->line.width = arcWidrh;             /*Arc width*/
        //
        //styleArcBlue = THEME_STYLE_GET(Theme_screen_Blue);
        //styleArcBlue->line.width = arcWidrh;             /*Arc width*/
        //
        //styleArcCyan = THEME_STYLE_GET(Theme_screen_Cyan);
        //styleArcCyan->line.width = arcWidrh;             /*Arc width*/

        styleButTransp = &lv_style_plain;
        styleButTransp->body.empty = true; //按钮全透明
        styleButTransp->body.border.width = 0;
        styleButTransp->image.color = LV_COLOR_WHITE;
        styleButTransp->image.intense = LV_OPA_100;

        //先绘制底部的圆弧
        m_imgXPosArc = new LVImage(butArea,nullptr);
        m_imgXPosArc->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
        m_imgXPosArc->setSrc(&xPosArc_107_34);
        m_imgXPosArc->align(LV_ALIGN_CENTER, arcSize/2 - (m_imgXPosArc->getWidth()/2),0);

        m_imgXNegArc = new LVImage(butArea,m_imgXPosArc);
        m_imgXNegArc->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
        m_imgXNegArc->setSrc(&xNegArc_107_34);
        m_imgXNegArc->align(LV_ALIGN_CENTER,-arcSize/2 + (m_imgXNegArc->getWidth()/2),0);

        m_imgYPosArc = new LVImage(butArea,m_imgXPosArc);
        m_imgYPosArc->setSrc(&yPosArc_107_34);
        m_imgYPosArc->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));
        m_imgYPosArc->align(LV_ALIGN_CENTER,0,-arcSize/2 + (m_imgYPosArc->getHeight()/2));

        m_imgYNegArc = new LVImage(butArea,m_imgYPosArc);
        m_imgYNegArc->setSrc(&yNegArc_107_34);
        m_imgYNegArc->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));
        m_imgYNegArc->align(LV_ALIGN_CENTER,0,arcSize/2 - (m_imgYNegArc->getHeight()/2));

        m_imgZPosArc = new LVImage(butArea,m_imgXPosArc);
        m_imgZPosArc->setSrc(&zPosArc_82_27);
        m_imgZPosArc->setStyle(THEME_STYLE_GET(Theme_screen_Red));
        m_imgZPosArc->align(LV_ALIGN_CENTER,arcSize/7*5,-arcSize/2 + (m_imgZPosArc->getHeight()/2) );

        m_imgZNegArc = new LVImage(butArea,m_imgZPosArc);
        m_imgZNegArc->setSrc(&zNegArc_82_27);
        m_imgZNegArc->setStyle(THEME_STYLE_GET(Theme_screen_Red));
        m_imgZNegArc->align(LV_ALIGN_CENTER,arcSize/7*5,arcSize/2 - (m_imgZNegArc->getHeight()/2) );


        LVLabel * labXPos = new LVLabel(butArea,nullptr);
        labXPos->setText("+X");
        labXPos->align(m_imgXPosArc,LV_ALIGN_IN_RIGHT_MID,-labAxisOffset);
        labXPos->setStyle(THEME_STYLE_GET(Theme_screen_Blue));

        LVLabel * labXNeg = new LVLabel(butArea,nullptr);
        labXNeg->setText("-X");
        labXNeg->align(m_imgXNegArc,LV_ALIGN_IN_LEFT_MID,labAxisOffset);
        labXNeg->setStyle(THEME_STYLE_GET(Theme_screen_Blue));

        LVLabel * labYPos = new LVLabel(butArea,nullptr);
        labYPos->setText("+Y");
        labYPos->align(m_imgYPosArc,LV_ALIGN_IN_TOP_MID,0,labAxisOffset);
        labYPos->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));

        LVLabel * labYNeg = new LVLabel(butArea,nullptr);
        labYNeg->setText("-Y");
        labYNeg->align(m_imgYNegArc,LV_ALIGN_IN_BOTTOM_MID,0,-labAxisOffset);
        labYNeg->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));

        LVLabel * labZPos = new LVLabel(butArea,nullptr);
        labZPos->setText("+Z");
        labZPos->align(m_imgZPosArc,LV_ALIGN_IN_TOP_MID,0,labAxisOffset);
        labZPos->setStyle(THEME_STYLE_GET(Theme_screen_Red));

        LVLabel * labZNeg = new LVLabel(butArea,nullptr);
        labZNeg->setText("-Z");
        labZNeg->align(m_imgZNegArc,LV_ALIGN_IN_BOTTOM_MID,0,-labAxisOffset);
        labZNeg->setStyle(THEME_STYLE_GET(Theme_screen_Red));


        //实际的运动按钮
        m_butXPos = new LVButton(butArea,nullptr);
        m_butXPos->setSize(butMoveHeight,butMoveWidth);
        m_butXPos->setStyle(LV_BTN_STYLE_PR,styleButTransp);
        m_butXPos->setStyle(LV_BTN_STYLE_REL,styleButTransp);
        m_butXPos->setAction(std::bind(&MotionScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butXPos->align(m_imgXPosArc,LV_ALIGN_IN_RIGHT_MID);
        m_butXPos->setLayout(LV_LAYOUT_OFF);
        LVImage * imgXPos = new LVImage(m_butXPos,nullptr);
        imgXPos->setSrc(&right_8_14);
        imgXPos->align(LV_ALIGN_IN_RIGHT_MID,-spacing,0);

        m_butXNeg = new LVButton(butArea,m_butXPos);
        m_butXNeg->align(m_imgXNegArc,LV_ALIGN_IN_LEFT_MID);
        m_butXNeg->setAction(std::bind(&MotionScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butXNeg->setLayout(LV_LAYOUT_OFF);
        LVImage * imgXNeg = new LVImage(m_butXNeg,nullptr);
        imgXNeg->setSrc(&left_8_14);
        imgXNeg->align(LV_ALIGN_IN_LEFT_MID,spacing,0);

        m_butYPos = new LVButton(butArea,m_butXPos);
        m_butYPos->setSize(butMoveWidth,butMoveHeight);
        m_butYPos->align(m_imgYPosArc,LV_ALIGN_IN_TOP_MID);
        m_butYPos->setAction(std::bind(&MotionScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butYPos->setLayout(LV_LAYOUT_OFF);
        LVImage * imgYPos = new LVImage(m_butYPos,nullptr);
        imgYPos->setSrc(&up_14_8);
        imgYPos->align(LV_ALIGN_IN_TOP_MID,0,spacing);

        m_butYNeg = new LVButton(butArea,m_butYPos);
        m_butYNeg->align(m_imgYNegArc,LV_ALIGN_IN_BOTTOM_MID);
        m_butYNeg->setAction(std::bind(&MotionScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butYNeg->setLayout(LV_LAYOUT_OFF);
        LVImage * imgYNeg = new LVImage(m_butYNeg,nullptr);
        imgYNeg->setSrc(&down_14_8);
        imgYNeg->align(LV_ALIGN_IN_BOTTOM_MID,0,-spacing);

        m_butZPos = new LVButton(butArea,m_butYPos);
        m_butZPos->align(m_imgZPosArc,LV_ALIGN_IN_TOP_MID);
        m_butZPos->setAction(std::bind(&MotionScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butZPos->setLayout(LV_LAYOUT_OFF);
        LVImage * imgZPos = new LVImage(m_butZPos,nullptr);
        imgZPos->setSrc(&up_14_8);
        imgZPos->align(LV_ALIGN_IN_TOP_MID,0,spacing);

        m_butZNeg = new LVButton(butArea,m_butYPos);
        m_butZNeg->align(m_imgZNegArc,LV_ALIGN_IN_BOTTOM_MID);
        m_butZNeg->setAction(std::bind(&MotionScreen::onButMoveClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butZNeg->setLayout(LV_LAYOUT_OFF);
        LVImage * imgZNeg = new LVImage(m_butZNeg,nullptr);
        imgZNeg->setSrc(&down_14_8);
        imgZNeg->align(LV_ALIGN_IN_BOTTOM_MID,0,-spacing);

        m_butHome = new LVButton(butArea,m_butYPos);
        m_butHome->setSize(home_34.header.w,home_34.header.h);
        m_butHome->align(LV_ALIGN_CENTER);
        m_butHome->setAction(std::bind(&MotionScreen::onButHomeClicked,this,_1),LV_BTN_ACTION_CLICK);
        LVImage * imgHome = new LVImage(m_butHome,nullptr);
        imgHome->setSrc(&home_34);

        LVImage * imgZUpDown = new LVImage(butArea,nullptr);
        imgZUpDown->setSrc(&upDown_34);
        imgZUpDown->align(LV_ALIGN_CENTER,arcSize/7*5,0);
    }

    {
        //状态栏
        LVObject * stateBar = new LVObject(this,nullptr);
        stateBar->setSize(screenWidth,stateBarHeight);
        stateBar->align(LV_ALIGN_IN_BOTTOM_LEFT,0,0);
        //状态栏样式
        stateBar->setStyle(THEME_STYLE_GET(Theme_Screen_StateBar));

        //状态栏信息
        m_labXPos = new LVLabel(stateBar,nullptr);
        m_labXPos->setText("X: ");
        m_labXPos->setWidth(labPosWidth);
        m_labXPos->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);

        m_labYPos = new LVLabel(stateBar,nullptr);
        m_labYPos->setText("Y: ");
        m_labYPos->setWidth(labPosWidth);
        m_labYPos->align(m_labXPos,LV_ALIGN_IN_LEFT_MID,labPosWidth,0);

        m_labZPos = new LVLabel(stateBar,nullptr);
        m_labZPos->setText("Z: ");
        m_labZPos->setWidth(labPosWidth);
        m_labZPos->align(m_labYPos,LV_ALIGN_IN_LEFT_MID,labPosWidth,0);
    }


    m_positionQueryTask = new ScreenTask(this,500);
    m_positionQueryTask->setTaskFunc([this](){

        char tempStr[16]; //!< 零时字符

        //查询运动轴位置
        sprintf(tempStr,"X: %s", ExtUI::isAxisPositionKnown(ExtUI::X) ? ftos(ExtUI::getAxisPosition_mm(ExtUI::X)) : m_showQuestionMark ? "????" :ftos(ExtUI::getAxisPosition_mm(ExtUI::X)) );
        m_labXPos->setText(tempStr);

        sprintf(tempStr,"Y: %s", ExtUI::isAxisPositionKnown(ExtUI::Y) ? ftos(ExtUI::getAxisPosition_mm(ExtUI::Y)) : m_showQuestionMark ? "????" :ftos(ExtUI::getAxisPosition_mm(ExtUI::Y)) );
        m_labYPos->setText(tempStr);

        sprintf(tempStr,"Z: %s", ExtUI::isAxisPositionKnown(ExtUI::Z) ? ftos(ExtUI::getAxisPosition_mm(ExtUI::Z)) : m_showQuestionMark ? "????" :ftos(ExtUI::getAxisPosition_mm(ExtUI::Z)) );
        m_labZPos->setText(tempStr);

        m_showQuestionMark = !m_showQuestionMark;

    });
    m_positionQueryTask->startAndRun();



    LV_LOG_INFO(__FUNCTION__);

    return true;
}

void MotionScreen::onLangChanged()
{
}

void MotionScreen::onThemeChanged()
{
}



bool MotionScreen::beforeShow()
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

void MotionScreen::beforeCleanScreen()
{
    //保存按钮的设定单位
    m_butUnits[0] = getButtonUnit(m_but05mm);
    m_butUnits[1] = getButtonUnit(m_but1mm);
    m_butUnits[2] = getButtonUnit(m_but10mm);
    m_butUnits[3] = getButtonUnit(m_but50mm);
}


void MotionScreen::afterCleanScreen()
{
    m_positionQueryTask = nullptr;

    m_butHomeX = nullptr;
    m_butHomeY = nullptr;
    m_butHomeZ = nullptr;
    m_butHome = nullptr;

    m_selectHit = nullptr;

    m_but05mm = nullptr;
    m_but1mm = nullptr;
    m_but10mm = nullptr;
    m_but50mm = nullptr;

    m_lab05mm = nullptr;
    m_lab1mm = nullptr;
    m_lab10mm = nullptr;
    m_lab50mm = nullptr;

    m_butXPos = nullptr;
    m_butXNeg = nullptr;
    m_butYPos = nullptr;
    m_butYNeg = nullptr;
    m_butZPos = nullptr;
    m_butZNeg = nullptr;

    m_labXPos = nullptr;
    m_labYPos = nullptr;
    m_labZPos = nullptr;
}
