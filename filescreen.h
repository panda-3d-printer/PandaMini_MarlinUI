#ifndef FILESCREEN_H
#define FILESCREEN_H

#include "screen.h"
#include "screentask.h"
#include <lvslot.hpp>
#include <ui_api.h>

class LVSlot;
class LVStyle;

class LVButton;
class LVLabel;
class LVImage;

/**
 * @brief 文件浏览界面
 *
 * BUG:无法保存文件视图状态
 *
 */
class FileScreen :public Screen
{
    LV_OBJECT
protected:
    //char m_printFileName[30]; //!<打印文件名 marlin中最长文件名是24字节
    char m_dirName[256]; //!< 目录路径
    char m_folderName[25]; //!< 文件夹名称
    uint16_t m_filePage = 0;
    ExtUI::FileList m_fileList; //!< 文件列表
    bool m_keepViewer = false; //!< 保持

    LVLabel* m_labPath = nullptr;

    LVButton * m_butItem0 = nullptr;
    LVImage  * m_imgItem0 = nullptr;
    LVLabel  * m_labItem0 = nullptr;

    LVButton * m_butItem1 = nullptr;
    LVImage  * m_imgItem1 = nullptr;
    LVLabel  * m_labItem1 = nullptr;

    LVButton * m_butItem2 = nullptr;
    LVImage  * m_imgItem2 = nullptr;
    LVLabel  * m_labItem2 = nullptr;

    LVButton * m_butItem3 = nullptr;
    LVImage  * m_imgItem3 = nullptr;
    LVLabel  * m_labItem3 = nullptr;

    LVButton * butPre = nullptr;
    LVButton * butNext = nullptr;
    LVLabel * labPageIdx = nullptr;

    LVSlot m_slotMediaInserted ; //!< 存储设备插入
    LVSlot m_slotMediaError ;    //!< 存储设备错误
    LVSlot m_slotMediaRemoved ;  //!< 存储设备移除

    //LVStyle * m_styleNavigation = nullptr;  //!< 导航按钮样式

public:

    enum ItemType : unsigned char
    {
        folder = 0b00001111,
        file = 0b11110000,
    };

    FileScreen();
    virtual ~FileScreen();

    /**
     * @brief 重置文件到根目录
     */
    void resetFileList();

    void setFileItem0(const char * name,bool isDir);
    void setFileItem1(const char * name,bool isDir);
    void setFileItem2(const char * name,bool isDir);
    void setFileItem3(const char * name,bool isDir);

    void setFileItem(const char * name,bool isDir,LVButton * but,LVImage * img , LVLabel * lab);

    uint16_t maxFilePage();

    /**
     * @brief 上一个文件页面
     */
    void preFilePage();

    /**
     * @brief 下一个文件页面
     */
    void nextFilePage();

    /**
     * @brief 打开文件夹
     * @param name
     */
    void openFolder(const char * name);

    /**
     * @brief 返回上一级目录
     */
    void backFolder();

    /**
     * @brief 刷新文件视图
     */
    void updateFilesView();

    /**
     * @brief 重初始化文件视图
     */
    void reinitFilesView(LVSignal *);


    /**
     * @brief Item按钮按下
     * @param obj
     * @return
     */
    lv_res_t onButItemClicked (struct _lv_obj_t * obj);

    lv_res_t onButBackClicked (struct _lv_obj_t * obj);

    void printMesgFunc(uint32_t retId);

    /**
     * @brief 保持文件视图状态(只生效一次)
     * @return
     */
    bool keepViewer() const;
    void setKeepViewer(bool keepViewer);

// Screen interface

protected:
    virtual bool initScreen() override;
    virtual void onLangChanged() override;
    virtual void onThemeChanged() override;

    virtual bool beforeShow() override;
    virtual void afterHide() override;
    virtual void afterCleanScreen() override;
    virtual void beforeCleanScreen() override;
};

#endif // FILESCREEN_H
