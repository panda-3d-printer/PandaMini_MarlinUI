#include "filescreen.h"
#include "marlinui.h"
#include "resources.h"
#include "multilang.h"
#include "theme.h"
#include "screentask.h"
#include <lvgl>
#include "lvslot.hpp"
#include "printingscreen.h"

#ifdef SDSUPPORT
  #include "../../sd/cardreader.h"
#endif

using std::placeholders::_1;

void FileScreen::printMesgFunc(uint32_t retId)
{
    //打印消息界面回调函数
    switch (retId)
    {
    case MR_Back:
    case MR_Cancel:
        //返回文件浏览界面
        setKeepViewer(true);
        show();
        break;
    case MR_Comfirm:
        //进入打印界面
        MarlinUi::getInstance()->printingScreen()->setPrintFileName(m_fileList.filename());
        MarlinUi::getInstance()->showPrintingScreen();
        ExtUI::printFile(m_fileList.shortFilename());
        MarlinUi::getInstance()->printingScreen()->PrintingStart();
    default:
        break;
    }
}


// 打印文件
lv_res_t FileScreen::onButItemClicked (struct _lv_obj_t * obj)
{
    LV_LOG_INFO("onButItemClicked");

    LVButton * but = static_cast<LVButton *>(lv_obj_get_free_ptr(obj));

    if(but)
    {

        LVLabel * labItem = nullptr;
        uint16_t fileIdx = m_filePage*4;

        if(but == m_butItem0)
        {
            labItem = m_labItem0;
            fileIdx += 0;
        }
        else if(but == m_butItem1)
        {
            labItem = m_labItem1;
            fileIdx += 1;
        }
        else if(but == m_butItem2)
        {
            labItem = m_labItem2;
            fileIdx += 2;
        }
        else if(but == m_butItem3)
        {
            labItem = m_labItem3;
            fileIdx += 3;
        }

        //选中点击的文件
        m_fileList.seek(fileIdx);

        if(but->getFreeNumber() == FileScreen::folder)
        {
            //打开文件夹
            if(labItem)
                openFolder(m_fileList.filename());
        }
        else
        {
            //TODO:准备提示界面
            MessageScreen * mesgScreen = MarlinUi::getInstance()->messageScreen();
            mesgScreen->setTitle(Lang_MesgScreen_LabTitle);
            char tempStr[40];
            sprintf(tempStr,"%s\n#EEEE00 %s#",tr(Lang_FileScreen_MesgOpenFile),m_fileList.filename());
            //strcpy(m_printFileName,m_fileList.shortFilename());
            mesgScreen->labMesg()->setAlign(LV_LABEL_ALIGN_CENTER);
            mesgScreen->labMesg()->setRecolor(true);
            mesgScreen->setMessage(tempStr);
            mesgScreen->setButton0(Lang_MesgScreen_ButConfirm,MR_Comfirm);
            mesgScreen->setButton1(Lang_MesgScreen_ButCancel,MR_Cancel);
            mesgScreen->setMessageFunc(std::bind(&FileScreen::printMesgFunc,this,_1));
            MarlinUi::getInstance()->showMessageScreen();
        }
    }

    return LV_RES_OK;
}

lv_res_t FileScreen::onButBackClicked(_lv_obj_t *obj)
{
    LV_LOG_INFO("onButItemClicked");

    LVButton * but = static_cast<LVButton *>(lv_obj_get_free_ptr(obj));

    if(but)
    {
        if(ExtUI::isMediaInserted())
        {
            if(!m_fileList.isAtRootDir())
            {
                backFolder();
            }
            else
            {
                return onButBackHomeClicked(obj);
            }
        }
        else
        {
            return onButBackHomeClicked(obj);
        }
    }

    return LV_RES_OK;
}


bool FileScreen::keepViewer() const
{
    return m_keepViewer;
}

void FileScreen::setKeepViewer(bool keepViewer)
{
    m_keepViewer = keepViewer;
}

FileScreen::FileScreen():Screen()
{
    //连接信号槽

    m_slotMediaInserted.setSlotFunc(std::bind(&FileScreen::reinitFilesView,this,_1));
    m_slotMediaError.setSlotFunc(std::bind(&FileScreen::reinitFilesView,this,_1));
    m_slotMediaRemoved.setSlotFunc(std::bind(&FileScreen::reinitFilesView,this,_1));

    connect(&MarlinUi::getInstance()->getSignalMediaInserted(),&m_slotMediaInserted);
    connect(&MarlinUi::getInstance()->getSignalMediaError(),&m_slotMediaError);
    connect(&MarlinUi::getInstance()->getSignalMediaRemoved(),&m_slotMediaRemoved);
}

