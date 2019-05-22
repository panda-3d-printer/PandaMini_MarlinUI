#ifndef THEME_H
#define THEME_H

#include <lvtheme.h>
#include <lvstyle.hpp>
#include <misc/lvmemory.hpp>

/**
 * @brief The ThemeId enum
 */
enum ThemeId : uint8_t
{
    Theme_Dark = 0,  //暗色主题
    Theme_Light, //亮色主题
    Theme_End
};

/**
 * @brief 接受主题数据的对象ID
 * 每添加一个对象需要在Theme中添加一个对应的基类虚函数
 */
enum ThemeObjId : uint16_t
{
    //// 基本公共元素 ////////////////////////////
    Theme_Screen_TitleBar = 0, //标题栏样式
    Theme_Screen_TitleIcon,    //标题栏图标
    Theme_Screen_ButtonBack,   //标题栏返回按钮
    Theme_Screen_TitleText,    //标题文本
    Theme_Screen_StateBar,     //状态栏样式
    Theme_Screen_StateText,    //状态栏信息文本
    Theme_screen_ButtonArea,   //按钮区域样式
    Theme_Screen_ButtonPr,     //按钮按下样式
    Theme_Screen_ButtonRel,    //按钮松开样式
    Theme_Screen_ButtonIna,    //按钮禁用样式
    Theme_Screen_ButtonRed,    //按钮红色样式
    Theme_Screen_ButtonGreen,  //按钮绿色样式
    Theme_Screen_ButtonBlue,   //按钮蓝色样式
    Theme_Screen_ButtonImg,    //按钮图标样式
    Theme_Screen_ButtonText,   //按钮文本样式
    Theme_screen_TextBubble,   //气泡消息提示样式
    Theme_screen_ProcessBar,   //进度条背景样式
    Theme_screen_ProcessIndic, //进度条前景提示样式

    Theme_screen_Red,       //红色样式
    Theme_screen_Green,     //绿色样式
    Theme_screen_Yellow, //绿色样式
    Theme_screen_Blue,      //蓝色样式
    Theme_screen_BlueDark,  //暗蓝色样式
    Theme_screen_Cyan,      //青色样式
    Theme_screen_Gray,      //灰色样式

    ////欢迎屏///////////////////////
    Theme_SplashScreen_Img, // 欢迎屏图片
    Theme_SplashScreen_TextBrand, //欢迎屏品牌
    Theme_SplashScreen_TextBrief, //欢迎牌简介

    /////主界面///////////////////////////
    Theme_HomeScreen_ButMove, //移动按钮
    Theme_HomeScreen_ButPreHeat, //预热按钮
    Theme_HomeScreen_ButExtrude, //挤出按钮
    Theme_HomeScreen_ButTool,  //挤出按钮
    Theme_HomeScreen_ButPrint,  //打印按钮

    /////菜单工具界面///////////////////////////
    Theme_MenuScreen_ButTitle,    //菜单界面标题
    Theme_MenuScreen_ButGCode,    //菜单界面按钮GCode
    Theme_MenuScreen_ButLeveling, //菜单界面按钮调平
    Theme_MenuScreen_ButCustom,   //菜单界面按钮自定义
    Theme_MenuScreen_ButSetting,  //菜单界面按钮设置
    Theme_MenuScreen_ButCheck,    //菜单界面按钮检测
    Theme_MenuScreen_ButConnect,  //菜单界面按钮连接
    Theme_MenuScreen_ButShutDown, //菜单界面按钮关机
    Theme_MenuScreen_ButFirmware, //菜单界面按钮固件
    Theme_MenuScreen_ButAbout,    //菜单界面按钮关于

    //主题数据项结束
    THEME_OBJ_END
};

#define THEME_STYLE_INCLASS(OBJID) m_##OBJID##_Style

#define THEME_STYLE_GET(OBJID)  Theme::activedTheme()->OBJID##_Style()

//声明样式对象变量
#define DECLARE_THEME_STYLE(OBJID) static LVStyle THEME_STYLE_INCLASS(OBJID);
#define IMPLEMENT_THEME_STYLE(OBJID,STYLE) LVStyle Theme::m_##OBJID##_Style(&STYLE);

//声明样式获取函数
#define DECLARE_VIRTUAL_THEMEFUNC(OBJID) virtual lv_style_t * OBJID##_Style() = 0;
#define IMPLEMENT_THEMEFUNC_OUTCLASS(OBJID) lv_style_t * Theme::##OBJID##_Style(){ return m_##OBJID##_Style; }
#define IMPLEMENT_THEMEFUNC_INCLASS(OBJID)  lv_style_t * OBJID##_Style(){ return m_##OBJID##_Style; }
#define IMPLEMENT_VIRTUAL_THEMEFUNC_NULLPTR(OBJID)  virtual lv_style_t * OBJID##_Style(){ return nullptr; }

