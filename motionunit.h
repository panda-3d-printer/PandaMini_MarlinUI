#ifndef MOTIONUNIT_H
#define MOTIONUNIT_H

/**
 * @brief 运动单位
 */
enum MotionUnit
{
    MU_05mm,
    MU_1mm ,
    MU_10mm,
    MU_50mm,
};

/**
 * @brief 获取枚举单位代表的数值
 * @param mu
 * @return
 */
float getUnit(MotionUnit mu);

#endif // MOTIONUNIT_H
