#include "screen.h"
#include "marlinui.h"
#include "screentask.h"
#include <lvbar.hpp>
#include <lvlabel.hpp>
#include <lvstyle.hpp>
#include <lvmessagebox.hpp>

extern void * _lv_def_scr;

Screen * Screen::s_lastScreen = nullptr;
Screen * Screen::s_currScreen = nullptr;

void memory_monitor(void * param);

/**
 * @brief 处理对象的事件
 */
lv_res_t screenSignalFunc(struct _lv_obj_t * obj, lv_signal_t sign, void * param)
{
    //这里需要依赖free_ptr得到屏幕对象
    //LVObject初始化时将对象指针保存在free_ptr成员中
    Screen * screen = static_cast<Screen *>(obj->free_ptr);
    if(screen)
    {
        lv_res_t ret = screen->defaultSignal(obj,sign,param);
        if(ret)
            return screen->signal(sign,param);
        else
            return ret;
    }
    else
    {
        LV_LOG_WARN("signalFunc() can not get true LVObject type Screen !!");
        return LV_RES_OK;
    }
}


bool Screen::isClearAfterHide() const
{
    return m_clearAfterHide;
}

void Screen::setClearAfterHide(bool clearAfterHide)
{
    m_clearAfterHide = clearAfterHide;
}

void Screen::cleanScreen()
{
    memory_monitor(nullptr);

    //NOTE: 注意清理的先后顺序

    beforeCleanScreen();

    //清理屏幕任务
    clearTaskList();
    //清理子对象
    clean();

    //触发子类清理动作
    afterCleanScreen();

    //重置初始化标识
    setInited(false);

    //整理内存碎片
    lv_mem_defrag();

    LV_LOG_INFO("Screen::cleanScreen()");

    memory_monitor(nullptr);
}

int32_t Screen::getUsedMemorySize()
{
    static lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    return mon.total_size - mon.free_size;
}


LVBar * &Screen::memoryDebuger()
{
    //用进度条指示内存状态
    static LVBar * barMem  = nullptr;
    static LVTask * memTask = nullptr;
    //进度条样式
    static LVStyle styleProcessBar(&lv_style_plain);
    static LVStyle styleProcessIndic(&lv_style_plain);

    if(barMem == nullptr)
    {
        styleProcessBar->body.main_color = LV_COLOR_BLACK;
        styleProcessBar->body.grad_color = LV_COLOR_BLACK;
        styleProcessBar->body.radius = 0;
        styleProcessBar->body.border.width = 0;
        styleProcessBar->text.color = LV_COLOR_WHITE;

        styleProcessIndic->body.main_color = LV_COLOR_GREEN;
        styleProcessIndic->body.grad_color = LV_COLOR_GREEN;
        styleProcessIndic->body.radius = 0;
        styleProcessIndic->body.border.width = 0;
        styleProcessIndic->body.padding.hor = 0;
        styleProcessIndic->body.padding.ver = 0;
        styleProcessIndic->body.padding.inner = 0;
        styleProcessIndic->text.color = LV_COLOR_WHITE;

        barMem = new LVBar((lv_obj_t*)_lv_def_scr,nullptr);
        barMem->setStyle(LV_BAR_STYLE_BG,styleProcessBar);
        barMem->setStyle(LV_BAR_STYLE_INDIC,styleProcessIndic);

        memTask = new LVTask(100);
        memTask->setTaskFunc([&](){
            static lv_mem_monitor_t mon;
            lv_mem_monitor(&mon);
            barMem->setValue(mon.used_pct);

            //切换警示色
            if(mon.used_pct <= 50)
            {
                styleProcessIndic->body.main_color = LV_COLOR_GREEN;
                styleProcessIndic->body.grad_color = LV_COLOR_GREEN;
            }
            else if(mon.used_pct <= 75)
            {
                styleProcessIndic->body.main_color = LV_COLOR_YELLOW;
                styleProcessIndic->body.grad_color = LV_COLOR_YELLOW;
            }
            else if(mon.used_pct <= 85)
            {
                styleProcessIndic->body.main_color = LV_COLOR_ORANGE;
                styleProcessIndic->body.grad_color = LV_COLOR_ORANGE;
            }
            else
            {
                styleProcessIndic->body.main_color = LV_COLOR_RED;
                styleProcessIndic->body.grad_color = LV_COLOR_RED;
            }

        });
        memTask->startAndRun();
    }

    return barMem;
}

