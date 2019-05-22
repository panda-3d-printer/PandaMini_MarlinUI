#ifndef MULTILANG_H
#define MULTILANG_H

#include <lvlang.hpp>
#include <misc/lvmemory.hpp>


// 定义一个方便语言切换的宏
//依赖于对象在这是文本时将文本编号保存在对象的freeNumber中
//如果自由数等于最大数值 MAX_FREENUMBER 则表示没有定义多文本编号,就不进行文本更新

#define CHANGE_LANG(OBJ,FUNC)  if(OBJ && OBJ->getFreeNumber() != MAX_FREENUMBER) { OBJ->FUNC(tr(OBJ->getFreeNumber())); OBJ->realign(); }

// 定义一个方便标签语言切换的宏
#define CHANGE_LABEL_LANG(OBJ)  CHANGE_LANG(OBJ,setText)


/**
 * @brief 翻译文本
 * @param textId 文本编号
 * @return
 *
 * 示例:
 * tr(Lang_SplashScreen_Brand);
 *
 */
const char * tr(uint16_t textId);

class LangBase;

//语言编号
enum LangID : uint8_t
{
    LANG_ZH = 0, //中文
    LANG_EN , //英语

    LANG_END,
};

//语言文本编号
//每添加一个枚举量,请在LangBase中添加对应的虚函数
enum LangTextID : uint16_t
{
    //欢迎界面文本////////////////////////////////////
    Lang_SplashScreen_Brand = 0,  //欢迎屏显示品牌
    Lang_SplashScreen_Brief,      //欢迎屏显示简介

    //主界面文本//////////////////////////////////////
    Lang_HomeScreen_ButMove,      //主界面按钮移动
    Lang_HomeScreen_ButPreHeat,   //主界面按钮预热
    Lang_HomeScreen_ButExtrude,   //主界面按钮挤出
    Lang_HomeScreen_ButTool,      //主界面按钮工具
    Lang_HomeScreen_ButPrint,     //主界面按钮打印
    Lang_HomeScreen_MsgConnected,      //主界面提示已连接
    Lang_HomeScreen_MsgNotConnected,   //主界面提示未连接
    Lang_HomeScreen_MsgConnectionLost, //主界面提连接断开
    Lang_HomeScreen_MsgSDCardInsert,   //主界面提示SD卡插入
    Lang_HomeScreen_MsgSDCardRemove,   //主界面提示SD卡移除

    //菜单界面文本//////////////////////////////////////
    Lang_MenuScreen_ButTitle,       //菜单界面标题
    Lang_MenuScreen_ButGCode,       //菜单界面按钮GCode
    Lang_MenuScreen_ButLeveling,    //菜单界面按钮调平
    Lang_MenuScreen_ButCustom,      //菜单界面按钮自定义
    Lang_MenuScreen_ButSetting,     //菜单界面按钮设置
    Lang_MenuScreen_ButCheck,       //菜单界面按钮检测
    Lang_MenuScreen_ButConnect,     //菜单界面按钮连接
    Lang_MenuScreen_ButShutDown,    //菜单界面按钮关机
    Lang_MenuScreen_ButFirmware,    //菜单界面按钮固件
    Lang_MenuScreen_ButAbout,       //菜单界面按钮关于

    //打印界面文本/////////////////////////////////////
    Lang_PrintingScreen_LabPrinting,   //打印界面标题打印中
    Lang_PrintingScreen_LabPauseed,   //打印界面标题已暂停
    Lang_PrintingScreen_LabStoped,    //打印界面标题已停止
    Lang_PrintingScreen_LabFinished,    //打印界面标题已完成

    //温度界面////////////////////////////////////////
    Lang_TemperatureScreen_LabTitle,  //温度界面标题
    Lang_TemperatureScreen_MesgHit, //温度界面提示

    //运动界面//////////////////////////////////////
    Lang_MotionScreen_LabTitle,   //运动界面标题
    Lang_MotionScreen_MesgUnknow, //运动界面提示

    //挤出界面//////////////////////////////////////
    Lang_ExtrudeScreen_LabTitle,   //挤出界面标题
    Lang_ExtrudeScreen_MesgHit,    //挤出界面提示

    //文件界面//////////////////////////////////////
    Lang_FileScreen_LabTitle,   //文件界面标题
    Lang_FileScreen_MesgOpenFile, //文件界面按钮取消

