#ifndef LANGCHINESE_H
#define LANGCHINESE_H

#include "langenglish.h"
#include "lvgl/lv_misc/lv_log.h"

/**
 * @brief 中文语言
 */
class LangChinese : public LangEnglish
{
    LV_MEMAORY_FUNC

protected:
    LangChinese(LangID id)
        :LangEnglish(id)
    {
    }

    LangChinese()
        :LangEnglish(LANG_ZH)
    {
        LV_LOG_INFO("LangChinese create");
    }

public:

    static void initLang()
    {
       static LangChinese * s_instance = nullptr;
       if(!s_instance)
          s_instance = new LangChinese();
    }

    //欢迎界面文本////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_SplashScreen_Brand,"品牌")  //欢迎屏显示品牌
    IMPLEMENT_TEXTFUNC(Lang_SplashScreen_Brief,"简介")  //欢迎屏显示简介

    //主界面文本//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButMove,"运动")      //主界面按钮移动
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButPreHeat,"预热")   //主界面按钮预热
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButExtrude,"挤出")   //主界面按钮挤出
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButTool,"工具")      //主界面按钮工具
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButPrint,"打印")     //主界面按钮打印
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgConnected,"已连接") //主界面提示已连接
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgNotConnected,"未连接") //主界面提示未连接
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgConnectionLost,"连接中断") //主界面提连接断开
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgSDCardInsert,"SD卡插入") //主界面提示SD卡插入
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgSDCardRemove,"SD卡移除") //主界面提示SD卡移除

    //菜单界面文本//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButTitle,"工具")       //菜单界面标题
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButGCode,"GCode")       //菜单界面按钮GCode
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButLeveling,"调平")    //菜单界面按钮调平
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButCustom,"自定义")      //菜单界面按钮自定义
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButSetting,"设置")     //菜单界面按钮设置
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButCheck,"检测")       //菜单界面按钮检测
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButConnect,"连接")     //菜单界面按钮连接
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButShutDown,"关机")    //菜单界面按钮关机
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButFirmware,"固件")    //菜单界面按钮固件
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButAbout,"关于")       //菜单界面按钮关于

    //打印界面文本/////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabPrinting,"打印中")   //打印界面标题打印中
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabPauseed,"已暂停")    //打印界面标题暂停
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabStoped,"已停止")    //打印界面标题已停止
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabFinished,"已完成")    //打印界面标题已完成

    //温度界面////////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_TemperatureScreen_LabTitle,"温度")    //温度界面标题
    IMPLEMENT_TEXTFUNC(Lang_TemperatureScreen_MesgHit,"点击图标开启预热")    //温度界面标题

    //运动界面//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_MotionScreen_LabTitle,"运动")   //运动界面标题
    IMPLEMENT_TEXTFUNC(Lang_MotionScreen_MesgUnknow,"未归零") //运动界面提示

    //挤出界面//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_ExtrudeScreen_LabTitle,"挤出")   //挤出界面标题
    IMPLEMENT_TEXTFUNC(Lang_ExtrudeScreen_MesgHit,"预热后才能操作挤出")    //挤出界面提示

    //文件界面//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_FileScreen_LabTitle,"文件")   //文件界面标题
    IMPLEMENT_TEXTFUNC(Lang_FileScreen_MesgOpenFile,"确认打开文件?") //文件界面按钮取消

    //消息界面/////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_MesgScreen_LabTitle,"返回") //文件界面按钮取消
    IMPLEMENT_TEXTFUNC(Lang_MesgScreen_ButConfirm,"确认") //文件界面按钮确认
    IMPLEMENT_TEXTFUNC(Lang_MesgScreen_ButCancel,"取消") //文件界面按钮取消
};

#endif // LANGCHINESE_H
