#include "temperaturescreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"
#include "numberimputscreen.h"

LVLabel *TemperatureScreen::labHeatEnd()
{
    if(m_labHeatEnd == nullptr)
       m_labHeatEnd = new LVLabel(this,nullptr);
    return m_labHeatEnd;
}

LVLabel *TemperatureScreen::labHeatBed()
{
    if(m_labHeatBed == nullptr)
       m_labHeatBed = new LVLabel(this,nullptr);
    return m_labHeatBed;
}

LVLabel *TemperatureScreen::labHeatEndValue()
{
    if(m_labHeatEndValue == nullptr)
       m_labHeatEndValue = new LVLabel(this,nullptr);
    return m_labHeatEndValue;
}

LVLabel *TemperatureScreen::labHeatBedValue()
{
    if(m_labHeatBedValue == nullptr)
       m_labHeatBedValue = new LVLabel(this,nullptr);
    return m_labHeatBedValue;
}

void TemperatureScreen::setHeatEndValue(int16_t value)
{
    if(m_heatEndValue != value)
    {
        m_heatEndValue = value;
        if(isInited())
            labHeatEndValue()->setValue(value);

        if(m_heatEndSwitch)
        {
            //NOTE:更新目标温度
            ExtUI::setTargetTemp_celsius(value,ExtUI::E0);
        }
    }
}

void TemperatureScreen::setHeatBedValue(int16_t value)
{
    if(m_heatBedValue != value)
    {
        m_heatBedValue = value;
        if(isInited())
            labHeatBedValue()->setValue(value);

        if(m_heatBedSwitch)
        {
            //NOTE:更新目标温度
            ExtUI::setTargetTemp_celsius(value,ExtUI::BED);
        }
    }
}

void TemperatureScreen::setHeatEndSwitch(bool value)
{
    if(m_heatEndSwitch != value)
    {
        m_heatEndSwitch = value;
        ExtUI::setTargetTemp_celsius(value ? m_heatEndValue : 0,ExtUI::E0);
        if(isInited())
            butHeatEndSwitch()->setChecked(value);
    }
}

void TemperatureScreen::setHeatBedSwitch(bool value)
{
    if(m_heatBedSwitch != value)
    {
        m_heatBedSwitch = value;
        ExtUI::setTargetTemp_celsius(value?m_heatBedValue:0,ExtUI::BED);
        if(isInited())
            butHeatBedSwitch()->setChecked(value);
    }
}

LVButton *TemperatureScreen::butHeatEndSwitch()
{
    if(m_butHeatEndSwitch == nullptr)
       m_butHeatEndSwitch = new LVButton(this,nullptr);
    return m_butHeatEndSwitch;
}

LVButton *TemperatureScreen::butHeatBedSwitch()
{
    if(m_butHeatBedSwitch == nullptr)
       m_butHeatBedSwitch = new LVButton(this,nullptr);
    return m_butHeatBedSwitch;
}

TemperatureScreen::TemperatureScreen() : Screen()
{
    //设置图标线段顶点
    m_serialBedBackUp.points = m_serialBedPoints;
    m_serialExtrude0BackUp.points = m_serialExtrude0Points;
    m_serialBedTargetBackUp.points = m_serialBedTargetPoints;
    m_serialExtrude0TargetBackUp.points = m_serialExtrude0TartgetPoints;
}

TemperatureScreen::~TemperatureScreen()
{
}

