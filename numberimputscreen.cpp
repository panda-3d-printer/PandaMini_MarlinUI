#include "numberimputscreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include <ui_api.h>
#include "lvslot.hpp"

//用到的字体
LV_FONT_DECLARE(lv_font_dejavu_10)
LV_FONT_DECLARE(lv_font_dejavu_20)
LV_FONT_DECLARE(lv_font_dejavu_30)

//数值
static char g_valueStr[10];

/*Called when a button is released ot long pressed*/
static lv_res_t numpadAction(lv_obj_t * btnm, const char *txt)
{
    printf("Button: %s released\n", txt);

    //退格键
    if(strcmp(txt,"<-") == 0)
    {
        MarlinUi::getInstance()->numberImputScreen()->backSpaceValue();
    }
    else
    {
        MarlinUi::getInstance()->numberImputScreen()->appendValue(txt[0]);
    }

    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

static lv_res_t onButBackClicked (struct _lv_obj_t * obj)
{
    MarlinUi::getInstance()->numberImputScreen()->setHideByUser(true);
    MarlinUi::getInstance()->numberImputScreen()->setOk(false);
    MarlinUi::getInstance()->numberImputScreen()->hide();
}

static lv_res_t onButOkClicked (struct _lv_obj_t * obj)
{
    MarlinUi::getInstance()->numberImputScreen()->setHideByUser(true);
    MarlinUi::getInstance()->numberImputScreen()->setOk(true);
    MarlinUi::getInstance()->numberImputScreen()->hide();
}


float NumberImputScreen::getMin() const
{
    return m_min;
}

void NumberImputScreen::setMin(const float &min)
{
    if(m_min != min)
    {
        m_min = min;
        //strcpy(m_minStr,ftos(min));
        if(isInited())
            labMin()->setValue(min);
    }
}

float NumberImputScreen::getMax() const
{
    return m_max;
}

void NumberImputScreen::setMax(const float &max)
{
    if(m_max != max)
    {
        m_max = max;
        //strcpy(m_maxStr,ftos(max));
        if(isInited())
            labMax()->setValue(max);
    }
}

LVLabel *NumberImputScreen::labTitle()
{
    if(m_labTitle == nullptr)
        m_labTitle = new LVLabel(this,nullptr);
    return m_labTitle;
}

void NumberImputScreen::setTitle(char *title)
{
    labTitle()->setText(title);
}

void NumberImputScreen::setTitle(uint16_t titleId)
{
    labTitle()->setText(titleId);
}

LVLabel *NumberImputScreen::labValue()
{
    if(m_labValue == nullptr)
        m_labValue = new LVLabel(this,nullptr);
    return m_labValue;
}

LVLabel *NumberImputScreen::labMin()
{
    if( m_labMin == nullptr)
        m_labMin = new LVLabel(this,nullptr);
 return m_labMin;
}

LVLabel *NumberImputScreen::labMax()
{
    if( m_labMax == nullptr)
        m_labMax = new LVLabel(this,nullptr);
 return m_labMax;
}

float NumberImputScreen::value()
{
    if(strlen(m_valueStr))
    {
        m_value = stof(m_valueStr);
        if(m_value > m_max)
            m_value == m_max;
        if(m_value < m_min)
            m_value = m_min;
    }
    else
    {
        m_value = m_min;
    }
    return m_value;
}
void NumberImputScreen::setValue(const float &value)
{
    if(m_value != value)
    {
        m_value = value;

        //数值判断
        //NOTE:需要先设置最值
        if(value > m_max)
            m_value = m_max;
        if(value < m_min)
            m_value = m_min;
        strcpy(m_valueStr,ftos(m_value));
        if(isInited())
            labValue()->setStaticText(m_valueStr);
    }
}

void NumberImputScreen::backSpaceValue()
{
    //退格,不检查数值范围
    int len =  strlen(m_valueStr);
    if(len > 0)
    {
        m_valueStr[len-1] = '\0';
        labValue()->setStaticText(m_valueStr);
    }
}

void NumberImputScreen::appendValue(char a)
{

    //输入数值,检查数值范围
    //BUG:这样处理的话,当最小值大于0会造成很多数字无法输入

    int len =  strlen(m_valueStr);
    bool allZero = true; //数字是否全是0
    bool hasDot = false;
    int8_t dotNum = -1;
    if(len < NUM_STR_LEN-2)
    //if( len+1 <= strlen(itos((int32_t)m_max)) )
    {

        for (int i = 0; i < len; ++i)
        {
            if(m_valueStr[i] != '0')
                allZero = false;
            if(m_valueStr[i] == '.')
                hasDot = true;
            if(hasDot)
                ++dotNum;
        }

        if(dotNum>=3)
        {
            return;
        }

        if(a == '0')
        {
            if(len == 1 && m_valueStr[0] == '0')
            {
                //数字首位已经输入了0,就不能再输入了
                return;
            }

            if(allZero) //将全是0的数字串变成一个0
            {
                strcpy(m_valueStr,"0");
                labValue()->setStaticText(m_valueStr);
                return;
            }
        }
        else if(a == '.')
        {
            //只能输入一个.
            if(len > 1 && m_valueStr[len-1] == '.')
            {
                return;
            }
        }


        m_valueStr[len] = a;
        m_valueStr[len+1] = '\0';

        //BUG:输入0造成数值超界
        if(!(a == '0' && hasDot) && a >= '0' && a <= '9') //输入数字时检查范围
        {
            //延后输入值的检查
            float value ;
            sscanf(m_valueStr,"%f",&value);

            if(value > m_min)
                setValue(value);
            else //允许输入的值比最小值小,因为在接下在的输入种他可能会变大
                labValue()->setStaticText(m_valueStr);
        }
        else
        {
            //输入其他字符暂时,只检查长度
            labValue()->setStaticText(m_valueStr);
        }
    }

}

NumInputFunc NumberImputScreen::numInputFunc() const
{
    return m_numInputFunc;
}

void NumberImputScreen::setNumInputFunc(const NumInputFunc &numInputFunc)
{
    m_numInputFunc = numInputFunc;
}

void NumberImputScreen::resetNumInput()
{
    setMin(0);
    setMax(1000);
    setValue(getMin());
    setOk(false);
    setHideByUser(false);
}

bool NumberImputScreen::ok() const
{
    return m_ok;
}

void NumberImputScreen::setOk(bool ok)
{
    m_ok = ok;
}

bool NumberImputScreen::hideByUser() const
{
    return m_hideByUser;
}

void NumberImputScreen::setHideByUser(bool userHasInput)
{
    m_hideByUser = userHasInput;
}

NumberImputScreen::NumberImputScreen()
    :Screen()
{

}


bool NumberImputScreen::initScreen()
{
    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 6;
    const int16_t titleBarHeight = 32;
    const int16_t valueBarHeight = 64;

    const int16_t butAreaMargin = 10;

    const int16_t fontSize = 16;
    const int16_t iconSize = 24;

    LVObject * reference = nullptr; //布局参考对象

    {
        //标题栏
        LVObject * titleBar = new LVObject(this,nullptr);
        titleBar->setStyle(THEME_STYLE_GET(Theme_Screen_TitleBar));
        titleBar->setSize(screenWidth,titleBarHeight);
        titleBar->align(LV_ALIGN_IN_TOP_LEFT);
        reference = titleBar;

        //标题返回按钮
        LVButton * butBack = new LVButton(titleBar,nullptr);
        butBack->setSize(titleBarHeight,titleBarHeight);
        butBack->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butBack->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butBack->align(LV_ALIGN_IN_LEFT_MID,safeMargin,0);
        butBack->setAction(LV_BTN_ACTION_CLICK,onButBackClicked);
        LVImage * imgBack = new LVImage(butBack,nullptr);
        imgBack->setSrc(&back_24);

        //标题
        labTitle()->setParent(titleBar);
        labTitle()->align(butBack,LV_ALIGN_OUT_RIGHT_MID);
        labTitle()->setAutoRealign(true);

        //确认按钮
        LVButton * butOk = new LVButton(titleBar,nullptr);
        butOk->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butOk->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butOk->setSize(titleBarHeight,titleBarHeight);
        butOk->align(LV_ALIGN_IN_RIGHT_MID,-spacing,0);
        butOk->setAction(LV_BTN_ACTION_CLICK,onButOkClicked);
        LVImage * imgOk = new LVImage(butOk,nullptr);
        imgOk->setSrc(&ok_24);
        imgOk->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));
    }

    {
        //功能区域

        m_butArea = new LVObject(this,nullptr);
        m_butArea->align(LV_ALIGN_IN_TOP_LEFT,0,titleBarHeight);
        m_butArea->setSize(screenWidth,screenHeight - titleBarHeight);
        m_butArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));

        //数值区域

        static LVStyle styleValue(THEME_STYLE_GET(Theme_screen_ButtonArea));
        styleValue->text.font = &lv_font_dejavu_40;

        static LVStyle styleMinMax(THEME_STYLE_GET(Theme_screen_ButtonArea));
        styleMinMax->text.font = &lv_font_dejavu_20;
        styleMinMax->text.color = THEME_STYLE_GET(Theme_screen_Gray)->text.color;

        labValue()->setParent(m_butArea);
        labValue()->setStyle(styleValue);
        labValue()->align(LV_ALIGN_IN_TOP_MID,0,valueBarHeight/4);
        labValue()->setAlign(LV_LABEL_ALIGN_CENTER);
        labValue()->setAutoRealign(true);
        labValue()->setStaticText(m_valueStr);

        labMin()->setParent(m_butArea);
        labMin()->setStyle(styleMinMax);
        labMin()->setAlign(LV_LABEL_ALIGN_CENTER);
        labMin()->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,valueBarHeight-20);
        labMin()->setAutoRealign(true);
        labMin()->setValue(m_min);

        labMax()->setParent(m_butArea);
        labMax()->setStyle(styleMinMax);
        labMax()->setAlign(LV_LABEL_ALIGN_CENTER);
        labMax()->align(LV_ALIGN_IN_TOP_RIGHT,-safeMargin,valueBarHeight-20);
        labMax()->setAutoRealign(true);
        labMax()->setValue(m_max);

        //数字键盘区域

        /*Create a button descriptor string array*/
        static const char * btnm_map[] = {"1", "2", "3", "<-", "\n",
                                          "4", "5", "6", "." , "\n",
                                          "7" ,"8", "9", "0" , "" };

        /*Create a new style for the button matrix back ground*/
        static LVStyle style_bg(THEME_STYLE_GET(Theme_screen_ButtonArea));
        style_bg->body.padding.hor = 0;
        style_bg->body.padding.ver = 0;
        style_bg->body.padding.inner = 2;

        /*Create 2 button styles*/
        static LVStyle style_btn_rel(THEME_STYLE_GET(Theme_Screen_ButtonRel));
        style_btn_rel->body.main_color = THEME_STYLE_GET(Theme_screen_BlueDark)->body.main_color;
        style_btn_rel->body.grad_color = THEME_STYLE_GET(Theme_screen_BlueDark)->body.main_color;
        style_btn_rel->text.font = &lv_font_dejavu_20;
        //style_btn_rel->body.border.color = LV_COLOR_SILVER;
        //style_btn_rel->body.border.width = 1;
        //style_btn_rel->body.border.opa = LV_OPA_50;
        //style_btn_rel->body.radius = 0;

        static LVStyle style_btn_pr(THEME_STYLE_GET(Theme_Screen_ButtonPr));
        style_btn_pr->body.main_color = THEME_STYLE_GET(Theme_screen_BlueDark)->body.main_color;
        style_btn_pr->body.grad_color = THEME_STYLE_GET(Theme_screen_BlueDark)->body.main_color;
        style_btn_pr->text.font = &lv_font_dejavu_20;

        /*Create a second button matrix with the new styles*/
        LVButtonMatrix * numPad = new LVButtonMatrix(m_butArea,nullptr);
        numPad->setMap(btnm_map);
        numPad->setSize(screenWidth-1,screenHeight-titleBarHeight-valueBarHeight);
        numPad->setStyle(LV_BTNM_STYLE_BG, style_bg);
        numPad->setStyle(LV_BTNM_STYLE_BTN_REL, style_btn_rel);
        numPad->setStyle(LV_BTNM_STYLE_BTN_PR, style_btn_pr);
        numPad->align(LV_ALIGN_IN_BOTTOM_MID, 0, 0);
        numPad->setAction(numpadAction);
    }

    LV_LOG_INFO(__FUNCTION__);

    return true;
}

void NumberImputScreen::onLangChanged()
{
}

void NumberImputScreen::onThemeChanged()
{
}


void NumberImputScreen::afterHide()
{
    //执行回调函数
    if(m_numInputFunc != nullptr && m_hideByUser)
        m_numInputFunc(value(),ok());

    //重置数字输入器
    resetNumInput();
}


bool NumberImputScreen::beforeShow()
{
    bool ret = Screen::beforeShow();
    if(ret)
    {
        //重定位布局
    }

    return ret;
}


void NumberImputScreen::afterCleanScreen()
{
    m_labTitle = nullptr;
    m_labValue = nullptr;
    m_labMin = nullptr;
    m_labMax = nullptr;
}