FileScreen::~FileScreen()
{

}

void FileScreen::resetFileList()
{
    m_filePage = 0;
    m_fileList.refresh();
    //m_fileList.changeDir("/");
    m_labPath->setText("/");
    strcpy(m_dirName,"/");
    strcpy(m_folderName,"");
    //m_printFileName[0] = '\0';
}

void FileScreen:: setFileItem0(const char *name, bool isDir)
{
    setFileItem(name,isDir,
                    m_butItem0,
                    m_imgItem0,
                    m_labItem0);
}

void FileScreen:: setFileItem1(const char *name, bool isDir)
{
    setFileItem(name,isDir,
                    m_butItem1,
                    m_imgItem1,
                    m_labItem1);
}

void FileScreen:: setFileItem2(const char *name, bool isDir)
{
    setFileItem(name,isDir,
                    m_butItem2,
                    m_imgItem2,
                    m_labItem2);
}

void FileScreen:: setFileItem3(const char *name, bool isDir)
{
    setFileItem(name,isDir,
                    m_butItem3,
                    m_imgItem3,
                    m_labItem3);
}

void FileScreen::setFileItem(const char *name, bool isDir, LVButton *but, LVImage *img, LVLabel *lab)
{
    if(name != nullptr)
    {
        but->setHidden(false);
        lab->setText(name);
        img->setSrc(isDir? (&folder_30_36) : (&modelFile_30_36));
        //标识项目类型
        but->setFreeNumber(isDir?folder:file);
    }
    else
    {
        but->setHidden(true);
    }
}

uint16_t FileScreen::maxFilePage()
{
    int16_t fileNum = m_fileList.count();
    return ((fileNum%4) == 0 ? fileNum/4 : fileNum/4+1);
}

void FileScreen::preFilePage()
{
    if(m_filePage > 0)
    {
        --m_filePage;
    }
    else
    {
        m_filePage = 0;
    }
    updateFilesView();
}

void FileScreen::nextFilePage()
{
    uint16_t pageSize = maxFilePage();
    if(m_filePage <=  pageSize)
    {
        ++m_filePage;
    }
    else
    {
        m_filePage = pageSize;
    }
    updateFilesView();
}

void FileScreen::openFolder(const char *name)
{

    m_filePage = 0;

    //保存文件夹名
    strcpy(m_folderName,name);

    //保存路径
    char tempStr[strlen(m_dirName)+1];
    strcpy(tempStr,m_dirName);
    sprintf(m_dirName,m_fileList.isAtRootDir() ? "%s%s" : "%s/%s",tempStr,m_folderName);
    m_labPath->setText(m_dirName);

    m_fileList.changeDir(m_folderName);
    updateFilesView();
}

void FileScreen::backFolder()
{
    //保存路径
    m_fileList.upDir();

    if(m_fileList.isAtRootDir())
    {
        strcpy(m_dirName,"/");
        strcpy(m_folderName,"");
    }
    else
    {
        uint16_t idx = strlen(m_dirName)-strlen(m_folderName)-1;
        m_dirName[idx] = '\0';
        //获取文件夹名称
        for (int i = strlen(m_dirName); i > 0; --i)
        {
            if(m_dirName[i] == '/')
            {
                strcpy(m_folderName,&m_dirName[i+1]);
            }
        }
    }
    m_labPath->setText(m_dirName);
    m_filePage = 0;
    updateFilesView();
}

