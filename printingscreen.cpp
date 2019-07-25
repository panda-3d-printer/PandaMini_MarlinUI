#include "printingscreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"
#include "numberimputscreen.h"

using std::placeholders::_1;

LVLabel *PrintingScreen::labFile()
{
    if(m_labFile == nullptr)
    {
        m_labFile = new LVLabel(this,nullptr);
    }
    return m_labFile;
}

void PrintingScreen::setPrintFileName(const char *name)
{
    if(name != m_printFileName)
        strcpy(m_printFileName,name);
    if(isInited())
        labFile()->setStaticText(m_printFileName);
}

void PrintingScreen::setPrintintProcess(uint16_t value)
{
    static uint8_t count = 0;
    static bool blink = true;

    char tempStr[6];

    if(value > 100)
        LV_LOG_ERROR("Printint Process > 100, Error!");

    if(isInited())
    {
        if(m_printingState == PS_Paused && ExtUI::isPrinting()) //避免暂停时显示进度为0
        {
            if( count > 2 )
            {
                count = 0;
                blink = ! blink;
            }
            m_barProgress->setValue(blink ? value : m_printintProcess);
            sprintf(tempStr,"%d%%",m_printintProcess);
            m_labProcess->setText(tempStr);
            m_labProcess->setHidden(blink);

            //暂停后,时间需要重新预估
            m_lastOneProgressPercentElapsedTime = 0;
            m_lastProgressPercentTime = 0;
        }
        else if(ExtUI::isPrinting())
        {
            m_barProgress->setValue(value);
            sprintf(tempStr,"%d%%",value);
            m_labProcess->setText(tempStr);
            m_labProcess->setHidden(false);
            //记录打印进度
            if(m_printintProcess != value)
            {
                m_printintProcess = value;
                //进度变化时记录消耗的时间,为时间预估做参考
                if(m_lastProgressPercentTime != 0)
                    m_lastOneProgressPercentElapsedTime = ExtUI::getProgress_seconds_elapsed() - m_lastProgressPercentTime;
                m_lastProgressPercentTime = ExtUI::getProgress_seconds_elapsed();
            }
        }
        else
        {
            m_barProgress->setValue(0);
            m_labProcess->setHidden(false);
        }
    }
    count ++;
}

void PrintingScreen::setPrintingElapsedTime(uint32_t value)
{
    uint32_t estimatedTime;
    uint16_t h;
    uint16_t m;
    uint16_t s;

    //估算剩余时间
    estimatedTime = m_lastOneProgressPercentElapsedTime == 0 ? 0 : (100 - m_printintProcess) * m_lastOneProgressPercentElapsedTime - (value - m_lastProgressPercentTime) ;
    //由于开始的时候要加热,时间会比较久,因此这个时间消耗不适合做时间预估
    setPrintingEstimatedTime( m_printintProcess > 1 ? estimatedTime : 0);

    s = value%60;
    value = value/60;

    m = value%60;
    value = value/60;

    h = value;

    char tempStr[16];
    sprintf(tempStr,"%02d:%02d:%02d",h,m,s);
    m_labTimeKeep->setText(tempStr);
}

void PrintingScreen::setPrintingEstimatedTime(uint32_t value)
{

    if(value == 0)
    {
        m_labTimeEstimate->setText("--:--:--");
    }
    else
    {
        uint16_t h = 0;
        uint16_t m = 0;
        uint16_t s = 0;

        s = value%60;
        value = value/60;

        m = value%60;
        value = value/60;

        h = value;

        char tempStr[16];
        sprintf(tempStr,"%02d:%02d:%02d",h,m,s);
        m_labTimeEstimate->setText(tempStr);
    }
}

PrintingScreen::PrintingScreen()
{
    strcpy(m_printFileName,"XXXXXXXXXXXXXXX.GGGG");

    //关联信号槽
    m_slotPrintingEvent.setSlotFunc(std::bind(&PrintingScreen::onPrintingEvent,this,_1));
    connect(&(MarlinUi::getInstance()->getSignalPrintTimerStarted()),&m_slotPrintingEvent);
    connect(&(MarlinUi::getInstance()->getSignalPrintTimerPaused()),&m_slotPrintingEvent);
    connect(&(MarlinUi::getInstance()->getSignalPrintTimerStopped()),&m_slotPrintingEvent);
    connect(&(MarlinUi::getInstance()->getSignalRemotePrinting()),&m_slotPrintingEvent);
}

PrintingScreen::~PrintingScreen()
{
}

