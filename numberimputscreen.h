#ifndef NUMBERIMPUTSCREEN_H
#define NUMBERIMPUTSCREEN_H

#undef min
#undef max
#include <functional>
#include "screen.h"

class LVObject;
class LVButton;
class LVLabel;

using NumInputFunc = std::function<void(float value,const bool ok)>;

//定义数字字符串最大长度
#define NUM_STR_LEN 12

/**
 * @brief 数字输入界面
 * TODO: 考虑屏幕被强占的情况,此时不应该执行回调函数
 */
class NumberImputScreen : public Screen
{
    LV_OBJECT
protected:
    float m_value = INT32_MAX; //!< 输入值
    float m_min = 0; //!< 最大值
    float m_max = 100; //!< 最小值

    char m_valueStr[NUM_STR_LEN] = "";

    NumInputFunc m_numInputFunc; //!<数字输入后的回调函数
    bool m_ok; //!< 数字输入状态 true:确认输入 false:取消输入
    bool m_hideByUser = false; //!< 用户造成的界面隐藏

    LVLabel* m_labTitle = nullptr;
    LVLabel* m_labValue = nullptr;
    LVLabel* m_labMin = nullptr;
    LVLabel* m_labMax = nullptr;

    LVObject * m_butArea = nullptr;

public:
    NumberImputScreen();

    void setValue(const float &value);
    float value();

    /**
     * @brief 数值退格
     */
    void backSpaceValue();
    void appendValue(char a);

    float getMin() const;
    void setMin(const float &min);

    float getMax() const;
    void setMax(const float &max);

    LVLabel * labTitle();
    void setTitle(char * title);
    void setTitle(uint16_t titleId);

    LVLabel * labValue();
    LVLabel * labMin();
    LVLabel * labMax();

    NumInputFunc numInputFunc() const;
    void setNumInputFunc(const NumInputFunc &numInputFunc);

    /**
     * @brief 重置数字输入
     */
    void resetNumInput();

    // Screen interface
    bool ok() const;
    void setOk(bool ok);

    bool hideByUser() const;
    void setHideByUser(bool hideByUser);

protected:
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;
    virtual bool initScreen() override;
    virtual bool beforeShow() override;
    virtual void afterHide() override;
    virtual void afterCleanScreen() override;
};

#endif // NUMBERIMPUTSCREEN_H
