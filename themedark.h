#ifndef THEMEDARK_H
#define THEMEDARK_H

#include "theme.h"

class ThemeDark : public Theme
{
    LV_MEMAORY_FUNC
protected:
    ThemeDark():Theme(Theme_Dark)
    {}
public:

    /**
     * @brief 初始化黑色主题
     */
    static void initTheme();

    // Theme interface
public:
    virtual void setupTheme() override;
};

#endif // THEMEDARK_H