bool TemperatureScreen::initScreen()
{
    //初始化界面

    int16_t screenWidth = this->width();
    int16_t screenHeight = this->height();
    int16_t screendpi = this->dpi();

#if SCREEN_SIZE ==28
    int16_t safeMargin = 6;
    int16_t spacing = 6;
    int16_t titleBarHeight = 32;
    int16_t stateBarHeight = 26;

    int16_t butWidth = 48;
    int16_t butHeight = 48;

    int16_t chartWidth = 180;
    int16_t chartHeight = 140;

    int16_t fontSize = 16;
    int16_t iconSize = 24;

    int16_t scaleSpacing = 3;
#elif SCREEN_SIZE ==35
    int16_t safeMargin = 7;
    int16_t spacing = 7;
    int16_t titleBarHeight = 38;
    int16_t stateBarHeight = 32;

    int16_t butWidth = 70;
    int16_t butHeight = 70;

    int16_t chartWidth = 300;
    int16_t chartHeight = 190;

    int16_t fontSize = 20;
    int16_t iconSize = 28;

    int16_t scaleSpacing = 4;
#endif

    LVObject * reference = nullptr; //布局参考对象

    //标题栏////////////////////////////////////////////////////////
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
    labTitle->setText(Lang_TemperatureScreen_LabTitle);
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

    //挤出界面
    LVButton * butExtrude = new LVButton(titleBar,butSetting);
    butExtrude->setSize(titleBarHeight,titleBarHeight);
    butExtrude->align(butSetting,LV_ALIGN_OUT_LEFT_TOP,-spacing,0);
    butExtrude->setAction([](struct _lv_obj_t *)->lv_res_t
                          {
                              MarlinUi::getInstance()->showExtrudeScreen();
                              return LV_RES_OK;
                          },LV_BTN_ACTION_CLICK);
    LVImage * imgExtrude = new LVImage(butExtrude,nullptr);
    imgExtrude->setSrc(&extrude_24);
    imgExtrude->setStyle(THEME_STYLE_GET(Theme_screen_Blue));


    //功能区域/////////////////////////////////////////////////////////

    LVObject * butArea = new LVObject(this,nullptr);
    butArea->setSize(screenWidth,screenHeight - titleBarHeight - stateBarHeight);
    butArea->align(LV_ALIGN_IN_TOP_LEFT,0,titleBarHeight);
    butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));

    //温度图表刻度
    styleScale = &lv_style_plain;
    styleScale->text.color = LV_COLOR_WHITE;
    styleScale->text.line_space = scaleSpacing;
    //styleScale->text.font = &lv_font_dejavu_20;

    LVLabel * labScale = new LVLabel(butArea,nullptr);
    labScale->setWidth(24);
    labScale->setStaticText("300\n250\n200\n150\n100\n50\n0\n-50");
    labScale->align(LV_ALIGN_IN_BOTTOM_LEFT,safeMargin,-safeMargin-6);
    labScale->setAlign(LV_LABEL_ALIGN_RIGHT);
    labScale->setStyle(styleScale);

    //温度图表
    styleChart = &lv_style_plain;
    styleChart->body.shadow.width = 0;
    styleChart->body.main_color = LV_COLOR_HEX(0x1c2025);
    styleChart->body.grad_color = LV_COLOR_HEX(0x1c2025);
    styleChart->line.color = LV_COLOR_HEX(0x111417);

    LVChart * chartTemp = new LVChart(butArea,nullptr);
    chartTemp->setSize(chartWidth,chartHeight);
    chartTemp->align(LV_ALIGN_IN_BOTTOM_LEFT,safeMargin + labScale->getWidth() + safeMargin, -safeMargin-3);//
    //同时绘制点线背景格
    chartTemp->setType(LV_CHART_TYPE_LINE); //||LV_CHART_TYPE_COLUMN //LV_CHART_TYPE_POINT
    chartTemp->setRange(-50,300); //TODO: 图表范围由marlin中的宏设定为准
    chartTemp->setPointCount(CHARTPOINTCOUNT); //NOTE: 图表上显示20个温度数据,按照2秒一个,显示1分钟内的温度变化情况
    chartTemp->setDivisionLineCount(6,0);
    chartTemp->setSeriesWidth(2);
    chartTemp->setSeriesOpacity(LV_OPA_60);
    chartTemp->setStyle(styleChart);

    //实时温度线
    m_serialBed = chartTemp->addSeries(THEME_STYLE_GET(Theme_screen_Blue)->image.color);
    m_serialExtrude0 = chartTemp->addSeries(THEME_STYLE_GET(Theme_screen_Green)->image.color);
    //LVChartSeries * serialExtrude1 = chartTemp->addSeries(LV_COLOR_GREEN);
    //LVChartSeries * serialExtrude2 = chartTemp->addSeries(LV_COLOR_GREEN);

    //目标温度线
    m_serialBedTarget = chartTemp->addSeries(THEME_STYLE_GET(Theme_screen_Yellow)->image.color);
    m_serialExtrude0Target = chartTemp->addSeries(THEME_STYLE_GET(Theme_screen_Red)->image.color);
    //LVChartSeries * serialExtrude1Target = chartTemp->addSeries(LV_COLOR_RED);
    //LVChartSeries * serialExtrude2Target = chartTemp->addSeries(LV_COLOR_RED);

    //chartTemp->clearSerie(serialBed); //清除线上的数据


    //初始化温度查询任务
    m_tempQueryTask = new ScreenTask(this,1000);//任务启停由屏幕显隐控制
    m_tempQueryTask->setTaskFunc([=](){
        //更新热床/喷嘴温度

        int16_t bedValue = ExtUI::getTargetTemp_celsius(ExtUI::BED);
        chartTemp->setNext(m_serialBedTarget,bedValue<=0?LV_CHART_POINT_DEF:bedValue);
        int16_t extrude0Value = ExtUI::getTargetTemp_celsius(ExtUI::E0);
        chartTemp->setNext(m_serialExtrude0Target,extrude0Value<=0?LV_CHART_POINT_DEF:extrude0Value);

        //更新热床/喷嘴温度
        bedValue = ExtUI::getActualTemp_celsius(ExtUI::BED);
        chartTemp->setNext(m_serialBed,bedValue);
        labHeatBed()->setText(itos(bedValue));

        extrude0Value = ExtUI::getActualTemp_celsius(ExtUI::E0);
        chartTemp->setNext(m_serialExtrude0,extrude0Value);
        labHeatEnd()->setText(itos(extrude0Value));

        //chartTemp->setNext(serialExtrude1,ExtUI::getActualTemp_celsius(ExtUI::H1));
        //chartTemp->setNext(serialExtrude2,ExtUI::getActualTemp_celsius(ExtUI::H2));
    });

    //PLA ABS 温度预设按钮
    LVButton * butPla = new LVButton(butArea,nullptr);
    butPla->setSize(butWidth,butHeight/2);
    butPla->align(chartTemp,LV_ALIGN_OUT_TOP_MID,-butPla->getWidth()/2-safeMargin,-safeMargin/2);
    butPla->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
    butPla->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butPla->setAction([this](struct _lv_obj_t * obj)->lv_res_t{
        //TODO:设置预设PLA温度,按照设置
        setHeatEndValue(200);
        setHeatBedValue(55);
    },LV_BTN_ACTION_CLICK);
    LVLabel * labPla = new LVLabel(butPla,nullptr);
    labPla->setStaticText("PLA");

    LVButton * butAbs = new LVButton(butArea,nullptr);
    butAbs->setSize(butWidth,butHeight/2);
    butAbs->align(chartTemp,LV_ALIGN_OUT_TOP_MID,butAbs->getWidth()/2+safeMargin,-safeMargin/2);
    butAbs->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
    butAbs->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butAbs->setAction([this](struct _lv_obj_t * obj)->lv_res_t{
        //TODO:设置预设ABS温度,按照预设
        setHeatEndValue(250);
        setHeatBedValue(110);
    },LV_BTN_ACTION_CLICK);
    LVLabel * labAbs = new LVLabel(butAbs,nullptr);
    labAbs->setText("ABS");


    //实际喷头温度
    LVObject * iconHeatEnd = new LVObject(butArea,nullptr);
    iconHeatEnd->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));
    iconHeatEnd->setSize(heatend_20_12.header.w+16,heatend_20_12.header.h+26);
    iconHeatEnd->align(chartTemp,LV_ALIGN_OUT_RIGHT_MID,safeMargin,-iconHeatEnd->getHeight());
    LVImage * imgHeatEnd = new LVImage(iconHeatEnd,nullptr);
    imgHeatEnd->setSrc(&heatend_20_12);
    imgHeatEnd->align(LV_ALIGN_IN_TOP_MID);
    imgHeatEnd->setStyle(THEME_STYLE_GET(Theme_screen_Green));
    labHeatEnd()->setParent(iconHeatEnd);
    labHeatEnd()->align(imgHeatEnd,LV_ALIGN_OUT_BOTTOM_MID,0,2);
    labHeatEnd()->setValue((int16_t)ExtUI::getActualTemp_celsius(ExtUI::E0));
    labHeatEnd()->setStyle(THEME_STYLE_GET(Theme_screen_Green));

    //实际喷头温度
    LVObject * iconHeatBed = new LVObject(butArea,nullptr);
    iconHeatBed->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));
    iconHeatBed->setSize(heatbed_20_12.header.w+16,heatbed_20_12.header.h+26);
    iconHeatBed->align(chartTemp,LV_ALIGN_OUT_RIGHT_MID,safeMargin,iconHeatBed->getHeight());
    LVImage * imgHeatBed = new LVImage(iconHeatBed,nullptr);
    imgHeatBed->setSrc(&heatbed_20_12);
    imgHeatBed->align(LV_ALIGN_IN_TOP_MID);
    imgHeatBed->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
    labHeatBed()->setParent(iconHeatBed);
    labHeatBed()->align(imgHeatBed,LV_ALIGN_OUT_BOTTOM_MID,0,2);
    labHeatBed()->setValue((int16_t)ExtUI::getActualTemp_celsius(ExtUI::BED));
    labHeatBed()->setStyle(THEME_STYLE_GET(Theme_screen_Blue));


    //TODO: 温度设定值添加特效
    //温度值文本样式
    //static LVStyle styleHeatValue(THEME_STYLE_GET(Theme_Screen_ButtonRel));
    //styleHeatValue->body.opa = 0;
    //styleHeatValue->body.border.width = 4;
    //styleHeatValue->body.border.part = LV_BORDER_BOTTOM;
    //styleHeatValue->body.border.color = LV_COLOR_BLUE;

    //喷头温度开关
    butHeatEndSwitch()->setParent(butArea);
    butHeatEndSwitch()->setSize(butWidth,butHeight);
    butHeatEndSwitch()->setToggle(true);
    butHeatEndSwitch()->setChecked(m_heatEndSwitch);
    styleHeatEndSwitch = THEME_STYLE_GET(Theme_screen_Red);
    styleHeatEndSwitch->image.color = LV_COLOR_WHITE;
    butHeatEndSwitch()->setStyle(LV_BTN_STATE_PR,styleHeatEndSwitch);
    butHeatEndSwitch()->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butHeatEndSwitch()->setStyle(LV_BTN_STATE_TGL_REL,styleHeatEndSwitch);
    butHeatEndSwitch()->setStyle(LV_BTN_STATE_TGL_PR,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butHeatEndSwitch()->align(LV_ALIGN_IN_TOP_RIGHT,-safeMargin,spacing*2);
    butHeatEndSwitch()->setAction([this](struct _lv_obj_t * obj)->lv_res_t
    {
        LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
        if(but) setHeatEndSwitch(but->checked());
        return LV_RES_OK;
    },LV_BTN_ACTION_CLICK);
    //TODO: 继续实现加热开关

    LVImage * imgHeatEndSwitch = new LVImage(m_butHeatEndSwitch,nullptr);
    imgHeatEndSwitch->setSrc(&heatend_24);

    LVButton * butHeatEndValue = new LVButton(butArea,nullptr);
    butHeatEndValue->setSize(butWidth,butHeight/2);
    butHeatEndValue->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
    butHeatEndValue->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butHeatEndValue->align(m_butHeatEndSwitch,LV_ALIGN_OUT_BOTTOM_LEFT,0,2);
    butHeatEndValue->setAction([this](struct _lv_obj_t * obj)->lv_res_t{
        //TODO:设置喷嘴温度

        //setClearAfterHide(false);
        NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
        numImput->setTitle("设置喷嘴温度");
        //TODO:从配置中得到温度限制
        numImput->setMin(0);
        numImput->setMax(275);
        numImput->setValue(m_heatEndValue);
        numImput->show();

        //设置回调函数
        numImput->setNumInputFunc([](float value,const bool ok)
        {
            TemperatureScreen * tempScreen = MarlinUi::getInstance()->temperatureScreen();
            //tempScreen->setClearAfterHide(true);
            if(ok)
            {
                tempScreen->setHeatEndValue(value);
            }
            tempScreen->show();
        });

    },LV_BTN_ACTION_CLICK);
    labHeatEndValue()->setParent(butHeatEndValue);
    //labHeatEndValue->SetBodyDraw(true);
    //labHeatEndValue->setStyle(styleHeatValue);
    labHeatEndValue()->setValue(m_heatEndValue);
    reference = butHeatEndValue;

    //热床温度开关
    butHeatBedSwitch()->setParent(butArea);
    butHeatBedSwitch()->setSize(butWidth,butHeight);
    butHeatBedSwitch()->setToggle(true);
    butHeatBedSwitch()->setChecked(m_heatBedSwitch);
    styleHeatBedSwitch = THEME_STYLE_GET(Theme_screen_Yellow);
    styleHeatBedSwitch->image.color = LV_COLOR_WHITE;
    butHeatBedSwitch()->setStyle(LV_BTN_STATE_PR,styleHeatBedSwitch);
    butHeatBedSwitch()->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butHeatBedSwitch()->setStyle(LV_BTN_STATE_TGL_REL,styleHeatBedSwitch);
    butHeatBedSwitch()->setStyle(LV_BTN_STATE_TGL_PR,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butHeatBedSwitch()->align(reference,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2);
    butHeatBedSwitch()->setAction([this](struct _lv_obj_t * obj)->lv_res_t
    {
        LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
        if(but) setHeatBedSwitch(but->checked());
        return LV_RES_OK;
    },LV_BTN_ACTION_CLICK);
    LVImage * imgHeatBedSwitch = new LVImage(m_butHeatBedSwitch,nullptr);
    imgHeatBedSwitch->setSrc(&heatbed_24);

    LVButton * butHeatBedValue = new LVButton(butArea,nullptr);
    butHeatBedValue->setSize(butWidth,butHeight/2);
    butHeatBedValue->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
    butHeatBedValue->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
    butHeatBedValue->align(m_butHeatBedSwitch,LV_ALIGN_OUT_BOTTOM_LEFT,0,2);
    butHeatBedValue->setAction([this](struct _lv_obj_t * obj)->lv_res_t{
        //TODO:设置喷嘴温度

        //setClearAfterHide(false);
        NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
        numImput->setTitle("设置热床温度");
        //TODO:从配置中得到温度限制
        numImput->setMin(0);
        numImput->setMax(120);
        numImput->setValue(m_heatBedValue);
        numImput->show();

        //设置回调函数
        numImput->setNumInputFunc([](float value,const bool ok)
        {
            TemperatureScreen * tempScreen = MarlinUi::getInstance()->temperatureScreen();
            //tempScreen->setClearAfterHide(true);
            if(ok)
            {
                tempScreen->setHeatBedValue(value);
            }
            tempScreen->show();
        });

    },LV_BTN_ACTION_CLICK);
    labHeatBedValue()->setParent(butHeatBedValue);
    //labHeatBedValue->setStyle(styleHeatValue);
    labHeatBedValue()->setValue(m_heatBedValue);


    //状态栏/////////////////////////////////////////////////
    LVObject * stateBar = new LVObject(this,nullptr);
    stateBar->setSize(screenWidth,stateBarHeight);
    stateBar->align(LV_ALIGN_IN_BOTTOM_LEFT,0,0);
    //状态栏样式
    stateBar->setStyle(THEME_STYLE_GET(Theme_Screen_StateBar));

    //状态栏信息
    LVLabel * stateMesg = new LVLabel(stateBar,nullptr);
    stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
    stateMesg->setText(Lang_TemperatureScreen_MesgHit); //"Preheate"
    stateMesg->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);

    LV_LOG_INFO(__FUNCTION__);

    return true;
}

