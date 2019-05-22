#include "multilang.h"
#include "lvgl/lv_misc/lv_log.h"


const char *tr(uint16_t textId)
{
    return (const char *)lv_lang_get_text(textId);
}

/**
 * @brief 实现文本动态切换的重要函数
 * @param textId
 */
const void * lvLangTextGet(uint16_t textId)
{
    LangID langId = (LangID)lv_lang_act();

    MultiLang * mLangs = MultiLang::getInstance();


    LangBase * lang = nullptr;

    //检查对应的语言是否存在
    if(mLangs->hasLang(langId))
        lang = mLangs->lang(langId);
    else
        lang = mLangs->defaultLang();

    if(lang)
        return (const void *)lang->getText(textId);
    else
        return "NullString";
}

MultiLang::MultiLang()
{
    //初始化语言列表
    m_lings = new LangBase*[LANG_END];
    for (int i = 0; i < LANG_END; ++i)
    {
        m_lings[i] = nullptr;
    }

    //注册语言切换函数
    setTextFunc(lvLangTextGet);
}

MultiLang *MultiLang::getInstance()
{
    static MultiLang * instance = nullptr;

    if(!instance)
    {
        instance = new MultiLang;
    }

    return instance;

}

bool MultiLang::hasLang(LangID id)
{
    return m_lings[id] != nullptr;
}

LangBase *MultiLang::lang(LangID id)
{
    return m_lings[id];
}

LangBase *MultiLang::defaultLang()
{
    return m_lings[m_defaultLang];
}



void MultiLang::loginLang(LangID langId,LangBase * lang)
{
    //TODO:注册语言

    if(langId > LANG_END)
    {
        LV_LOG_WARN("multilang::loginLang langId > LANG_END");
        return;
    }

    m_lings[langId] = lang;
}

void MultiLang::logoutLang(LangID langId)
{
    //TODO:注销语言

    if(langId > LANG_END)
    {
        LV_LOG_WARN("multilang::logoutLang langId > LANG_END");
        return;
    }

    m_lings[langId] = nullptr;
}

//获取对应的语言文本
LangBase::LangBase(LangID id)
{
    //注册语言对象
    MultiLang::getInstance()->loginLang(id,this);
}

MultiLang::~MultiLang()
{
    delete[] m_lings;
}
