#ifndef CHECKSCREEN_H
#define CHECKSCREEN_H

#include "lvstyle.hpp"
#include "screen.h"

class LVButton;

/**
 * @brief 限位开关检查界面
 * 除了检查限位开关,也可以检查断料开关
 * 调平探针等
 *
 */
class CheckScreen : public Screen
{
	LV_OBJECT
public:
    CheckScreen();

    LVStyle m_activeStyle;
    LVStyle m_inactiveStyle;


protected:

    LVButton * butXmin = nullptr;
    LVLabel * labXminState = nullptr;
    LVButton * butXmax = nullptr;
    LVLabel * labXmaxState = nullptr;
    LVButton * butYmin = nullptr;
    LVLabel * labYminState = nullptr;
    LVButton * butYmax = nullptr;
    LVLabel * labYmaxState = nullptr;
    LVButton * butZmin = nullptr;
    LVLabel * labZminState = nullptr;
    LVButton * butZmax = nullptr;
    LVLabel * labZmaxState = nullptr;

    // Screen interface
//    virtual bool beforeShow() override;
//    virtual void afterHide() override;
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;
    virtual bool initScreen() override;
};

#endif // CHECKSCREEN_H