bool PrintingScreen::initScreen()
{
    //初始化屏幕

    /*
     * 实现打印界面

    ==============================================
    +++XXXXXXX+++++++++++++++++++++++++++++++&&&&&
    +++++++++++++++++++++++++++++++++++++++++&&&&&
    =                                            =
    =   0000000   1111111   2222222    3333333   =
    =   0000000   1111111   2222222    3333333   =
    =   0000000   1111111   2222222    3333333   =
    =                                            =
    =   4444444   5555555   666666666666666666   =
    =   4444444   5555555   666666666666666666   =
    =   4444444   5555555   666666666666666666   =
    =                                            =
    =+++++++++++++++++---------------------------=
    =+++++++++++++++++---------------------------=
    &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    */

    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 4;
    const int16_t titleBarHeight = 32;
    const int16_t stateBarHeight = 26;

    const int16_t imgYOffset = spacing*3;

    const int16_t labPosWidth = 80;

    const int16_t butSize = 74;
    const int16_t progressHeight = 22;

    const int16_t fontSize = 16;
    const int16_t iconSize = 24;


    //特殊按钮状态样式 颜色区分
    styleButRed = &lv_style_btn_rel;
    styleButRed->body.main_color = LV_COLOR_HEX(0xdf0024);
    styleButRed->body.grad_color = LV_COLOR_HEX(0xdf0024);
    styleButRed->body.border.width = 0;
    styleButRed->body.radius = 0;

    styleButGreen = &lv_style_btn_rel;
    styleButGreen->body.main_color = LV_COLOR_HEX(0x45ff91);
    styleButGreen->body.grad_color = LV_COLOR_HEX(0x45ff91);
    styleButGreen->body.border.width = 0;
    styleButGreen->body.radius = 0;

    styleButBlue = &lv_style_btn_rel;
    styleButBlue->body.main_color = LV_COLOR_HEX(0x2e76fc);
    styleButBlue->body.grad_color = LV_COLOR_HEX(0x2e76fc);
    styleButBlue->body.border.width = 0;
    styleButBlue->body.radius = 0;

    //进度条样式
    styleProcessBar = &lv_style_plain;
    styleProcessBar->body.main_color = LV_COLOR_HEX(0x172845);
    styleProcessBar->body.grad_color = LV_COLOR_HEX(0x172845);
    styleProcessBar->body.radius = 0;
    styleProcessBar->body.border.width = 0;
    styleProcessBar->text.color = LV_COLOR_WHITE;

    styleProcessIndic = &lv_style_plain;
    styleProcessIndic->body.main_color = LV_COLOR_HEX(0x2e77ff);
    styleProcessIndic->body.grad_color = LV_COLOR_HEX(0x2e77ff);
    styleProcessIndic->body.radius = 0;
    styleProcessIndic->body.border.width = 0;
    styleProcessIndic->body.padding.hor = 0;
    styleProcessIndic->body.padding.ver = 0;
    styleProcessIndic->body.padding.inner = 0;
    styleProcessIndic->text.color = LV_COLOR_WHITE;


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
        butBack->setAction(std::bind(&PrintingScreen::onButBackClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_imgBack = new LVImage(butBack,nullptr);
        m_imgBack->setSrc( ExtUI::isPrinting() ? &printing_24 : &back_24);

        //标题
        m_labTitle = new LVLabel(titleBar,nullptr);
        m_labTitle->setText(Lang_PrintingScreen_LabPrinting);
        m_labTitle->align(butBack,LV_ALIGN_OUT_RIGHT_MID,0,0);
        //其他功能按钮

        //暂停 / 恢复
        m_butPause = new LVButton(titleBar,nullptr);
        m_butPause->setSize(titleBarHeight*1.5f,titleBarHeight);
        m_butPause->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
        m_butPause->setStyle(LV_BTN_STATE_TGL_REL,styleButBlue);
        m_butPause->setStyle(LV_BTN_STATE_TGL_PR,styleButGreen);
        m_butPause->setStyle(LV_BTN_STATE_PR,styleButBlue);
        m_butPause->setStyle(LV_BTN_STATE_REL,styleButBlue);
        m_butPause->setLayout(LV_LAYOUT_OFF);
        m_butPause->setToggle(true);//开启开关功能
        m_butPause->setAction(std::bind(&PrintingScreen::onButPauseClicked,this,_1),LV_BTN_ACTION_CLICK);
        //m_butPause->setHidden(true);
        m_imgPause = new LVImage(m_butPause,nullptr);
        m_imgPause->setSrc(&pause_24);
        m_imgPause->align(LV_ALIGN_CENTER);

        //停止
        m_butStop = new LVButton(titleBar,nullptr);
        m_butStop->setSize(titleBarHeight*1.5f,titleBarHeight);
        m_butStop->align(LV_ALIGN_IN_RIGHT_MID,-(safeMargin+titleBarHeight*1.5f+spacing),0);
        m_butStop->setStyle(LV_BTN_STATE_REL,styleButRed);
        m_butStop->setStyle(LV_BTN_STATE_PR,styleButRed);
        m_butStop->setAction(std::bind(&PrintingScreen::onButStopClicked,this,_1),LV_BTN_ACTION_CLICK);
        m_butStop->setHidden(true);
        LVImage * imgStop = new LVImage(m_butStop,nullptr);
        imgStop->setSrc(&stop_24);
        imgStop->align(LV_ALIGN_CENTER);


        //文件名 (滚动于标题和停止按钮之间)
        labFile()->setParent(titleBar);
        labFile()->setLongMode(LV_LABEL_LONG_ROLL); //文件名过长就滚动显示
        labFile()->setStaticText(m_printFileName);
        labFile()->align(m_labTitle,LV_ALIGN_OUT_RIGHT_MID,spacing*2,0);
        labFile()->setAutoRealign(true);
        labFile()->setWidth((m_butStop->getHidden() ? (m_butPause->getHidden()? (screenWidth-safeMargin) : m_butPause->getX()) : m_butStop->getX()) -  m_labFile->getX() - 4);
    }

    {
        //按钮区域
        LVObject * butArea = new LVObject(this,nullptr);
        butArea->setSize(screenWidth,screenHeight-titleBarHeight-stateBarHeight);
        butArea->align(reference,LV_ALIGN_OUT_BOTTOM_LEFT);
        butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));
        reference = butArea;

        //喷嘴温度
        {
            LVButton * butHeatEnd = new LVButton(butArea,nullptr);
            butHeatEnd->setSize(butSize,butSize);
            butHeatEnd->setLayout(LV_LAYOUT_OFF);
            butHeatEnd->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butHeatEnd->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butHeatEnd->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,spacing);
            butHeatEnd->setAction([](lv_obj_t * obj)->lv_res_t {
                //设置喷嘴温度

                NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
                numImput->setTitle("设置喷嘴温度");
                //TODO:从配置中得到温度限制
                numImput->setMin(0);
                numImput->setMax(275);
                numImput->setValue(ExtUI::getTargetTemp_celsius(ExtUI::E0));
                numImput->show();

                //设置回调函数
                numImput->setNumInputFunc([](float value,const bool ok)
                {
                    PrintingScreen * printingScreen = MarlinUi::getInstance()->printingScreen();
                    //tempScreen->setClearAfterHide(true);
                    if(ok)
                    {
                        ExtUI::setTargetTemp_celsius(value, ExtUI::E0);
                    }
                    printingScreen->show();
                });

            },LV_BTN_ACTION_CLICK);
            m_imgHeatEnd = new LVImage(butHeatEnd,nullptr);
            m_imgHeatEnd->setSrc(&heatend_24);
            m_imgHeatEnd->align(LV_ALIGN_CENTER,0,-imgYOffset);
            m_labHeatEnd = new LVLabel(butHeatEnd,nullptr);
            m_labHeatEnd->setText("999/999"); //TODO:减少字间距
            m_labHeatEnd->align(m_imgHeatEnd,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2);
            reference = butHeatEnd;
        }

        //热床温度
        {
            LVButton * butHeatBed = new LVButton(butArea,nullptr);
            butHeatBed->setSize(butSize,butSize);
            butHeatBed->setLayout(LV_LAYOUT_OFF);
            butHeatBed->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butHeatBed->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butHeatBed->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing,0);
            butHeatBed->setAction([](lv_obj_t * obj)->lv_res_t {
                //设置热床温度

                NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
                numImput->setTitle("设置热床温度");
                //TODO:从配置中得到温度限制
                numImput->setMin(0);
                numImput->setMax(120);
                numImput->setValue(ExtUI::getTargetTemp_celsius(ExtUI::BED));
                numImput->show();

                //设置回调函数
                numImput->setNumInputFunc([](float value,const bool ok)
                {
                    PrintingScreen * printingScreen = MarlinUi::getInstance()->printingScreen();
                    //tempScreen->setClearAfterHide(true);
                    if(ok)
                    {
                        ExtUI::setTargetTemp_celsius(value, ExtUI::BED);
                    }
                    printingScreen->show();
                });

            },LV_BTN_ACTION_CLICK);
            m_imgHeatBed = new LVImage(butHeatBed,nullptr);
            m_imgHeatBed->setSrc(&heatbed_24);
            m_imgHeatBed->align(LV_ALIGN_CENTER,0,-imgYOffset);
            m_labHeatBed = new LVLabel(butHeatBed,nullptr);
            m_labHeatBed->setText("999/999"); //TODO:减少字间距
            m_labHeatBed->align(m_imgHeatBed,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2);
            reference = butHeatBed;
        }

        //风扇速度
        {
            LVButton * butFan = new LVButton(butArea,nullptr);
            butFan->setSize(butSize,butSize);
            butFan->setLayout(LV_LAYOUT_OFF);
            butFan->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butFan->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butFan->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing,0);
            butFan->setAction([](lv_obj_t * obj)->lv_res_t {
                //TODO:设置风扇速度

                NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
                numImput->setTitle("设置风扇速度");
                numImput->setMin(0);
                numImput->setMax(100);
                numImput->setValue(ExtUI::getTargetFan_percent(ExtUI::FAN0));
                numImput->show();

                //设置回调函数
                numImput->setNumInputFunc([](float value,const bool ok)
                {
                    PrintingScreen * printingScreen = MarlinUi::getInstance()->printingScreen();
                    //tempScreen->setClearAfterHide(true);
                    if(ok)
                    {
                        ExtUI::setTargetFan_percent(value, ExtUI::FAN0);
                    }
                    printingScreen->show();
                });

            },LV_BTN_ACTION_CLICK);
            m_imgFan = new LVImage(butFan,nullptr);
            m_imgFan->setSrc(&fan_24);
            m_imgFan->align(LV_ALIGN_CENTER,0,-imgYOffset);
            m_labFan = new LVLabel(butFan,nullptr);
            m_labFan->setText("100%"); //TODO:减少字间距
            m_labFan->align(m_imgFan,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2);
            reference = butFan;
        }

        //打印选项
        {
            LVButton * butOption = new LVButton(butArea,nullptr);
            butOption->setSize(butSize,butSize);
            butOption->setLayout(LV_LAYOUT_OFF);
            butOption->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butOption->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butOption->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing,0);
            LVImage * imgOption = new LVImage(butOption,nullptr);
            imgOption->setSrc(&opction_24);
            imgOption->align(LV_ALIGN_CENTER,0,-imgYOffset);

            LVImage * imgSave = new LVImage(butOption,nullptr);
            imgSave->setSrc(&save_16);
            imgSave->align(imgOption,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2); //此处间隙有问题

            LVImage * imgShutdown = new LVImage(butOption,nullptr);
            imgShutdown->setSrc(&shutdown_16);
            imgShutdown->align(imgSave,LV_ALIGN_OUT_RIGHT_MID,spacing,0);
        }

        //速率
        {
            LVButton * butRate = new LVButton(butArea,nullptr);
            butRate->setSize(butSize,butSize);
            butRate->setLayout(LV_LAYOUT_OFF);
            butRate->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butRate->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butRate->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,spacing+butSize+spacing);
            butRate->setAction([](lv_obj_t * obj)->lv_res_t {
                //设置打印速率

                NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
                numImput->setTitle("设置打印速率");
                numImput->setMin(10);
                numImput->setMax(500);
                numImput->setValue(ExtUI::getFeedrate_percent());
                numImput->show();

                //设置回调函数
                numImput->setNumInputFunc([](float value,const bool ok)
                {
                    PrintingScreen * printingScreen = MarlinUi::getInstance()->printingScreen();
                    //tempScreen->setClearAfterHide(true);
                    if(ok)
                    {
                        ExtUI::setFeedrate_percent(value);
                    }
                    printingScreen->show();
                });

            },LV_BTN_ACTION_CLICK);
            LVImage * imgRate = new LVImage(butRate,nullptr);
            imgRate->setSrc(&rate_24);
            imgRate->align(LV_ALIGN_CENTER,0,-imgYOffset);
            m_labRate = new LVLabel(butRate,nullptr);
            m_labRate->setText("100%"); //TODO:减少字间距
            m_labRate->align(imgRate,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2);
            reference = butRate;
        }

        //流量
        {
            LVButton * butFlow = new LVButton(butArea,nullptr);
            butFlow->setSize(butSize,butSize);
            butFlow->setLayout(LV_LAYOUT_OFF);
            butFlow->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butFlow->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butFlow->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing,0);
            //TODO:无法设置打印流量
