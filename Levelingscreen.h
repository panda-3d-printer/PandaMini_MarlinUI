#ifndef CHECKSCREEN_H
#define CHECKSCREEN_H

#include "lvstyle.hpp"
#include "screen.h"

/**
 * @brief 限位开关检查界面
 * 除了检查限位开关,也可以检查断料开关
 * 调平探针等
 *
 */
class LevelingScreen : public Screen
{
public:
    LevelingScreen();

    LVStyle m_activeStyle;
    LVStyle m_inactiveStyle;

    // Screen interface
protected:
//    virtual bool beforeShow() override;
//    virtual void afterHide() override;
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;
    virtual bool initScreen() override;
};

#endif // CHECKSCREEN_H
