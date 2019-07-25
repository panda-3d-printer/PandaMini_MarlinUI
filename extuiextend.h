#ifndef EXTUIEXTEND_H
#define EXTUIEXTEND_H

//由于marlin 的extui中函数并不全面,需要自行扩展一些功能

//#undef max
//#undef min

#include <ui_api.h>


namespace ExtUI {

    /**
     * @brief 开关触发器
     */
    enum trigger_t     : uint16_t { XMIN, XMAX , YMIN , YMAX , ZMIN , ZMAX ,         // xyz的正负限位触发器
                                   X2MIN, X2MAX , Y2MIN , Y2MAX , Z2MIN , Z2MAX , Z3MIN , Z3MAX ,   // 双XYZ模式的限位触发器
                                   E0DET , E1DET , E2DET ,E3DET , E4DET , E5DET ,  // 挤出机断料检测触发器
                                   ZPROBE ,                                         // 自动调平探针触发器
                                   POWERCK                                          // 电源检测触发器
                                 };


    /**
     * @brief 获取触发器状态
     * @param trigger
     * @return true:触发状态 ; false:未触发状态
     */
    bool getTriggerState(trigger_t trigger);

    bool getEndstopState(trigger_t trigger);
    bool getFilamentDetectState(trigger_t trigger);
    bool getZProbeState();
    bool getPowerCheckState();


}

#endif // EXTUIEXTEND_H