void Screen::installMemoryDebuger(Screen *screen)
{
    if(screen)
    {
        LVBar * barMem = memoryDebuger();
        barMem->setParent(screen);
        barMem->setWidth(screen->width());
        barMem->setHeight(2);
        barMem->align(LV_ALIGN_IN_TOP_LEFT);

        //提示内存使用情况,检查内存泄露
        //char tempStr[16];
        //sprintf(tempStr,"Use:%dByte",screen->m_memoryUsed);
        //showBubble(tempStr,1000);
    }
}

void Screen::uninstallMemoryDebuger()
{
    memoryDebuger()->setParent((lv_obj_t*)_lv_def_scr);
}

LVLabel * &Screen::bubble()
{
    static LVLabel * bubble = nullptr;
    static LVStyle styleBubble;

    if(bubble == nullptr)
    {
        //气泡提示
        bubble = new LVLabel((lv_obj_t*)_lv_def_scr,nullptr);
        bubble->setAutoRealign(true);
        bubble->SetBodyDraw(true);
        bubble->setHidden(true); //隐藏,需要时再显示

        styleBubble = &lv_style_plain;
        styleBubble->body.main_color = LV_COLOR_MAKE(0x17,0x28,0x45);
        styleBubble->body.grad_color = LV_COLOR_MAKE(0x17,0x28,0x45);
        styleBubble->text.color = LV_COLOR_WHITE;
        bubble->setStyle(styleBubble);
    }

    return bubble;
}

void Screen::showBubble(const char *mesg, uint32_t period)
{
    //BUG: 消息会闪跳

    static LVTask * bubbleTask = nullptr;
    if(bubbleTask == nullptr)
    {
        bubbleTask = new LVTask(period);
        bubbleTask->setTaskFunc([=](){
            bubble()->setHidden(true);
            bubbleTask->stop();
        });
    }

    bubble()->setAlign(LV_LABEL_ALIGN_CENTER);
    bubble()->setText(mesg);
    bubble()->realign();
    bubble()->setHidden(false);

    //一会自动隐藏
    bubbleTask->start(period);
}

void Screen::showMessageBox(const char *mesg, const char **buts, lv_btnm_action_t action, int32_t period)
{
    LVMessageBox * mbox = messageBox();
    mbox->setText(mesg);
    mbox->addButton(buts,action);
    if(period!=-1)
        mbox->startAutoClose(period);
    else
        mbox->stopAutoClose();

    mbox->align(LV_ALIGN_CENTER);
    mbox->setHidden(false);
}

void Screen::installBubble(Screen *screen)
{
    if(screen)
    {
        bubble()->setParent(screen);
        bubble()->align(LV_ALIGN_IN_TOP_MID,0,8);
    }
}

void Screen::uninstallBubble()
{
    bubble()->setParent((lv_obj_t*)_lv_def_scr);
}

LVMessageBox *&Screen::messageBox()
{
    static LVMessageBox * mbox = nullptr;
    if(mbox == nullptr)
    {
        mbox = new LVMessageBox((lv_obj_t*)_lv_def_scr,nullptr);
        mbox->setHidden(true);
    }
    return mbox;
}

void Screen::installMessageBox(Screen *screen)
{
    messageBox()->setParent(screen);
    messageBox()->align(LV_ALIGN_CENTER);
}

void Screen::uninstallMessageBox()
{
    messageBox()->setParent((lv_obj_t*)_lv_def_scr);
}

