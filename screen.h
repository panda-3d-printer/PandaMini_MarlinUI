#ifndef SCREEN_H
#define SCREEN_H

#include <lvobject.hpp>
#include <lvlinklist.hpp>
#include <lvcolor.hpp>
#include <lvmessagebox.hpp>

class ScreenTask;
class LVBar;
class LVLabel;
class LVMessageBox;

// 方便删除指针对象
#define DELETE_PTR(ptr) delete ptr; ptr = nullptr;

/**
 * @brief 定义一个屏幕界面的基类
 * 屏幕管理
 *
 * 控制显示
 * 控制隐藏
 *
 * 管理与屏幕相关的任务
 *
 */
class Screen : public LVObject
{
    LV_OBJECT

    friend class MarlinUi;
    friend class ScreenTask;
    friend lv_res_t screenSignalFunc(struct _lv_obj_t * obj, lv_signal_t sign, void * param);
protected:

    ////////// 形成屏幕的显示链 ////////////////
    Screen * m_preScreen = nullptr; //!< 前一个屏幕
    Screen * m_nextScreen = nullptr; //!< 后一个屏幕

    static Screen * s_lastScreen; //!< 之前显示的屏幕
    static Screen * s_currScreen; //!< 当前显示的屏幕

    /////////// 任务列表 ////////////
    LVLinkList m_taskList; //!< 屏幕拥有的任务列表

    //////////// 外观属性 /////////////////
    LVColor m_screenColor; //!< 屏幕颜色
    bool m_inited = false; //!< 标识屏幕是否初始化
    bool m_deleteAfterHide = false; //!< 隐藏后清理屏幕数据
    bool m_clearAfterHide = true; //!< 隐藏后清理屏幕(子对象)数据
    int32_t m_memoryUsed = -1; //!< 统计内存消耗

    //////////// 内存调试 //////////////////

    /**
     * @brief 内存调试器
     * 一个简单的线条指示内存状态
     * @return
     */
    static LVBar *&memoryDebuger();
    /**
     * @brief 安装内存调试器到屏幕界面下方
     * @param screen
     */
    static void installMemoryDebuger(Screen * screen);

    /**
     * @brief 卸载内存调试器
     */
    static void uninstallMemoryDebuger();

    /**
     * @brief 气泡消息
     * @return
     */
    static LVLabel *&bubble();

    static void installBubble(Screen * screen);
    static void uninstallBubble();

    /**
     * @brief 消息提示框
     * @return
     */
    static LVMessageBox *& messageBox();

    static void installMessageBox(Screen * screen);
    static void uninstallMessageBox();

    /**
     * @brief 安装些公共组件到屏幕界面
     * @param screen
     */
    static void installPublicComponents(Screen * screen );

    /**
     * @brief 卸载公屏幕界面的公共组件
     */
    static void uninstallPublicComponents();

public:
    Screen();

    ~Screen();

    /**
     * @brief 显示屏幕
     * @return true 显示成功 ; false 显示失败
     */
    bool show();

    /**
     * @brief 显示之前运行的函数
     * 可用来判断屏幕是否能够显示
     * @return true 可以显示 ; false 不能显示
     */
    virtual bool beforeShow();

    /**
     * @brief 显示之后
     */
    virtual void afterShow(){}

    /**
     * @brief 隐藏屏幕
     * 跳转到上一个屏幕或者主屏幕
     * @return true 显示成功 ; false 显示失败
     */
    bool hide();

    /**
     * @brief 隐藏之前
     * 可用来判断屏幕是否能够隐藏
     * @return true 可以显示 ; 不能隐藏
     */
    virtual bool beforeHide(){return true;}

    /**
     * @brief 隐藏之后
     */
    virtual void afterHide(){}

    /**
     * @brief 退回到上一个显示的屏幕
     */
    void backScreen();
    bool hasBackScreen();

    /**
     * @brief 显示前一个屏幕
     */
    void preScreen();
    bool hasPreScreen();
    void setPreScreen(Screen * screen);

    /**
     * @brief 显示后一个屏幕
     */
    void nextScreen();
    bool hasNextScreen();
    void setNextScreen(Screen * screen);

    /**
     * @brief 在屏幕上是否可见
     * @return
     */
    bool isVisible();

    /**
     * @brief 设置屏幕的显隐
     * @param value
     */
    void setVisible(bool value);

    /**
     * @brief 最后显示的一个屏幕界面
     * @return
     */
    static Screen * LastScreen();

    /**
     * @brief 当前显示的屏幕界面
     * @return
     */
    static Screen * CurrScreen();

    /**
     * @brief 屏幕的背景颜色
     * @return
     */\
    LVColor screenColor();

    /**
     * @brief 设置屏幕的背景颜色
     * @param color
     */
    void setScreenColor(LVColor color);

    bool isDeleteAfterHide();
    void setDeleteAfterHide(bool value);

    bool isInited(){ return m_inited; }

    /**
     * @brief 获取屏幕宽度
     * @return
     */
    static uint16_t width();

    /**
     * @brief 获取屏幕高度
     * @return
     */
    static uint16_t height();

    /**
     * @brief 获取屏幕像素密度
     * @return
     */
    static uint16_t dpi();

    bool isClearAfterHide() const;
    void setClearAfterHide(bool isClearAfterHide);

    /**
     * @brief 清除屏幕
     */
    void cleanScreen();

    /**
     * @brief 获取已用内存
     * @return
     */
    static int32_t getUsedMemorySize();

    /**
     * @brief 显示气泡消息
     * @param mesg 默认停留显示3秒
     * @param period
     */
    static void showBubble(const char * mesg , uint32_t period = 3000);

    /**
     * @brief 显示消息框
     * @param mesg 消息文本
     * @param buts 按钮文本
     * @param action 回调函数
     * @param period 显示的周期  -1表示不自动关闭
     */
    static void showMessageBox(const char * mesg , const char ** buts ,
                               lv_btnm_action_t action , int32_t period = -1);

protected:

    /**
     * @brief 事件信号处理函数
     * @param sign
     * @param param
     * @return
     */
    virtual lv_res_t signal(lv_signal_t sign, void * param);

    /**
     * @brief 处理语言切换
     */
    virtual void onLangChanged();

    /**
     * @brief 处理主题切换
     */
    virtual void onThemeChanged();

    /**
     * @brief 屏幕清理前
     */
    virtual void beforeCleanScreen();

    /**
     * @brief 屏幕清理后
     */
    virtual void afterCleanScreen();

    /**
     * @brief 初始化屏幕
     * 初始化屏幕显示和任务
     * @return
     */
    virtual bool initScreen();

    /**
     * @brief 设置之前显示的屏幕
     * @param screen
     */
    static void setLastScreen(Screen * screen);

    /**
     * @brief 设置当前显示的屏幕
     * @param screen
     */
    static void setCurrScreen(Screen * screen);

    /**
     * @brief 开始屏幕任务
     */
    virtual void startScreenTask();

    /**
     * @brief 关闭屏幕任务
     */
    virtual void stopScreenTask();

    /**
     * @brief 注册任务
     */
    void loginTask(ScreenTask * task);

    /**
     * @brief 注销任务
     */
    void logoutTask(ScreenTask *task);

    /**
     * @brief 清除任务列表
     */
    void clearTaskList();

    /**
     * @brief 设置屏幕初始化标识
     * @param value
     */
    void setInited(bool value);
};

#endif // SCREEN_H