void FileScreen::updateFilesView()
{
    //TODO:实现文件视图刷新功能

    //更新文件列表
    //检查是否有存储器插入
    if(ExtUI::isMediaInserted() && m_fileList.count())
    {

        if( m_fileList.seek(m_filePage*4,false))
        {
            setFileItem0(m_fileList.filename(),m_fileList.isDir());
        }
        else
        {
            setFileItem0(nullptr,false);
        }

        if( m_fileList.seek(m_filePage*4+1,false))
        {
            setFileItem1(m_fileList.filename(),m_fileList.isDir());
        }
        else
        {
            setFileItem1(nullptr,false);
        }

        if( m_fileList.seek(m_filePage*4+2,false))
        {
            setFileItem2(m_fileList.filename(),m_fileList.isDir());
        }
        else
        {
            setFileItem2(nullptr,false);
        }

        if( m_fileList.seek(m_filePage*4+3,false))
        {
            setFileItem3(m_fileList.filename(),m_fileList.isDir());
        }
        else
        {
            setFileItem3(nullptr,false);
        }
    }
    else //没有存储器
    {
        //隐藏4个文件对象
        setFileItem0(nullptr,false);
        setFileItem1(nullptr,false);
        setFileItem2(nullptr,false);
        setFileItem3(nullptr,false);
    }


    //更新导航按钮
    butPre->setHidden(m_filePage == 0);
    butNext->setHidden(m_filePage == (maxFilePage() > 0 ? maxFilePage()-1 : 0));

    //更新页数指示器

    if(m_fileList.count())
    {
        char tempStr[16];
        sprintf(tempStr,"%d/%d",m_filePage+1,maxFilePage());
        labPageIdx->setHidden(false);
        labPageIdx->setText(tempStr);
    }
    else
    {
        labPageIdx->setHidden(true);
    }

    //媒体状态提示
    if(!ExtUI::isMediaInserted())
    {
        m_labPath->setText("No Media Insert");
    }
    else
    {
        m_labPath->setText(m_dirName);
    }
}

void FileScreen::reinitFilesView(LVSignal *)
{
    if(isInited())
    {
        //主动初始化SD卡
        #ifdef SDSUPPORT
        card.initsd();
        #endif
        //重置文件视图
        resetFileList();
        setKeepViewer(false);
        updateFilesView();
    }
}

