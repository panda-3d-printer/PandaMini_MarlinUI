#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include "screen.h"
#include "screentask.h"

#include <lvstyle.hpp>

class LVLabel;
class LVStyle;
class LVImage;

class HomeScreen : public Screen
{
    LV_OBJECT
protected:

    LVLabel * labMove = nullptr;
    LVLabel * labPreheat = nullptr;
    LVLabel * labExtrude = nullptr;
    LVLabel * labTool = nullptr;
    LVLabel * labPrint = nullptr;
    LVLabel * stateMesg = nullptr;

    LVLabel * labHeatbed = nullptr;
    LVLabel * labHeatend = nullptr;

    LVImage * imgHeatbed = nullptr;
    LVImage * imgHeatend = nullptr;

    ScreenTask * m_tempQueryTask = nullptr; //!< 温度查询任务

public:
    HomeScreen();

    /**
     * @brief 初始化主屏幕布局
     */
    bool initScreen() override;

    /**
     * @brief 切换界面语言
     */
    virtual void onLangChanged() override;

    /**
     * @brief 切换界面主题
     */
    virtual void onThemeChanged() override;

    virtual void afterCleanScreen() override;
};



#endif // HOMESCREEN_H