/**
 * @brief 界面主题
 *
 * 定义好界面上会用到的所有
 * 风格样式,字体资源,文字资源等外观数据
 * 由子类去初始化,并切换使用
 *
 */
class Theme
{
    LV_MEMAORY_FUNC
private:
    /**
     * @brief 初始化主题基类
     */
    static void initTheme();

protected:

    static ThemeId s_activeThemeId; //!< 激活的主题
    static Theme ** s_themes; //!< 主题列表

    ThemeId m_themeId; //!< 主题对象ID

    /**
     * @brief 构造主题
     */
    Theme(ThemeId id);

    /**
     * @brief 添加登记主题
     * @param id
     * @param theme
     */
    static void loginTheme(ThemeId id,Theme * theme);

    /**
     * @brief 设置主题ID编号
     * @param themeId
     */
    void setThemeId(const ThemeId &themeId);

//// 基本公共元素 ////////////////////////////
    DECLARE_THEME_STYLE(Theme_Screen_TitleBar)
//    DECLARE_THEME_STYLE(Theme_Screen_TitleIcon)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonBack)
//    DECLARE_THEME_STYLE(Theme_Screen_TitleText)
    DECLARE_THEME_STYLE(Theme_Screen_StateBar)
//    DECLARE_THEME_STYLE(Theme_Screen_StateText)
    DECLARE_THEME_STYLE(Theme_screen_ButtonArea)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonPr)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonRel)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonIna)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonRed)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonGreen)
    DECLARE_THEME_STYLE(Theme_Screen_ButtonBlue)
//    DECLARE_THEME_STYLE(Theme_Screen_ButtonImg)
//    DECLARE_THEME_STYLE(Theme_Screen_ButtonText)
    DECLARE_THEME_STYLE(Theme_screen_TextBubble)
    DECLARE_THEME_STYLE(Theme_screen_ProcessBar)
    DECLARE_THEME_STYLE(Theme_screen_ProcessIndic)

    DECLARE_THEME_STYLE(Theme_screen_Red)       //红色样式
    DECLARE_THEME_STYLE(Theme_screen_Green)     //绿色样式
    DECLARE_THEME_STYLE(Theme_screen_Yellow) //暗绿色样式
    DECLARE_THEME_STYLE(Theme_screen_Blue)      //蓝色样式
    DECLARE_THEME_STYLE(Theme_screen_BlueDark)  //暗蓝色样式
    DECLARE_THEME_STYLE(Theme_screen_Cyan)      //青色色样式
    DECLARE_THEME_STYLE(Theme_screen_Gray)      //灰色图片样式

public:

    ////定义基本界面风格对象

    //// 基本公共元素 ////////////////////////////
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_TitleBar)     //标题栏样式
    IMPLEMENT_VIRTUAL_THEMEFUNC_NULLPTR(Theme_Screen_TitleIcon)    //标题栏图标
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonBack)    //标题栏返回按钮
    IMPLEMENT_VIRTUAL_THEMEFUNC_NULLPTR(Theme_Screen_TitleText)    //标题文本
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_StateBar)     //状态栏样式
    IMPLEMENT_VIRTUAL_THEMEFUNC_NULLPTR(Theme_Screen_StateText)    //状态栏信息文本
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_ButtonArea)   //按钮区域样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonPr)     //按钮按下
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonRel)    //按钮松开
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonIna)    //按钮禁用样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonRed)    //按钮红色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonGreen)  //按钮绿色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_Screen_ButtonBlue)   //按钮蓝色样式
    IMPLEMENT_VIRTUAL_THEMEFUNC_NULLPTR(Theme_Screen_ButtonImg)    //按钮图标样式
    IMPLEMENT_VIRTUAL_THEMEFUNC_NULLPTR(Theme_Screen_ButtonText)   //按钮文本样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_TextBubble)   //气泡消息提示
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_ProcessBar)   //菜单界面进度背景
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_ProcessIndic) //菜单界面进度前景

    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_Red)       //红色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_Green)     //绿色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_Yellow) //暗绿色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_Blue)      //蓝色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_BlueDark)  //暗蓝色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_Cyan)      //青色样式
    IMPLEMENT_THEMEFUNC_INCLASS(Theme_screen_Gray)      //灰色样式

    /**
     * @brief 激活的主题对象
     * @return
     */
    static Theme * activedTheme();

    /**
     * @brief 激活的主题对象ID
     * @return
     */
    static ThemeId activedThemeId(){ return s_activeThemeId; }

    /**
     * @brief 激活主题
     * return 主题是否激活成功
     */
    static bool activeTheme(ThemeId id);

    bool activeTheme();

    /**
     * @brief 设置主题
     * 由子类实现对主题的设定
     * @param id
     */
    virtual void setupTheme() = 0;

    /**
     * @brief 主题的ID编号
     * @return
     */
    ThemeId themeId() const;
};

#endif // THEME_H