void Screen::installPublicComponents(Screen *screen)
{
    installBubble(screen);
    installMemoryDebuger(screen);
    installMessageBox(screen);
}

void Screen::uninstallPublicComponents()
{
    uninstallMessageBox();
    uninstallMemoryDebuger();
    uninstallBubble();
}

Screen::Screen()
    : LVObject()
    ,m_taskList(sizeof (ScreenTask *))
{
    //设置事件处理函数
    setSignalFunc(::screenSignalFunc);

    LV_LOG_INFO("Screen::Screen()");
}

Screen::~Screen()
{
    //清理掉数据和任务
    cleanScreen();

    //清除自己在其他地方的记录
    if(LastScreen() == this)
        setLastScreen(nullptr);
    if(CurrScreen() == this)
        setCurrScreen(nullptr);

    LV_LOG_INFO("Screen::~Screen()");
}

lv_res_t Screen::signal(lv_signal_t sign, void *param)
{
    //处理事件,比如语言发生了变化

    if(sign == LV_SIGNAL_LANG_CHG) onLangChanged();
    else if(sign == LV_SIGNAL_STYLE_CHG) onThemeChanged();

    return LV_RES_OK;
}

void Screen::onLangChanged()
{
    //NOTE: 子类中完成处理语言切换时更新界面文本的操作
}

void Screen::onThemeChanged()
{
    //NOTE: 子类中完成处理主题切换时界面刷新
}

void Screen::beforeCleanScreen()
{
    //NOTE: 子类中完成屏幕清理动作
}

void Screen::afterCleanScreen()
{
    //NOTE: 子类中完成屏幕清理动作
}

bool Screen::initScreen()
{
    LV_LOG_INFO("Screen::initScreen() empty function");
    return false;
}

bool Screen::show()
{
    bool ret = false;

    //如果存在其他的屏幕界面正在显示,需要先隐藏当前的界面
    if(CurrScreen())
        CurrScreen()->hide();

    if(beforeShow())
    {
        screenLoad();
        setCurrScreen(this);
        //开启相关任务
        startScreenTask();
        afterShow();
        ret = true;
    }

    //安装界面的公共元素
    installPublicComponents(this);

    return ret;
}

bool Screen::beforeShow()
{

    //初始化界面
    //将初始化放到这理执行,有助于减少内存占用情况
    //没有用到的界面就不会消耗内存了
    if(!m_inited)
    {

        //统计初始化屏幕用了多少内存
        //方便在屏幕清理的时候发现内存泄露
        memory_monitor(nullptr);
        m_memoryUsed = getUsedMemorySize();
        m_inited = initScreen();
        m_memoryUsed = getUsedMemorySize() - m_memoryUsed;
        memory_monitor(nullptr);

        {
            char str[40];
            //警告内存有泄露
            sprintf(str,"Memory used : %d Bytes!", m_memoryUsed);
            LV_LOG_WARN(str);
        }

        if(!m_inited)
            LV_LOG_INFO("Screen::beforeShow() initScreen false");
    }

    return m_inited;
}

bool Screen::hide()
{
    bool ret = false;
    if(beforeHide())
    {
        //关闭相关任务
        stopScreenTask();
        //if(s_lastScreen != nullptr)
        //    s_lastScreen->show();
        //else if(((Screen*) MarlinUi::getInstance()->homeScreen()) != this )
        //    //BUG:存在主屏幕无法显示的情况
        //    MarlinUi::getInstance()->showHomeScreen();

        setCurrScreen(nullptr);
        setLastScreen(this);

        //卸载界面公共元素,避免被删除
        uninstallPublicComponents();

        //隐藏后清理屏幕
        if(isClearAfterHide())
        {
            int32_t memoryRecovery = getUsedMemorySize();
            cleanScreen();
            memoryRecovery = memoryRecovery - getUsedMemorySize();

            if(m_memoryUsed != memoryRecovery)
            {
                char str[40];
                //警告内存有泄露
                sprintf(str,"Memory leak deteted : %d Bytes!", m_memoryUsed - memoryRecovery);
                LV_LOG_WARN(str);
            }
        }

        afterHide();

        //隐藏后删除屏幕
        if(isDeleteAfterHide())
        {
            setLastScreen(nullptr);
            //延后清理对象,防止对象此时正在被使用
            //deleteLater();

            delete this;
        }

        ret = true;
    }

    return ret;
}