//            butFlow->setAction([](lv_obj_t * obj)->lv_res_t {
//                //设置打印流量

//                NumberImputScreen * numImput = MarlinUi::getInstance()->numberImputScreen();
//                numImput->setTitle("设置打印流量");
//                numImput->setMin(0);
//                numImput->setMax(100);
//                numImput->setValue(ExtUI::get);
//                numImput->show();

//                //设置回调函数
//                numImput->setNumInputFunc([](float value,const bool ok)
//                {
//                    PrintingScreen * printingScreen = MarlinUi::getInstance()->printingScreen();
//                    //tempScreen->setClearAfterHide(true);
//                    if(ok)
//                    {
//                        ExtUI::setFeedrate_percent(value);
//                    }
//                    printingScreen->show();
//                });

//            },LV_BTN_ACTION_CLICK);
            LVImage * imgFlow = new LVImage(butFlow,nullptr);
            imgFlow->setSrc(&flow_24);
            imgFlow->align(LV_ALIGN_CENTER,0,-imgYOffset);
            m_labFlow = new LVLabel(butFlow,nullptr);
            m_labFlow->setText("100%"); //TODO:减少字间距
            m_labFlow->align(imgFlow,LV_ALIGN_OUT_BOTTOM_MID,0,spacing*2);
            reference = butFlow;
        }

        //时间
        {
            LVButton * butTime = new LVButton(butArea,nullptr);
            butTime->setSize(butSize+spacing+butSize,butSize);
            butTime->setLayout(LV_LAYOUT_OFF);
            butTime->setStyle(LV_INDEV_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            butTime->setStyle(LV_INDEV_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            butTime->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing,0);
            //计时
            LVImage * imgTimeKeep = new LVImage(butTime,nullptr);
            imgTimeKeep->setSrc(&time_16);
            imgTimeKeep->setStyle(THEME_STYLE_GET(Theme_screen_Red));
            imgTimeKeep->align(LV_ALIGN_IN_TOP_LEFT,safeMargin*3,safeMargin*2);
            m_labTimeKeep = new LVLabel(butTime,nullptr);
            m_labTimeKeep->setText("00:00:01"); //TODO:减少字间距
            m_labTimeKeep->align(imgTimeKeep,LV_ALIGN_OUT_RIGHT_MID,spacing,0);

            //估时
            LVImage * imgTimeEstimate = new LVImage(butTime,nullptr);
            imgTimeEstimate->setSrc(&time_16);
            imgTimeEstimate->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
            imgTimeEstimate->align(imgTimeKeep,LV_ALIGN_OUT_BOTTOM_LEFT,0,safeMargin*2);
            m_labTimeEstimate = new LVLabel(butTime,nullptr);
            m_labTimeEstimate->setText("00:00:00"); //TODO:减少字间距
            m_labTimeEstimate->align(imgTimeEstimate,LV_ALIGN_OUT_RIGHT_MID,spacing,0);

            reference = butTime;
        }

        //进度
        {
            m_barProgress = new LVBar(butArea,nullptr);
            m_barProgress->setSize(screenWidth,progressHeight);
            m_barProgress->align(LV_ALIGN_IN_BOTTOM_LEFT);
            m_barProgress->setStyle(LV_BAR_STYLE_BG,styleProcessBar);
            m_barProgress->setStyle(LV_BAR_STYLE_INDIC,styleProcessIndic);
            m_barProgress->setValue(m_printintProcess);
            m_labProcess = new LVLabel(m_barProgress,nullptr);
            m_labProcess->setText("    ");
            m_labProcess->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);

        }
    }

    {
        //状态栏
        LVObject * stateBar = new LVObject(this,nullptr);
        stateBar->setSize(screenWidth,stateBarHeight);
        stateBar->setStyle(THEME_STYLE_GET(Theme_Screen_StateBar));
        stateBar->align(LV_ALIGN_IN_BOTTOM_LEFT);

        //坐标
        //labPosition->setRecolor(true);
        //labPosition->setText("#df0024 X:#999 #45ff91 Y:#999 #2e76fc Z:#99.90"); //添加文字颜色标识

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

        //速度
        m_labSpeed = new LVLabel(stateBar,nullptr);
        m_labSpeed->setText("999 mm/s"); //添加文字颜色标识
        m_labSpeed->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
    }


    m_queryTask = new ScreenTask(this,250);
    m_queryTask->setTaskFunc([this](){

        static uint32_t count = 0;

        char tempStr[10]; //!< 零时字符

        //检查打印状态
        //if(ExtUI::isPrinting() && ExtUI::isPrintingFromMediaPaused())
        //{
        //    if(m_printingState != PS_Paused)
        //        pringingPause();
        //}
        //else
        if(!ExtUI::isPrinting() && m_printintProcess >= 99 && (m_printingState == PS_Printing ||  m_printingState == PS_Finished))
        {
            //判断打印完成
            if(m_printingState != PS_Finished)
                pringingFinish();
        }
        //else if(ExtUI::isPrinting() && ExtUI::isPrintingFromMedia() && !ExtUI::isPrintingFromMediaPaused() )
        //{
        //    if((m_printingState != PS_Printing))
        //        PrintingStart();
        //}
        //else if(ExtUI::isPrinting() && !ExtUI::isPrintingFromMedia() && m_printingState != PS_Paused)
        //{
        //    if(m_printingState != PS_RemotePrinting)
        //        remotePrinting();
        //}
        //else if(!ExtUI::isPrinting() && !isPrintingFinished() && m_printingState != PS_RemotePrinting)
        //{
            //if(m_printingState != PS_Stoped)
        //    printingStop();
        //}


        //查询温度
        sprintf(tempStr,"%3d/%d",(uint16_t)ExtUI::getActualTemp_celsius(ExtUI::E0),(uint16_t)ExtUI::getTargetTemp_celsius(ExtUI::E0));
        m_labHeatEnd->setText(tempStr);

        sprintf(tempStr,"%3d/%d",(uint16_t)ExtUI::getActualTemp_celsius(ExtUI::BED),(uint16_t)ExtUI::getTargetTemp_celsius(ExtUI::BED));
        m_labHeatBed->setText(tempStr);

        //更新图标状态颜色
        if(ExtUI::getTargetTemp_celsius(ExtUI::BED) > 5)
            m_imgHeatBed->setStyle(THEME_STYLE_GET(Theme_screen_Yellow));
        else
            m_imgHeatBed->setStyle(nullptr); //白色

        if(ExtUI::getTargetTemp_celsius(ExtUI::E0) > 5)
            m_imgHeatEnd->setStyle(THEME_STYLE_GET(Theme_screen_Red));
        else
            m_imgHeatEnd->setStyle(nullptr); //白色

        if(ExtUI::getTargetFan_percent(ExtUI::FAN0) > 0)
            m_imgFan->setStyle(THEME_STYLE_GET(Theme_screen_Blue));
        else
            m_imgFan->setStyle(nullptr);

        //查询风扇速度

        sprintf(tempStr,"%3d%%",(uint16_t)ExtUI::getActualFan_percent(ExtUI::FAN0));
        m_labFan->setText(tempStr);

        //查询速率
        sprintf(tempStr,"%3d%%",(uint16_t)ExtUI::getFeedrate_percent());
        m_labRate->setText(tempStr);

        //查询流量
        //TODO:缺少流量数据
        //sprintf(tempStr,"%3d%%",(uint16_t)ExtUI::getFeedrate_percent());
        //m_labRate->setText(tempStr);

        //查询运动轴位置
        if(count == 0 || count%2) //避免闪烁太快
        {
            sprintf(tempStr,"X: %s", ExtUI::isAxisPositionKnown(ExtUI::X) ? ftos(ExtUI::getAxisPosition_mm(ExtUI::X)) : m_showQuestionMark ? "????" :ftos(ExtUI::getAxisPosition_mm(ExtUI::X)) );
            m_labXPos->setText(tempStr);
            sprintf(tempStr,"Y: %s", ExtUI::isAxisPositionKnown(ExtUI::Y) ? ftos(ExtUI::getAxisPosition_mm(ExtUI::Y)) : m_showQuestionMark ? "????" :ftos(ExtUI::getAxisPosition_mm(ExtUI::Y)) );
            m_labYPos->setText(tempStr);
            sprintf(tempStr,"Z: %s", ExtUI::isAxisPositionKnown(ExtUI::Z) ? ftos(ExtUI::getAxisPosition_mm(ExtUI::Z)) : m_showQuestionMark ? "????" :ftos(ExtUI::getAxisPosition_mm(ExtUI::Z)) );
            m_labZPos->setText(tempStr);
            m_showQuestionMark = !m_showQuestionMark;
        }


        //查询运动速度
        sprintf(tempStr,"%s mm/s",ftos(ExtUI::getMinTravelFeedrate_mm_s(),1));
        m_labSpeed->setText(tempStr);

        //更新时间
        setPrintingElapsedTime(isRemotePrinting() ? 0 : isPrintingFinished() ? 0 : ExtUI::getProgress_seconds_elapsed());

        //更新进度
        setPrintintProcess( isRemotePrinting() ? 0 : isPrintingFinished() ? 100 :  ExtUI::getProgress_percent());

        //更新退出按钮提示

        const void * img =  ExtUI::isPrinting() ? &printing_24 : &back_24;
        if(m_imgBack->getSrc() != img)
            m_imgBack->setSrc(img);

        //统计次数
        ++count;

    });
    m_queryTask->startAndRun();

    LV_LOG_INFO(__FUNCTION__);

    return true;

}