bool FileScreen::initScreen()
{
    //TODO:初始化文件浏览界面

    const int16_t screenWidth = width();
    const int16_t screenHeight = height();
    const int16_t screendpi = dpi();

    const int16_t safeMargin = 6;
    const int16_t spacing = 6;
    const int16_t titleBarHeight = 32;
    const int16_t navigationBarHeight = 26;

    const int16_t imgYOffset = 10;

    const int16_t butFileWidth = 151;
    const int16_t butFileHeight = 75;

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
        butBack->setAction(std::bind(&FileScreen::onButBackClicked,this,_1) ,LV_BTN_ACTION_CLICK);
        LVImage * imgBack = new LVImage(butBack,nullptr);
        imgBack->setSrc(&back_24);

        //标题
        LVLabel* labTitle = new LVLabel(titleBar,nullptr);
        labTitle->setText(Lang_FileScreen_LabTitle);
        labTitle->align(butBack,LV_ALIGN_OUT_RIGHT_MID,0,0);

        //历史记录按钮
        LVButton * butHistory = new LVButton(titleBar,nullptr);
        butHistory->setSize(titleBarHeight,titleBarHeight);
        butHistory->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butHistory->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_Screen_ButtonBack));
        butHistory->align(LV_ALIGN_IN_RIGHT_MID,-safeMargin,0);
        //butHistory->setAction(LV_BTN_ACTION_CLICK,onButBackHomeClicked);
        LVImage * imgHistory = new LVImage(butHistory,nullptr);
        imgHistory->setSrc(&history_24);
        imgHistory->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));

        //路径
        m_labPath = new LVLabel(titleBar,nullptr);
        m_labPath->setAlign(LV_LABEL_ALIGN_LEFT);
        m_labPath->setLongMode(LV_LABEL_LONG_ROLL);
        m_labPath->setHeight(titleBar->getHeight());
        m_labPath->setAlign(LV_LABEL_ALIGN_CENTER);
        m_labPath->setWidth(butHistory->getX() - labTitle->getWidth() - labTitle->getX() - spacing*6);
        m_labPath->setText(m_dirName);
        m_labPath->align(labTitle,LV_ALIGN_OUT_RIGHT_TOP,spacing*2,0);
    }


    {
        //文件按钮区域
        LVObject * fileArea = new LVObject(this,nullptr);
        fileArea->align(reference,LV_ALIGN_OUT_BOTTOM_LEFT);
        fileArea->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));
        fileArea->setSize(screenWidth,screenHeight-titleBarHeight);
        reference = fileArea;

        {
            //文件按钮1
            m_butItem0 = new LVButton(fileArea,nullptr);
            m_butItem0->setLayout(LV_LAYOUT_OFF);
            m_butItem0->setSize(butFileWidth,butFileHeight);
            m_butItem0->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,safeMargin);
            m_butItem0->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            m_butItem0->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            m_butItem0->setAction(std::bind(&FileScreen::onButItemClicked,this,_1),LV_BTN_ACTION_CLICK);
            m_imgItem0 = new LVImage(m_butItem0,nullptr);
            m_imgItem0->setSrc(&folder_30_36);
            m_imgItem0->align(LV_ALIGN_IN_LEFT_MID,imgYOffset);
            m_labItem0 = new LVLabel(m_butItem0,nullptr);
            m_labItem0->align(m_imgItem0,LV_ALIGN_OUT_RIGHT_TOP,safeMargin);
            m_labItem0->setLongMode(LV_LABEL_LONG_BREAK);
            m_labItem0->setAlign(LV_LABEL_ALIGN_LEFT);
            m_labItem0->setWidth( m_butItem0->getWidth() - m_imgItem0->getWidth() - safeMargin*3 );
            m_labItem0->setHeight(m_imgItem0->getHeight());
            m_labItem0->setText("Models");
            reference = m_butItem0;

            //文件按钮2
            m_butItem1 = new LVButton(fileArea,nullptr);
            m_butItem1->setLayout(LV_LAYOUT_OFF);
            m_butItem1->setSize(butFileWidth,butFileHeight);
            m_butItem1->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing);
            m_butItem1->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            m_butItem1->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            m_butItem1->setAction(std::bind(&FileScreen::onButItemClicked,this,_1),LV_BTN_ACTION_CLICK);
            m_imgItem1 = new LVImage(m_butItem1,nullptr);
            m_imgItem1->setSrc(&modelFile_30_36);
            m_imgItem1->align(LV_ALIGN_IN_LEFT_MID,imgYOffset);
            m_labItem1 = new LVLabel(m_butItem1,nullptr);
            m_labItem1->align(m_imgItem1,LV_ALIGN_OUT_RIGHT_TOP,safeMargin);
            m_labItem1->setLongMode(LV_LABEL_LONG_BREAK);
            m_labItem1->setAlign(LV_LABEL_ALIGN_LEFT);
            m_labItem1->setWidth( m_butItem1->getWidth() - m_imgItem1->getWidth() - safeMargin*3 );
            m_labItem1->setHeight(m_imgItem1->getHeight());
            m_labItem1->setText("Models");
            reference = m_butItem1;

            //文件按钮3
            m_butItem2 = new LVButton(fileArea,nullptr);
            m_butItem2->setLayout(LV_LAYOUT_OFF);
            m_butItem2->setSize(butFileWidth,butFileHeight);
            m_butItem2->align(LV_ALIGN_IN_TOP_LEFT,safeMargin,safeMargin+butFileHeight+safeMargin);
            m_butItem2->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            m_butItem2->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            m_butItem2->setAction(std::bind(&FileScreen::onButItemClicked,this,_1),LV_BTN_ACTION_CLICK);
            m_imgItem2 = new LVImage(m_butItem2,nullptr);
            m_imgItem2->setSrc(&modelFile_30_36);
            m_imgItem2->align(LV_ALIGN_IN_LEFT_MID,imgYOffset);
            m_labItem2 = new LVLabel(m_butItem2,nullptr);
            m_labItem2->align(m_imgItem2,LV_ALIGN_OUT_RIGHT_TOP,safeMargin);
            m_labItem2->setLongMode(LV_LABEL_LONG_BREAK);
            m_labItem2->setAlign(LV_LABEL_ALIGN_LEFT);
            m_labItem2->setWidth( m_butItem2->getWidth() - m_imgItem2->getWidth() - safeMargin*3 );
            m_labItem2->setHeight(m_imgItem2->getHeight());
            m_labItem2->setText("Models");
            reference = m_butItem2;

            //文件按钮4
            m_butItem3 = new LVButton(fileArea,nullptr);
            m_butItem3->setLayout(LV_LAYOUT_OFF);
            m_butItem3->setSize(butFileWidth,butFileHeight);
            m_butItem3->align(reference,LV_ALIGN_OUT_RIGHT_MID,spacing);
            m_butItem3->setStyle(LV_BTN_STATE_PR,THEME_STYLE_GET(Theme_Screen_ButtonPr));
            m_butItem3->setStyle(LV_BTN_STATE_REL,THEME_STYLE_GET(Theme_Screen_ButtonRel));
            m_butItem3->setAction(std::bind(&FileScreen::onButItemClicked,this,_1),LV_BTN_ACTION_CLICK);
            m_imgItem3 = new LVImage(m_butItem3,nullptr);
            m_imgItem3->setSrc(&modelFile_30_36);
            m_imgItem3->align(LV_ALIGN_IN_LEFT_MID,imgYOffset);
            m_labItem3 = new LVLabel(m_butItem3,nullptr);
            m_labItem3->align(m_imgItem3,LV_ALIGN_OUT_RIGHT_TOP,safeMargin);
            m_labItem3->setLongMode(LV_LABEL_LONG_BREAK);
            m_labItem3->setAlign(LV_LABEL_ALIGN_LEFT);
            m_labItem3->setWidth( m_butItem3->getWidth() - m_imgItem3->getWidth() - safeMargin*3 );
            m_labItem3->setHeight(m_imgItem3->getHeight());
            m_labItem3->setText("Models");
            reference = m_butItem3;
        }
    }

    {
        //导航

        //m_styleNavigation = new LVStyle(&lv_style_btn_rel);
        //(*m_styleNavigation)->body.main_color

        //前一页
        butPre = new LVButton(this,nullptr);
        butPre->setSize(prePage_28_16.header.w+spacing*2,navigationBarHeight);
        butPre->align(LV_ALIGN_IN_BOTTOM_LEFT,safeMargin*2,-safeMargin*2);
        butPre->setLayout(LV_LAYOUT_OFF);
        butPre->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_screen_ButtonArea));
        butPre->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_screen_ButtonArea));
        butPre->setAction([this](_lv_obj_t *obj)->lv_res_t{
            preFilePage();
        },LV_BTN_ACTION_CLICK);
        LVImage * imgPre = new LVImage(butPre,nullptr);
        imgPre->setSrc(&prePage_28_16);
        imgPre->align(LV_ALIGN_CENTER);

        //后一页
        butNext = new LVButton(this,nullptr);
        butNext->setSize(nextPage_28_16.header.w+spacing*2,navigationBarHeight);
        butNext->align(LV_ALIGN_IN_BOTTOM_RIGHT,-safeMargin*2,-safeMargin*2);
        butNext->setLayout(LV_LAYOUT_OFF);
        butNext->setStyle(LV_BTN_STYLE_PR,THEME_STYLE_GET(Theme_screen_ButtonArea));
        butNext->setStyle(LV_BTN_STYLE_REL,THEME_STYLE_GET(Theme_screen_ButtonArea));
        butNext->setAction([this](_lv_obj_t *obj)->lv_res_t{
            nextFilePage();
        },LV_BTN_ACTION_CLICK);
        LVImage * imgNext = new LVImage(butNext,nullptr);
        imgNext->setSrc(&nextPage_28_16);
        imgNext->align(LV_ALIGN_CENTER);

        labPageIdx = new LVLabel(this,nullptr);
        labPageIdx->setText("    ");
        labPageIdx->setStyle(THEME_STYLE_GET(Theme_Screen_ButtonRel));
        labPageIdx->align(LV_ALIGN_IN_BOTTOM_MID,0,-safeMargin);

    }

    LV_LOG_INFO(__FUNCTION__);

    return true;
}

void FileScreen::onLangChanged()
{

}

void FileScreen::onThemeChanged()
{

}

bool FileScreen::beforeShow()
{
    bool ret = Screen::beforeShow();
    if(ret)
    {
        //主动初始化SD卡
        //if(!ExtUI::isMediaInserted())
        if(!keepViewer())
        {
            #ifdef SDSUPPORT
            card.initsd();
            #endif
        }

        //重置文件视图
        if(!keepViewer())//|| !ExtUI::isMediaInserted()
        {
            resetFileList();
        }
        else
        {
            setKeepViewer(false);
        }

        setKeepViewer(false);
        updateFilesView();
    }
    return ret;
}

void FileScreen::afterHide()
{
    Screen::afterHide();

    if(isInited())
    {
        //断开信号和槽

    }
}


void FileScreen::afterCleanScreen()
{
//    delete m_slotMediaInserted; m_slotMediaInserted = nullptr;
//    delete m_slotMediaError   ; m_slotMediaError = nullptr;
//    delete m_slotMediaRemoved ; m_slotMediaRemoved = nullptr;
}


void FileScreen::beforeCleanScreen()
{
}