void Screen::backScreen()
{
    if(hasBackScreen())
        s_lastScreen->show();
}

bool Screen::hasBackScreen()
{
    return s_lastScreen != nullptr && s_lastScreen != this;
}

void Screen::preScreen()
{
    if(hasPreScreen())
        m_preScreen->show();
}

bool Screen::hasPreScreen()
{
    return m_preScreen != nullptr;
}

void Screen::setPreScreen(Screen *screen)
{
    m_preScreen = screen;
}

void Screen::nextScreen()
{
    if(hasNextScreen())
        m_nextScreen->show();
}

bool Screen::hasNextScreen()
{
    return m_nextScreen != nullptr;
}

void Screen::setNextScreen(Screen *screen)
{
    m_nextScreen = screen;
}

bool Screen::isVisible()
{
    return lv_scr_act() == m_this;
}

void Screen::setVisible(bool value)
{
    value ? show(): hide();
}

Screen *Screen::LastScreen()
{
    return s_lastScreen;
}

Screen *Screen::CurrScreen()
{
    return s_currScreen;
}

LVColor Screen::screenColor()
{
    return m_screenColor;
}

void Screen::setScreenColor(LVColor color)
{
    m_screenColor = color;

    //TODO: 完成屏幕颜色设置
}

bool Screen::isDeleteAfterHide()
{
    return m_deleteAfterHide;
}

void Screen::setDeleteAfterHide(bool value)
{
    m_deleteAfterHide = true;
}

uint16_t Screen::width()
{
    MarlinUi::getInstance()->width();
}

uint16_t Screen::height()
{
    MarlinUi::getInstance()->height();
}

uint16_t Screen::dpi()
{
    MarlinUi::getInstance()->dpi();
}

void Screen::setLastScreen(Screen *screen)
{
    s_lastScreen = screen;
}

void Screen::setCurrScreen(Screen *screen)
{
    s_currScreen = screen;
}

void Screen::startScreenTask()
{
    LVLLNode * node = m_taskList.head();
    while (node)
    {
        ScreenTaskNode * task = (ScreenTaskNode *)node;
        (*task)->onScreenShow();
        node = m_taskList.next(node);
    }
}

void Screen::stopScreenTask()
{
    LVLLNode * node = m_taskList.head();
    while (node)
    {
        ScreenTaskNode * task = (ScreenTaskNode *)node;
        (*task)->onScreenHide();
        node = m_taskList.next(node);
    }
}

void Screen::loginTask(ScreenTask *task)
{
    if(task && !task->m_nodeId)
    {
        LVLLNode * newNode =  m_taskList.insertTail();

        //保存节点指针
        task->m_nodeId = newNode;

        //保存任务指针
        ScreenTaskNode * taskPtr = (ScreenTaskNode *)newNode;
        taskPtr->mate() = task;
    }
}

void Screen::logoutTask(ScreenTask *task)
{
    if(task && task->m_nodeId)
    {
        m_taskList.remove(task->m_nodeId);
        //task->m_screen = nullptr;
        task->m_nodeId = nullptr;
    }
}

void Screen::clearTaskList()
{
    LVLLNode * node = m_taskList.head();
    while (node)
    {
        ScreenTaskNode * task = (ScreenTaskNode *)node;
        (*task)->m_nodeId = nullptr;
        (*task)->m_screen = nullptr;
        delete task->mate();
        node = m_taskList.next(node);
    }

    m_taskList.clear();
}

void Screen::setInited(bool value)
{
    m_inited = value;
}