void PrintingScreen::onLangChanged()
{
    //CHANGE_LABEL_LANG(labTitle);

    if(m_labFile)
    {
        //文件名标签重布局
        m_labTitle->realign();
        //设置新宽度
        m_labFile->setWidth((m_butStop->getHidden() ? m_butPause->getX() : m_butStop->getX()) -  m_labFile->getX() - 4);
        m_labFile->realign();
    }
}


lv_res_t PrintingScreen::onButPauseClicked(struct _lv_obj_t * obj)
{
    LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
    if(but)
    {
        if(but->checked())
        {
            pringingPause();
        }
        else
        {
            printingResume();
        }
    }

    return LV_RES_OK;
}


/*Called when a button is clicked*/
static lv_res_t mbox_apply_action(lv_obj_t * mbox, const char * txt)
{
    printf("Mbox button: %s\n", txt);
    lv_obj_set_hidden(mbox,true);

    return LV_RES_OK; /*Return OK if the message box is not deleted*/
}

void PrintingScreen::printMesgFunc(uint32_t retId)
{
    //打印消息界面回调函数
    show();

    switch (retId)
    {
    case MR_Back:
    case MR_Cancel:
        //返回打印界面
        break;
    case MR_Comfirm:
        //进入打印界面
        printingStop();
    default:
        break;
    }

    setClearAfterHide(true);
}

