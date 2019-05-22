//#ifndef EVENT_H
//#define EVENT_H

//#include <lvlinklist.h>
//#include <functional>

//class EvenHandelerList;

///**
// * 定义事件处理器
// * 可以是函数
// * 可以是成员函数
// * 可以是拉姆达表达式
// */
//using EvenFunc = std::function<void(void)>;

///**
// * @brief 事件处理器
// */
//class EvenHandeler
//{
//    friend class EvenHandelerList;
//private:
//    LVLLNode * m_nodeId = nullptr; //!< 事件处理器的节点指针
//    EvenHandelerList * m_listPtr = nullptr; //!< 事件处理器所属的处理器列表
//    EvenFunc m_handeler; //!< 处理器的执行的函数对象
//public:
//    EvenHandeler(const EvenFunc & handeler)
//        :m_handeler(handeler)
//    {
//    }

//    ~EvenHandeler()
//    {
//        logout();
//    }

//    /**
//     * @brief exec
//     */
//    void exec()
//    {
//        m_handeler();
//    }

//    /**
//     * @brief 注册处理器
//     */
//    void login(EvenHandelerList * list = nullptr);

//    /**
//     * @brief 注销处理器
//     */
//    void logout();
//};

///**
// * @brief 事件处理器列表
// */
//class EvenHandelerList
//{
//private:
//    LVLinkList m_list; //!< 处理器列表
//public:
//    EvenHandelerList()
//        :m_list(sizeof(EvenHandeler *))
//    {}

//    ~EvenHandelerList()
//    {
//        clear();
//    }

//    /**
//     * @brief 执行处理器列表中的处理器
//     */
//    void exec()
//    {
//        //遍历处理器并执行
//        LVLLNode * node = m_list.head();
//        while (node)
//        {
//            ((EvenHandeler *)node)->exec();
//            node = m_list.next(node);
//        }
//    }

//    /**
//     * @brief 注册事件处理器
//     * @param handeler
//     * @return
//     */
//    void login(EvenHandeler * handeler)
//    {
//        LVLLNode * newNode = m_list.insertTail();
//        memcpy(newNode,handeler,sizeof(EvenHandeler *));
//        handeler->m_nodeId = newNode;
//        handeler->m_listPtr = this;
//    }

//    /**
//     * @brief 注销事件处理器
//     * @param handeler
//     */
//    void logout(EvenHandeler * handeler)
//    {
//        if(handeler && handeler->m_nodeId)
//        {
//            m_list.remove(handeler->m_nodeId);
//            handeler->m_nodeId = nullptr;
//        }
//    }

//    /**
//     * @brief 清空处理器列表
//     */
//    void clear()
//    {
//        LVLLNode * node = m_list.head();
//        while (node)
//        {
//            logout(((EvenHandeler *)node));
//            node = m_list.next(node);
//        }
//        m_list.clear();
//    }
//};

//#endif // EVENT_H
