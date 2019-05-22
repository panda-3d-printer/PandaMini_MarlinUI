#ifndef MOTIONSCREEN_H
#define MOTIONSCREEN_H

#include "screen.h"
#include "motionunit.h"
#include "screentask.h"

#include <lvstyle.hpp>

class LVButton;
class LVLabel;
class LVImage;

/**
 * @brief 运动控制界面
 *
 * TODO: 为单位按钮添加设置功能
 * TODO: 设置运动速度
 *
 */
class MotionScreen : public Screen
{
    LV_OBJECT
protected:
    float m_unit;   //!< 移动单位
    bool m_showQuestionMark = false; //!< 轴位置不清楚时显示 ???
    float m_butUnits[4]; //!< 记录四个按钮的单位
    float m_fastSpeed; //!< 快速度
    float m_slowSpeed; //!< 慢速度

    ScreenTask * m_positionQueryTask = nullptr; //!< 位置查询任务

    LVButton * m_butHomeX = nullptr;
    LVButton * m_butHomeY = nullptr;
    LVButton * m_butHomeZ = nullptr;
    LVButton * m_butHome = nullptr;

    LVObject * m_selectHit = nullptr;

    LVButton * m_but05mm = nullptr;
    LVButton * m_but1mm = nullptr;
    LVButton * m_but10mm = nullptr;
    LVButton * m_but50mm = nullptr;

    LVLabel * m_lab05mm = nullptr;
    LVLabel * m_lab1mm = nullptr;
    LVLabel * m_lab10mm = nullptr;
    LVLabel * m_lab50mm = nullptr;

    LVImage * m_imgXPosArc = nullptr;
    LVImage * m_imgXNegArc = nullptr;
    LVImage * m_imgYPosArc = nullptr;
    LVImage * m_imgYNegArc = nullptr;
    LVImage * m_imgZPosArc = nullptr;
    LVImage * m_imgZNegArc = nullptr;

    LVButton * m_butXPos = nullptr;
    LVButton * m_butXNeg = nullptr;
    LVButton * m_butYPos = nullptr;
    LVButton * m_butYNeg = nullptr;
    LVButton * m_butZPos = nullptr;
    LVButton * m_butZNeg = nullptr;

    LVLabel * m_labXPos = nullptr;
    LVLabel * m_labYPos = nullptr;
    LVLabel * m_labZPos = nullptr;

    LVStyle styleDisBut;
    LVStyle styleDisButSelect;
    LVStyle styleHit;

//    LVStyle styleArcRed;
//    LVStyle styleArcBlue;
//    LVStyle styleArcCyan;
    LVStyle styleMotoOff;
    LVStyle styleButTransp;

public:
    MotionScreen();
    ~MotionScreen();

    float unit() const;
    void setUnit(float unit);
    void setUnit(MotionUnit mu);

    /**
     * @brief 当单位按钮被点击时
     * @param obj
     * @return
     */
    lv_res_t onButMMClicked (struct _lv_obj_t * obj);

    /**
     * @brief 移动按钮被按下
     * @param obj
     * @return
     */
    lv_res_t onButMoveClicked (struct _lv_obj_t * obj);

    lv_res_t onButHomeClicked (struct _lv_obj_t * obj);

    /**
     * @brief 把单位放置在按钮的自由数中
     * @param but
     * @param unit
     */
    void setButtonUnit(LVButton * but, LVLabel *lab, float unit);

    /**
     * @brief 获取按钮中存储的单位
     * @param but
     * @return
     */
    float getButtonUnit(LVButton * but);

    /**
     * @brief 记录选中的单位
     * @param but
     */
    void storeSelectUnit(LVButton * but);

    /**
     * @brief 恢复选中的单位按钮状态
     */
    void restoreSelectUnit();

    float fastSpeed() const;
    void setFastSpeed(float fastSpeed);

    float slowSpeed() const;
    void setSlowSpeed(float slowSpeed);

protected:
    // Screen interface
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;
    virtual bool initScreen() override;
    virtual bool beforeShow() override;
    virtual void beforeCleanScreen() override;
    virtual void afterCleanScreen() override;
};

#endif // MOTIONSCREEN_H