lv_res_t PrintingScreen::onButStopClicked(_lv_obj_t *obj)
{
    LVButton * but = static_cast<LVButton*>(lv_obj_get_free_ptr(obj));
    if(but)
    {
        //TODO:准备提示界面
        setClearAfterHide(false);

        MessageScreen * mesgScreen = MarlinUi::getInstance()->messageScreen();
        mesgScreen->setTitle("Stop Printing");
        mesgScreen->labMesg()->setAlign(LV_LABEL_ALIGN_CENTER);
        mesgScreen->setMessage("Confirm to stop printing?");
        mesgScreen->setButton0(Lang_MesgScreen_ButConfirm,MR_Comfirm);
        mesgScreen->setButton1(Lang_MesgScreen_ButCancel,MR_Cancel);
        mesgScreen->setMessageFunc(std::bind(&PrintingScreen::printMesgFunc,this,_1));
        MarlinUi::getInstance()->showMessageScreen();

    }
    return LV_RES_OK;
}

lv_res_t PrintingScreen::onButBackClicked(_lv_obj_t *obj)
{
    LV_LOG_INFO("onButItemClicked");

    LVButton * but = static_cast<LVButton *>(lv_obj_get_free_ptr(obj));

    if(but)
    {
        if(ExtUI::isPrinting())
        {
            //啥也不做
        }
        else
        {
            resetPrintingState();
            return onButBackHomeClicked(obj);
        }
    }

    return LV_RES_OK;
}