void TemperatureScreen::onLangChanged()
{

}

void TemperatureScreen::onThemeChanged()
{

}

void TemperatureScreen::beforeCleanScreen()
{
    //保存图标数据
    copySerial(m_serialBed,&m_serialBedBackUp);
    copySerial(m_serialExtrude0,&m_serialExtrude0BackUp);
    copySerial(m_serialBedTarget,&m_serialBedTargetBackUp);
    copySerial(m_serialExtrude0Target,&m_serialExtrude0TargetBackUp);
}


bool TemperatureScreen::beforeShow()
{
    bool wasInites = isInited();

    bool ret = Screen::beforeShow();
    if(ret)
    {
        //恢复图标数据,至少保证数据备份过一次
        if(m_serialBedBackUp.color.full == m_serialBed->color.full && !wasInites)
        {
            copySerial(&m_serialBedBackUp,m_serialBed);
            copySerial(&m_serialExtrude0BackUp,m_serialExtrude0);
            copySerial(&m_serialBedTargetBackUp,m_serialBedTarget);
            copySerial(&m_serialExtrude0TargetBackUp,m_serialExtrude0Target);
        }
    }

    return ret;
}


void TemperatureScreen::afterCleanScreen()
{
    //重置指针指向
    m_labHeatEnd = nullptr; //!< 实际喷嘴温度
    m_labHeatBed = nullptr; //!< 实际热床温度
    m_labHeatEndValue = nullptr; //!< 目标喷嘴温度
    m_labHeatBedValue = nullptr; //!< 目标热床温度

    m_butHeatEndSwitch = nullptr;
    m_butHeatBedSwitch = nullptr;

    m_serialBed = nullptr;
    m_serialExtrude0 = nullptr;
}

void TemperatureScreen::copySerial(LVSeries *res, LVSeries *dest)
{
    dest->color = res->color;
    dest->start_point = res->start_point;
    memcpy(dest->points,res->points,CHARTPOINTCOUNT*sizeof(lv_coord_t));
}
