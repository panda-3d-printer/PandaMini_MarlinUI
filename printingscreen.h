#ifndef NEWSCREEN_H
#define NEWSCREEN_H

#include "screen.h"

#include <lvstyle.hpp>
#include <lvslot.hpp>

class LVImage;
class LVLabel;
class LVButton;

class PrintingScreen : public Screen
{
    LV_OBJECT

    enum PrintingState : uint8_t
    {
        PS_Printing,
        PS_Paused,
        PS_Stoped,
        PS_Finished,
        PS_RemotePrinting,
    };

protected:
    char m_printFileName[30]; //!< 打印文件名
    bool m_showQuestionMark = false; //!< 轴位置不清楚时显示 ???
    ScreenTask * m_queryTask = nullptr; //!< 位置查询任务
    PrintingState m_printingState = PS_Finished; //!< 打印状态
    uint8_t m_printintProcess = 0; //!< 打印进度
    uint32_t m_lastOneProgressPercentElapsedTime = 0; //!< 最后打印完成1%的耗时 s
    uint32_t m_lastProgressPercentTime = 0; //!< 最后打印进度变化的时间 s

    LVImage * m_imgBack = nullptr;

    LVLabel * m_labTitle = nullptr;
    LVLabel * m_labFile = nullptr;

    LVButton * m_butPause = nullptr;
    LVImage * m_imgPause = nullptr;
    LVButton * m_butStop = nullptr;

    LVImage * m_imgHeatEnd = nullptr;
    LVLabel * m_labHeatEnd = nullptr;
    LVImage * m_imgHeatBed = nullptr;
    LVLabel * m_labHeatBed = nullptr;
    LVImage * m_imgFan = nullptr;
    LVLabel * m_labFan = nullptr;
    LVLabel * m_labRate = nullptr;
    LVLabel * m_labFlow = nullptr;

    LVLabel * m_labTimeKeep = nullptr;
    LVLabel * m_labTimeEstimate = nullptr;

    LVBar * m_barProgress = nullptr;
    LVLabel * m_labProcess = nullptr;

    LVLabel * m_labXPos = nullptr;
    LVLabel * m_labYPos = nullptr;
    LVLabel * m_labZPos = nullptr;

    LVLabel * m_labSpeed = nullptr;

    LVStyle styleButRed;
    LVStyle styleButGreen;
    LVStyle styleButBlue;

    LVStyle styleProcessBar;
    LVStyle styleProcessIndic;

    LVSlot m_slotPrintingEvent; //!< 打印事件槽

public:
    PrintingScreen();
    ~PrintingScreen();

    void printMesgFunc(uint32_t retId);

    lv_res_t onButPauseClicked(struct _lv_obj_t * obj);
    lv_res_t onButStopClicked(struct _lv_obj_t * obj);
    lv_res_t onButBackClicked (struct _lv_obj_t * obj);


    void PrintingStart();
    void printingStop();
    void pringingPause();
    void printingResume();
    void pringingFinish();
    void remotePrinting();

    bool isRemotePrinting();
    bool isPrintingFinished();

    void resetPrintingState();

    void updatePrintCtrlButton();

    LVLabel *labFile();

    /**
     * @brief 设置打印文件名
     * @param name 打印的文件名
     */
    void setPrintFileName(const char * name);

    /**
     * @brief 设置打印进度
     * @param value
     */
    void setPrintintProcess(uint16_t value);

    void setPrintingElapsedTime(uint32_t value);
    void setPrintingEstimatedTime(uint32_t value);


    // Screen interface
protected:
    virtual void onLangChanged() override;
    virtual bool initScreen() override;
    virtual void beforeCleanScreen() override;
    virtual void afterCleanScreen() override;
    virtual bool beforeShow() override;

    /**
     * @brief 处理打印事件
     * @param signal
     */
    void onPrintingEvent(LVSignal *signal);
    void onPrintingStarted(LVSignal * signal);
    void onPrintingPaused(LVSignal * signal);
    void onPrintingStopped(LVSignal * signal);
    void onRemotePrinting(LVSignal * signal);
};

#endif // NEWSCREEN_H