void PrintingScreen::PrintingStart()
{
    if(ExtUI::isPrinting() || m_printingState == PS_Finished)
    {
        //ExtUI::printFile(xxxx) //在别处执行
        if(m_printingState != PS_Printing)
        {
            m_printingState = PS_Printing;
            showBubble("Printing start");
        }
    }
    else
    {
        //错误的状态
        m_printingState = PS_Stoped;
    }

    updatePrintCtrlButton();
}

void PrintingScreen::printingStop()
{
    if(ExtUI::isPrinting())
    {
        ExtUI::stopPrint();
        m_printingState = PS_Stoped;
        showBubble("Printing stop");
    }
    else
    {
        //错误的状态
        m_printingState = PS_Stoped;
    }

    updatePrintCtrlButton();
}

void PrintingScreen::pringingPause()
{
    if(ExtUI::isPrinting())
    {
        ExtUI::pausePrint();
        m_printingState = PS_Paused;
        showBubble("Printing pause");
    }
    else
    {
        //错误的状态
        m_printingState = PS_Stoped;
    }

    updatePrintCtrlButton();
}

void PrintingScreen::printingResume()
{
    if(ExtUI::isPrinting())
    {
        ExtUI::resumePrint();
        m_printingState = PS_Printing;
        showBubble("Printing resume");
    }
    else
    {
        //错误的状态
        m_printingState = PS_Stoped;
    }

    updatePrintCtrlButton();
}

