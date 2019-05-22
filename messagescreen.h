#ifndef COMFIRMSCREEN_H
#define COMFIRMSCREEN_H

#include "screen.h"
#include <functional>
#include <lvslot.hpp>

class LVObject;
class LVButton;
class LVLabel;
class LVContainer;
class MesgParam;

enum MessageResult
{
    MR_Unknow = 0,
    MR_Comfirm,     //确认
    MR_Cancel,      //取消
    MR_Back,        //返回

    MR_USER = 10, //用户自定义
};

/**
 *  @brief确认界面执行完后的回调函数
 *
 * 例子:
 *
 * void mesgAction(MessageResult ret)
 * {
 *      switch()
 *      {
 *          case CR_Back   : ;break;
 *          case CR_Cancel : ;break;
 *          case CR_Back   : ;break;
 *      }
 * }
 *
 */
using MessageFunc = std::function<void(uint32_t ret)>;

/**
 * @brief 消息确认界面
 *
 * TODO: 考虑屏幕被强占的情况,此时不应该执行回调函数
 */
class MessageScreen : public Screen
{
    LV_OBJECT
protected:
    MessageFunc m_messageFunc; //!< 消息动作
    uint32_t m_result; //!< 消息结果 //MessageResult
    LVSlot m_printerKilledSlot; //!< 打印机被杀时槽对象
    bool m_hideByUser = false; //!< 用户造成的界面隐藏

    int16_t m_butWidth = 60;
    int16_t m_butHeight = 38;

    LVObject * m_butArea = nullptr;
    LVContainer * m_contButs = nullptr;

    LVLabel* m_labTitle = nullptr;
    LVLabel* m_labMesg = nullptr;

    LVButton * m_button0 = nullptr;
    LVButton * m_button1 = nullptr;
    LVButton * m_button2 = nullptr;

    LVLabel* m_labBut0 = nullptr;
    LVLabel* m_labBut1 = nullptr;
    LVLabel* m_labBut2 = nullptr;

public:

    MessageScreen();

    ~MessageScreen();

    LVLabel * labTitle();
    LVLabel * labMesg();

    LVButton * button0();
    LVButton * button1();
    LVButton * button2();

    LVLabel * labBut0();
    LVLabel * labBut1();
    LVLabel * labBut2();

    void setTitle(char * title);
    void setTitle(uint16_t titleId);

    void setMessage(const char *mesg);
    void setMessage(uint16_t mesgId);

    /**
     * @brief 设置消息按钮
     * @param text 按钮文本
     * @param retId 按钮触发标识
     */
    void setButton0(char * text , uint32_t resultId);
    void setButton1(char * text , uint32_t resultId);
    void setButton2(char * text , uint32_t resultId);
    void setButton0(uint16_t textId , uint32_t resultId);
    void setButton1(uint16_t textId , uint32_t resultId);
    void setButton2(uint16_t textId , uint32_t resultId);

    void clearAllBut();

    uint32_t result() const;
    void setResult(const uint32_t &result);

    MessageFunc messageFunc() const;
    void setMessageFunc(const MessageFunc &messageFunc);

    void onPrinterKilled(LVSignal* signal);

    bool hideByUser() const;
    void setHideByUser(bool hideByUser);

protected:
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;
    virtual bool initScreen() override;
    virtual void afterHide() override;
    virtual bool beforeShow() override;
    virtual void afterCleanScreen() override;
};

#endif // COMFIRMSCREEN_H
