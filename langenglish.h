#ifndef LANGENGLISH_H
#define LANGENGLISH_H

#include "multilang.h"
#include "lvgl/lv_misc/lv_log.h"

/**
 * @brief 英语语言文本
 */
class LangEnglish : public LangBase
{
    LV_MEMAORY_FUNC

protected:
    LangEnglish(LangID id)
        :LangBase(id)
    {}

    LangEnglish()
        :LangBase(LANG_EN)
    {
        LV_LOG_INFO("LangEnglish create");
    }
public:

    static void initLang()
    {
        static LangEnglish * s_instance = nullptr;
        if(!s_instance)
            s_instance = new LangEnglish();
    }

    //欢迎界面文本////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_SplashScreen_Brand,"Brand")  //欢迎屏显示品牌
    IMPLEMENT_TEXTFUNC(Lang_SplashScreen_Brief,"Brief")  //欢迎屏显示简介

    //主界面文本//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButMove,"Motion")      //主界面按钮移动
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButPreHeat,"Temperature")   //主界面按钮预热
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButExtrude,"Extrude")   //主界面按钮挤出
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButTool,"Tool")      //主界面按钮工具
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_ButPrint,"Print")     //主界面按钮打印
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgConnected,"Connected") //主界面提示已连接
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgNotConnected,"Not Connected") //主界面提示未连接
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgConnectionLost,"Connection Lost") //主界面提连接断开
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgSDCardInsert,"SD Card Insert") //主界面提示SD卡插入
    IMPLEMENT_TEXTFUNC(Lang_HomeScreen_MsgSDCardRemove,"SD Card Remove") //主界面提示SD卡移除

    //菜单界面文本//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButTitle,"Tool")       //菜单界面标题
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButGCode,"GCode")       //菜单界面按钮GCode
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButLeveling,"Leveling")    //菜单界面按钮调平
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButCustom,"Custom")      //菜单界面按钮自定义
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButSetting,"Setting")     //菜单界面按钮设置
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButCheck,"Check")       //菜单界面按钮检测
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButConnect,"Connect")     //菜单界面按钮连接
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButShutDown,"ShutDown")    //菜单界面按钮关机
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButFirmware,"Firmware")    //菜单界面按钮固件
    IMPLEMENT_TEXTFUNC(Lang_MenuScreen_ButAbout,"About")       //菜单界面按钮关于

    //打印界面文本/////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabPrinting,"Printing")   //打印界面标题打印中
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabPauseed,"Pauseed")    //打印界面标题暂停
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabStoped,"Stoped")    //打印界面标题已停止
    IMPLEMENT_TEXTFUNC(Lang_PrintingScreen_LabFinished,"Finished")    //打印界面标题已完成

    //温度界面////////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_TemperatureScreen_LabTitle,"Temperature")    //温度界面标题
    IMPLEMENT_TEXTFUNC(Lang_TemperatureScreen_MesgHit,"Click the icon to turn on heating")    //温度界面标题

    //运动界面//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_MotionScreen_LabTitle,"Motion")   //运动界面标题
    IMPLEMENT_TEXTFUNC(Lang_MotionScreen_MesgUnknow,"Unknow") //运动界面提示

    //挤出界面//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_ExtrudeScreen_LabTitle,"Extrude")   //挤出界面标题
    IMPLEMENT_TEXTFUNC(Lang_ExtrudeScreen_MesgHit,"Preheat before operation")    //挤出界面提示

    //文件界面//////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_FileScreen_LabTitle,"File")   //文件界面标题
    IMPLEMENT_TEXTFUNC(Lang_FileScreen_MesgOpenFile,"Open this file ?") //文件界面按钮取消

    //消息界面/////////////////////////////////////
    IMPLEMENT_TEXTFUNC(Lang_MesgScreen_LabTitle,"Return") //文件界面按钮取消
    IMPLEMENT_TEXTFUNC(Lang_MesgScreen_ButConfirm,"Confirm") //文件界面按钮确认
    IMPLEMENT_TEXTFUNC(Lang_MesgScreen_ButCancel,"Cancel") //文件界面按钮取消
};


#endif // LANGENGLISH_H