void PrintingScreen::pringingFinish()
{
    m_printingState = PS_Finished;
    showBubble("Printing Finish");
    updatePrintCtrlButton();
}

void PrintingScreen::remotePrinting()
{
    m_printingState = PS_RemotePrinting;
    strcpy(m_printFileName,"Remote Printing");
    showBubble("Remote Printing");
    updatePrintCtrlButton();
}

bool PrintingScreen::isRemotePrinting()
{
    return m_printingState == PS_RemotePrinting;
}

bool PrintingScreen::isPrintingFinished()
{
    return m_printingState == PS_Finished;
}

void PrintingScreen::resetPrintingState()
{
    m_printintProcess = 0;
    m_printingState = PS_Finished;
    m_lastOneProgressPercentElapsedTime = 0;
    m_lastProgressPercentTime = 0;

    strcpy(m_printFileName,"");

    //开启远程打印检测
    MarlinUi::getInstance()->getRemotePrintingTask()->start();
}

void PrintingScreen::updatePrintCtrlButton()
{
    if(!isInited()) return;

    switch (m_printingState)
    {

    case PS_Paused:
        m_butPause->setChecked(true);
        m_imgPause->setSrc(&resume_24);
        m_butStop->setHidden(false);
        m_butPause->setStyle(LV_BTN_STATE_TGL_REL,(styleButGreen));
        m_butPause->setStyle(LV_BTN_STATE_TGL_PR,(styleButGreen));
        m_butPause->setStyle(LV_BTN_STATE_REL,(styleButGreen));
        m_butPause->setStyle(LV_BTN_STATE_PR,(styleButGreen));
        m_labTitle->setText(Lang_PrintingScreen_LabPauseed);
        break;

    case PS_Printing:
        m_butPause->setChecked(false);
        m_imgPause->setSrc(&pause_24);
        m_butStop->setHidden(true);
        m_butPause->setStyle(LV_BTN_STATE_TGL_REL,(styleButBlue));
        m_butPause->setStyle(LV_BTN_STATE_TGL_PR,(styleButBlue));
        m_butPause->setStyle(LV_BTN_STATE_REL,(styleButBlue));
        m_butPause->setStyle(LV_BTN_STATE_PR,(styleButBlue));
        m_labTitle->setText(Lang_PrintingScreen_LabPrinting);
        break;

    case PS_Stoped:
        m_butPause->setChecked(false);
        m_butPause->setHidden(true);
        m_butStop->setHidden(true);
        m_labTitle->setText(Lang_PrintingScreen_LabStoped);
        break;

    case PS_Finished:
        m_butPause->setChecked(false);
        m_butPause->setHidden(true);
        m_butStop->setHidden(true);
        m_labTitle->setText(Lang_PrintingScreen_LabFinished);
        break;

    case PS_RemotePrinting:
        m_butPause->setChecked(false);
        m_butPause->setHidden(true);
        m_butStop->setHidden(true);
        m_labTitle->setText(Lang_PrintingScreen_LabPrinting);
        break;
    }

    //重布局一些对象
    m_labTitle->realign();
    m_labFile->setWidth((m_butStop->getHidden() ? (m_butPause->getHidden()? (width()- 6) : m_butPause->getX()) : m_butStop->getX()) -  m_labFile->getX() - 4);
    m_labFile->realign();

    //刷新返回按钮
    m_imgBack->setSrc( ExtUI::isPrinting() ? &printing_24 : &back_24);

}


