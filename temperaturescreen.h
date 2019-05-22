#ifndef TEMPERATURESCREEN_H
#define TEMPERATURESCREEN_H

#include "screen.h"

#include <lvstyle.hpp>
#include <lvchart.hpp>

//图标点数目
#define CHARTPOINTCOUNT 60

class LVLabel;
class LVButton;


/**
 * @brief 温度控制界面
 * 最好的情况是屏幕切换任然保留图标数据
 */
class TemperatureScreen : public Screen
{
    LV_OBJECT

protected:
    int16_t m_heatEndValue = 200; //!< 喷嘴温度
    int16_t m_heatBedValue = 55; //!< 热床温度
    bool m_heatEndSwitch = false; //!< 喷嘴加热开关
    bool m_heatBedSwitch = false; //!< 热床加热开关

    ScreenTask * m_tempQueryTask = nullptr; //!< 温度查询任务

    LVLabel * m_labHeatEnd = nullptr; //!< 实际喷嘴温度
    LVLabel * m_labHeatBed = nullptr; //!< 实际热床温度
    LVLabel * m_labHeatEndValue = nullptr; //!< 目标喷嘴温度
    LVLabel * m_labHeatBedValue = nullptr; //!< 目标热床温度

    LVButton * m_butHeatEndSwitch = nullptr; //!< 喷嘴加热开关按钮
    LVButton * m_butHeatBedSwitch = nullptr; //!< 热床加热开关按钮

    LVSeries * m_serialBed = nullptr;
    LVSeries * m_serialExtrude0 = nullptr;
    LVSeries * m_serialBedTarget = nullptr;
    LVSeries * m_serialExtrude0Target = nullptr;

    LVStyle styleScale;
    LVStyle styleChart;

    LVStyle styleHeatEndSwitch;
    LVStyle styleHeatBedSwitch;

    //备份图标数据
    lv_coord_t m_serialBedPoints[CHARTPOINTCOUNT];
    lv_coord_t m_serialBedTargetPoints[CHARTPOINTCOUNT];
    lv_coord_t m_serialExtrude0Points[CHARTPOINTCOUNT];
    lv_coord_t m_serialExtrude0TartgetPoints[CHARTPOINTCOUNT];

    LVSeries m_serialBedBackUp;
    LVSeries m_serialBedTargetBackUp;
    LVSeries m_serialExtrude0BackUp;
    LVSeries m_serialExtrude0TargetBackUp;

public:
    TemperatureScreen();
    ~TemperatureScreen();
    LVLabel *labHeatEnd();
    LVLabel *labHeatBed();
    LVLabel *labHeatEndValue();
    LVLabel *labHeatBedValue();

    void setHeatEndValue(int16_t value);
    void setHeatBedValue(int16_t value);

    void setHeatEndSwitch(bool value);
    void setHeatBedSwitch(bool value);

    LVButton *butHeatEndSwitch();
    LVButton *butHeatBedSwitch();

protected:
    virtual void onLangChanged() override;
    virtual bool initScreen() override;
    virtual void onThemeChanged() override;
    virtual void beforeCleanScreen() override;
    virtual bool beforeShow() override;
    virtual void afterCleanScreen() override;

    /**
     * @brief 备份图标线段
     * @param res
     * @param dest
     */
    void copySerial(LVSeries * res,LVSeries * dest);
};

#endif // TEMPERATURESCREEN_H