    //消息界面/////////////////////////////////////
    Lang_MesgScreen_LabTitle, //文件界面按钮取消
    Lang_MesgScreen_ButConfirm, //文件界面按钮确认
    Lang_MesgScreen_ButCancel, //文件界面按钮取消


    //语言文本结束
    LANG_TEXT_END,
};

//声明文本函数的虚函数
#define DECLARE_VIRTUAL_TEXTFUNC(TEXTID) virtual const char * TEXTID##_Str() = 0;
//定义文本函数的实现
#define IMPLEMENT_TEXTFUNC(TEXTID,TEXT) virtual const char * TEXTID##_Str() override { return TEXT; }


#define CaseTextItem(textId) case textId : return textId##_Str();


/**
 * @brief 定义语言基类
 * 使用基类将文本拆分成虚函数,可以防止漏翻译
 */
class LangBase
{
    LV_MEMAORY_FUNC
public:
    LangBase(LangID id);
    virtual ~LangBase(){}

    //欢迎界面文本////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_SplashScreen_Brand)  //欢迎屏显示品牌
    DECLARE_VIRTUAL_TEXTFUNC(Lang_SplashScreen_Brief)  //欢迎屏显示简介

    //主界面文本//////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_ButMove)      //主界面按钮移动
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_ButPreHeat)   //主界面按钮预热
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_ButExtrude)   //主界面按钮挤出
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_ButTool)      //主界面按钮工具
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_ButPrint)     //主界面按钮打印
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_MsgConnected) //主界面提示已连接
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_MsgNotConnected) //主界面提示未连接
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_MsgConnectionLost) //主界面提连接断开
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_MsgSDCardInsert) //主界面提示SD卡插入
    DECLARE_VIRTUAL_TEXTFUNC(Lang_HomeScreen_MsgSDCardRemove) //主界面提示SD卡移除

    //菜单界面文本//////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButTitle)       //菜单界面标题
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButGCode)       //菜单界面按钮GCode
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButLeveling)    //菜单界面按钮调平
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButCustom)      //菜单界面按钮自定义
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButSetting)     //菜单界面按钮设置
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButCheck)       //菜单界面按钮检测
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButConnect)     //菜单界面按钮连接
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButShutDown)    //菜单界面按钮关机
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButFirmware)    //菜单界面按钮固件
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MenuScreen_ButAbout)       //菜单界面按钮关于

    //打印界面文本/////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_PrintingScreen_LabPrinting)   //打印界面标题
    DECLARE_VIRTUAL_TEXTFUNC(Lang_PrintingScreen_LabPauseed)    //打印暂停
    DECLARE_VIRTUAL_TEXTFUNC(Lang_PrintingScreen_LabStoped)    //打印界面标题已停止
    DECLARE_VIRTUAL_TEXTFUNC(Lang_PrintingScreen_LabFinished)    //打印界面标题已完成

    //温度界面////////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_TemperatureScreen_LabTitle)    //温度界面标题
    DECLARE_VIRTUAL_TEXTFUNC(Lang_TemperatureScreen_MesgHit)    //温度界面标题

    //运动界面//////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MotionScreen_LabTitle)   //运动界面标题
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MotionScreen_MesgUnknow) //运动界面提示

    //挤出界面//////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_ExtrudeScreen_LabTitle)   //挤出界面标题
    DECLARE_VIRTUAL_TEXTFUNC(Lang_ExtrudeScreen_MesgHit)    //挤出界面提示

    //文件界面//////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_FileScreen_LabTitle)   //文件界面标题
    DECLARE_VIRTUAL_TEXTFUNC(Lang_FileScreen_MesgOpenFile) //文件界面按钮取消

    //消息界面/////////////////////////////////////
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MesgScreen_LabTitle) //文件界面按钮取消
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MesgScreen_ButConfirm) //文件界面按钮确认
    DECLARE_VIRTUAL_TEXTFUNC(Lang_MesgScreen_ButCancel) //文件界面按钮取消


    const char * getText(uint16_t id)
    {
        switch (id)
        {

        //欢迎界面文本////////////////////////////////////
        CaseTextItem(Lang_SplashScreen_Brand)  //欢迎屏显示品牌
        CaseTextItem(Lang_SplashScreen_Brief)      //欢迎屏显示简介

        //主界面文本//////////////////////////////////////
        CaseTextItem(Lang_HomeScreen_ButMove)      //主界面按钮移动
        CaseTextItem(Lang_HomeScreen_ButPreHeat)   //主界面按钮预热
        CaseTextItem(Lang_HomeScreen_ButExtrude)   //主界面按钮挤出
        CaseTextItem(Lang_HomeScreen_ButTool)      //主界面按钮工具
        CaseTextItem(Lang_HomeScreen_ButPrint)     //主界面按钮打印
        CaseTextItem(Lang_HomeScreen_MsgConnected) //主界面提示已连接
        CaseTextItem(Lang_HomeScreen_MsgNotConnected) //主界面提示未连接
        CaseTextItem(Lang_HomeScreen_MsgConnectionLost) //主界面提连接断开
        CaseTextItem(Lang_HomeScreen_MsgSDCardInsert) //主界面提示SD卡插入
        CaseTextItem(Lang_HomeScreen_MsgSDCardRemove) //主界面提示SD卡移除

        //菜单界面文本//////////////////////////////////////
        CaseTextItem(Lang_MenuScreen_ButTitle)       //菜单界面标题
        CaseTextItem(Lang_MenuScreen_ButGCode)       //菜单界面按钮GCode
        CaseTextItem(Lang_MenuScreen_ButLeveling)    //菜单界面按钮调平
        CaseTextItem(Lang_MenuScreen_ButCustom)      //菜单界面按钮自定义
        CaseTextItem(Lang_MenuScreen_ButSetting)     //菜单界面按钮设置
        CaseTextItem(Lang_MenuScreen_ButCheck)       //菜单界面按钮检测
        CaseTextItem(Lang_MenuScreen_ButConnect)     //菜单界面按钮连接
        CaseTextItem(Lang_MenuScreen_ButShutDown)    //菜单界面按钮关机
        CaseTextItem(Lang_MenuScreen_ButFirmware)    //菜单界面按钮固件
        CaseTextItem(Lang_MenuScreen_ButAbout)       //菜单界面按钮关于

        //打印界面文本/////////////////////////////////////
        CaseTextItem(Lang_PrintingScreen_LabPrinting)   //打印界面标题
        CaseTextItem(Lang_PrintingScreen_LabPauseed)
        CaseTextItem(Lang_PrintingScreen_LabStoped)    //打印界面标题已停止
        CaseTextItem(Lang_PrintingScreen_LabFinished)    //打印界面标题已完成

        //温度界面////////////////////////////////////////
        CaseTextItem(Lang_TemperatureScreen_LabTitle)    //温度界面标题
        CaseTextItem(Lang_TemperatureScreen_MesgHit)    //温度界面标题

        //运动界面//////////////////////////////////////
        CaseTextItem(Lang_MotionScreen_LabTitle)   //运动界面标题
        CaseTextItem(Lang_MotionScreen_MesgUnknow) //运动界面提示

        //挤出界面//////////////////////////////////////
        CaseTextItem(Lang_ExtrudeScreen_LabTitle)   //挤出界面标题
        CaseTextItem(Lang_ExtrudeScreen_MesgHit)    //挤出界面提示

        //文件界面//////////////////////////////////////
        CaseTextItem(Lang_FileScreen_LabTitle)   //文件界面标题
        CaseTextItem(Lang_FileScreen_MesgOpenFile) //文件界面按钮取消

        //消息界面/////////////////////////////////////
        CaseTextItem(Lang_MesgScreen_LabTitle) //文件界面按钮取消
        CaseTextItem(Lang_MesgScreen_ButConfirm) //文件界面按钮确认
        CaseTextItem(Lang_MesgScreen_ButCancel) //文件界面按钮取消



        default:
            return "NullString"; //默认返回错误提示
        }
    }
};

/**
 * @brief 实现多语言切换
 */
class MultiLang : public LVLang
{
protected:

    LangBase ** m_lings = nullptr; //!<语言列表
    LangID m_defaultLang =  LANG_ZH; //!< 默认语言是中文

    MultiLang();
    ~MultiLang();
public:
    static MultiLang * getInstance();

    bool hasLang(LangID id);

    LangBase *lang(LangID id);

    LangBase * defaultLang();

    //注册语言
    void loginLang(LangID langId , LangBase * lang);

    //注销语言
    void logoutLang(LangID langId);
};


#endif // MULTILANG_H
