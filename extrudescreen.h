#ifndef EXTRUDESCREEN_H
#define EXTRUDESCREEN_H

#include <lvstyle.hpp>
#include <screen.h>


class LVButton;
class LVLabel;
class LVImage;

/**
 * @brief 挤出控制界面
 *
 * TODO:添加冷挤出保护开关
 * TODO: 设置运动速度
 */
class ExtrudeScreen : public Screen
{
    LV_OBJECT

protected:
    float m_unit;   //!< 移动单位
    float m_butUnits[4]; //!< 记录四个按钮的单位
    float m_fastSpeed; //!< 快速度 mm/s
    float m_slowSpeed; //!< 慢速度 mm/s

    ScreenTask * m_queryTask = nullptr; //!< 温度位置查询任务

    LVObject * m_selectHit = nullptr;

    LVButton * m_but05mm = nullptr;
    LVButton * m_but1mm = nullptr;
    LVButton * m_but10mm = nullptr;
    LVButton * m_but50mm = nullptr;

    LVLabel * m_lab05mm = nullptr;
    LVLabel * m_lab1mm = nullptr;
    LVLabel * m_lab10mm = nullptr;
    LVLabel * m_lab50mm = nullptr;

    LVButton * m_butENegF = nullptr;
    LVButton * m_butENegS = nullptr;
    LVButton * m_butEPosF = nullptr;
    LVButton * m_butEPosS = nullptr;

    LVImage * m_imgHeatend = nullptr;
    LVLabel * m_labHeatend = nullptr;
    LVLabel * m_labEPos = nullptr;

    LVStyle styleDisBut;
    LVStyle styleDisButSelect;
    LVStyle styleHit;
    LVStyle styleArcRed;
    LVStyle styleButTransp;

public:
    ExtrudeScreen();

    float getUnit() const;
    void setUnit(float unit);

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
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;
    virtual bool initScreen() override;
    virtual bool beforeShow() override;
    virtual void beforeCleanScreen() override;
    virtual void afterCleanScreen() override;
};

#endif // EXTRUDESCREEN_H
