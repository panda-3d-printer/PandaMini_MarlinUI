#include "theme.h"
#include "lvtask.hpp"


Theme ** Theme::s_themes = nullptr;
ThemeId Theme::s_activeThemeId = Theme_End;

//// 基本公共元素 ////////////////////////////
IMPLEMENT_THEME_STYLE(Theme_Screen_TitleBar,lv_style_plain)     //标题栏样式
//IMPLEMENT_THEME_STYLE(Theme_Screen_TitleIcon,lv_style_plain)    //标题栏图标
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonBack,lv_style_plain)   //标题栏返回按钮
//IMPLEMENT_THEME_STYLE(Theme_Screen_TitleText,lv_style_plain)    //标题文本
IMPLEMENT_THEME_STYLE(Theme_Screen_StateBar,lv_style_plain)     //状态栏样式
//IMPLEMENT_THEME_STYLE(Theme_Screen_StateText,lv_style_plain)    //状态栏信息文本
IMPLEMENT_THEME_STYLE(Theme_screen_ButtonArea,lv_style_plain)   //按钮区域样式
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonPr,lv_style_btn_pr)    //按钮按下
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonRel,lv_style_btn_rel)  //按钮松开
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonIna,lv_style_btn_ina)  //按钮禁用样式
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonRed,lv_style_btn_rel)    //按钮红色样式
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonGreen,lv_style_btn_rel)  //按钮绿色样式
IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonBlue,lv_style_btn_rel)   //按钮蓝色样式
//IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonImg,lv_style_plain)    //按钮图标样式
//IMPLEMENT_THEME_STYLE(Theme_Screen_ButtonText,lv_style_plain)   //按钮文本样式
IMPLEMENT_THEME_STYLE(Theme_screen_TextBubble,lv_style_plain)   //气泡消息提示
IMPLEMENT_THEME_STYLE(Theme_screen_ProcessBar,lv_style_plain)   //菜单界面按钮固件
IMPLEMENT_THEME_STYLE(Theme_screen_ProcessIndic,lv_style_plain) //菜单界面按钮关于

IMPLEMENT_THEME_STYLE(Theme_screen_Red,lv_style_plain)       //红色样式
IMPLEMENT_THEME_STYLE(Theme_screen_Green,lv_style_plain)     //绿色样式
IMPLEMENT_THEME_STYLE(Theme_screen_Yellow,lv_style_plain) //暗绿色样式
IMPLEMENT_THEME_STYLE(Theme_screen_Blue,lv_style_plain)      //蓝色样式
IMPLEMENT_THEME_STYLE(Theme_screen_BlueDark,lv_style_plain)  //暗蓝色样式
IMPLEMENT_THEME_STYLE(Theme_screen_Cyan,lv_style_plain)      //青色样式
IMPLEMENT_THEME_STYLE(Theme_screen_Gray,lv_style_plain)      //灰色样式





#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 通知主题已经更换了,需要刷新界面
 */
void notificThemeChanged();

#ifdef __cplusplus
}
#endif


ThemeId Theme::themeId() const
{
    return m_themeId;
}

void Theme::setThemeId(const ThemeId &themeId)
{
    m_themeId = themeId;
}

Theme *Theme::activedTheme()
{
    if(s_themes == nullptr || s_activeThemeId > Theme_End)
        return nullptr;
    else
        return s_themes[s_activeThemeId];
}

bool Theme::activeTheme(ThemeId id)
{
    if(id < Theme_End && s_themes != nullptr)
    {
        if(s_activeThemeId != id)
        {
            s_activeThemeId = id;
            //应用主题
            s_themes[id]->setupTheme();

            //延后通知主题已经切换
            //LVTask::once(2,[](){notificThemeChanged();});
            notificThemeChanged();
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool Theme::activeTheme()
{
    return activeTheme(themeId());
}

Theme::Theme(ThemeId id)
{
    initTheme();
    loginTheme(id,this);
}

void Theme::initTheme()
{
    //初始化主题列表
    if(s_themes == nullptr)
    {
        s_themes = new Theme*[Theme_End];
        for (int i = 0; i < Theme_End; ++i)
        {
            s_themes[i] = nullptr;
        }
    }
}



void Theme::loginTheme(ThemeId id, Theme *theme)
{
    s_themes[id] = theme;
}



