#ifndef SCREENTASK_H
#define SCREENTASK_H

#include <lvtask.hpp>
#include <lvlinklist.hpp>
#include <misc/lvmemory.hpp>

class Screen;

/**
 * @brief 与屏幕密切相关的任务
 * 屏幕显示默认就运行
 * 屏幕隐藏默认就停止
 */
class ScreenTask : public LVTask
{
    LV_MEMAORY_FUNC

    friend Screen;
protected:
    Screen * m_screen = nullptr; //!< 所属的屏幕
    bool m_runWithScreen; //!< 屏幕显示时就运行
    bool m_stopWithScreen;//!< 屏幕隐藏时就停止
    LVLLNode * m_nodeId = nullptr; //!< 在任务列表中的指针
public:
    /**
     * @brief ScreenTask 屏幕任务构造函数
     * @param screen 任务所属的屏幕
     * @param period 任务周期 默认500毫秒
     * @param prio 任务优先级 默认中等优先级
     */
    ScreenTask(Screen * screen ,uint32_t period = 500, lv_task_prio_t prio = LV_TASK_PRIO_LOWEST);

    virtual ~ScreenTask();

    Screen * screen() { return m_screen; }
    void setScreen(Screen * screen);

    void setRunWithScreen(bool value = true) { m_runWithScreen = value; }
    void setStopWithScreen(bool value = true) { m_stopWithScreen = value; }

    bool isRunWithScreen() { return m_runWithScreen; }
    bool isStopWithScreen() { return m_stopWithScreen; }

protected:
    /**
     * @brief 屏幕显示时调用
     */
    virtual void onScreenShow();
    /**
     * @brief 屏幕隐藏时调用
     */
    virtual void onScreenHide();

    /**
     * @brief 注册任务到屏幕的任务列表
     */
    void loginTask();

    /**
     * @brief 从屏幕的任务列表中注销任务
     */
    void logoutTask();
};

//struct ScreenTaskPtrMate
//{
//    ScreenTask * taskPtr;
//    ScreenTask * mate(){ return taskPtr; }
//};
//using ScreenTaskPtr = LVLLNodeMate<ScreenTaskPtrMate>;
using ScreenTaskNode = LVLLNodeMate<ScreenTask>;

#endif // SCREENTASK_H
