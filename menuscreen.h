#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include "screen.h"

class LVLabel;

/**
 * @brief MenuScreen
 */
class MenuScreen : public Screen
{
    LV_OBJECT
protected:
    LVLabel * labTitle = nullptr;
    LVLabel * labGcode = nullptr;
    LVLabel * labLeveling = nullptr;
    LVLabel * labCustom = nullptr;
    LVLabel * labSetting = nullptr;
    LVLabel * labCheck = nullptr;
    LVLabel * labConnect = nullptr;
    LVLabel * labShutdown = nullptr;
    LVLabel * labFirmware = nullptr;
    LVLabel * labAbout = nullptr;
public:
    MenuScreen();

    /**
     * @brief 初始化屏幕
     */
    bool initScreen() override;

    // Screen interface
public:
    virtual void onLangChanged() override;
};



#endif // MENUSCREEN_H
