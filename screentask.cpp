#include "screentask.h"
#include "screen.h"

ScreenTask::ScreenTask(Screen * screen ,uint32_t period, lv_task_prio_t prio)
    : LVTask(period, prio)
      ,m_runWithScreen(true)
      ,m_stopWithScreen(true)
{
    //关联所属屏幕
    if(screen != nullptr)
        setScreen(screen);
}

ScreenTask::~ScreenTask()
{
    logoutTask();
}

void ScreenTask::setScreen(Screen * screen)
{
    //注册任务到任务列表

    if(m_screen != screen)
    {
        //先注销
        if(m_screen != nullptr && m_nodeId != nullptr)
            m_screen->logoutTask(this);

        //再注册
        if(screen != nullptr)
            screen->loginTask(this);
        m_screen = screen;
    }
}

void ScreenTask::onScreenShow()
{
    if(m_runWithScreen)
    {
        this->start();
        //this->ready();
    }
}

void ScreenTask::onScreenHide()
{
    if(m_stopWithScreen)
        this->stop();
}

void ScreenTask::loginTask()
{
    if(m_screen != nullptr && m_nodeId == nullptr)
        m_screen->loginTask(this);
}

void ScreenTask::logoutTask()
{
    if(m_screen != nullptr && m_nodeId != nullptr)
    {
        m_screen->logoutTask(this);
        m_nodeId = nullptr;
    }
}