void PrintingScreen::beforeCleanScreen()
{

}

void PrintingScreen::afterCleanScreen()
{
    m_labFile = nullptr;
}


bool PrintingScreen::beforeShow()
{
    bool ret = Screen::beforeShow();
    if(ret)
    {
        if(!ExtUI::isPrinting())
        {
            showBubble("Not Printing");
        }

        //停止远程打印检测,由打印界面自己来检测
        MarlinUi::getInstance()->getRemotePrintingTask()->stop();
    }
    return ret;
}

void PrintingScreen::onPrintingStarted(LVSignal *signal)
{
    //首先显示打印界面
    if(!isVisible())
        show();
    //显示打印控制按钮
    PrintingStart();
}

void PrintingScreen::onPrintingPaused(LVSignal *signal)
{

    //首先显示打印界面
    if(!isVisible())
        show();
    //显示打印控制按钮
    pringingPause();
}

void PrintingScreen::onPrintingStopped(LVSignal *signal)
{
    //首先显示打印界面
    if(!isVisible())
        show();
    printingStop();
}

void PrintingScreen::onRemotePrinting(LVSignal *signal)
{
    if(!isVisible())
        show();
    remotePrinting();
}

void PrintingScreen::onPrintingEvent(LVSignal *signal)
{
    if(signal == &MarlinUi::getInstance()->getSignalPrintTimerStarted() )
        onPrintingStarted(signal);
    else if(signal == &MarlinUi::getInstance()->getSignalPrintTimerPaused())
        onPrintingPaused(signal);
    else if(signal == &MarlinUi::getInstance()->getSignalPrintTimerStopped())
        onPrintingStopped(signal);
    else if( signal == &MarlinUi::getInstance()->getSignalRemotePrinting())
        onRemotePrinting(signal);
}
