#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "screen.h"

#include <lvstyle.hpp>

#define SPLASH_FPS 25

class LVImage;
class SplashAnimTask;
class LVCanvas;
class LVLabel;


/**
 * @brief 启动欢迎屏
 *
 * 注意::这个屏幕只在开机时显示,然后就会清理掉
 *
 * 显示开机动画
 * 显示些关键信息
 * 触发固件升级
 */
class SplashScreen : public Screen
{
    LV_OBJECT

    friend class SplashAnimTask;
protected:
    SplashAnimTask * m_animTask = nullptr;
    int m_animFps; //!< 显示图片的帧率
    char * m_imageFile = nullptr; //!< 图片文件
    int m_duration; //!< 动画播放时长 0~0xEFFFFFFF ms ; -1 表示一直播放
    int m_startTime = 0; //!< 动画开始播放的时间

    ////L V O B J E C T //////////////////
    //LVCanvas * m_canvas = nullptr;
        //动画相关对象
    LVImage * m_imgPrinter = nullptr;
    LVImage * m_imgExtrude = nullptr;
    LVImage * m_imgPlatform = nullptr;
    LVObject * m_imgMask = nullptr;
    LVImage * m_imgObj = nullptr;
    LVLabel * m_labBrandName = nullptr;

    ////S T Y L E//////////////////////////
    LVStyle m_styleTransp;
    LVStyle styleBrand;

    ////A N I M///////////////////////
    int16_t stepFactor = 100;       //!< 动画运输细分数

    int16_t extrudeY = 7;
    int16_t extrudex = 0;
    int16_t extrudeMinX = 5;
    int16_t extrudeMaxX = 59;
    int16_t extrudeStartX = extrudeMinX;
    int16_t extrudeEndX = extrudeMaxX;
    int16_t extrudeDir = 1;
    int16_t extrudeStep = 0;

    int16_t platformY = 0;
    int16_t platformMinY = 20;
    int16_t platformMaxY = 51;
    int16_t platformStartY = platformMinY;
    int16_t platformEndY = platformMaxY;
    int16_t platformStep = 0;

    int16_t objectHeight = 0; //打印物高度
    int16_t objectStep = 0;

public:
    SplashScreen(char *imageFile , int duration = 3000, int fps = SPLASH_FPS);

    ~SplashScreen();

    /**
     * @brief 结束欢迎屏的显示
     * 与hide()效果一致
     * 同时跳转到下一个屏幕界面
     */
    void finish();

    /**
     * @brief 动画的帧数
     * @return
     */
    int animFps();

    /**
     * @brief 设置动画的帧数
     * 同时更新动画任务的时间间隔
     * @param fps
     */
    void setAnimFps(int fps);

    /**
     * @brief 开机图片路径
     * @return
     */
    char * imageFile();

    /**
     * @brief 设置要显示的图片名
     * @param file
     */
    void setImageFile( char * file );

    int duration() const;
    void setDuration(int duration);

protected:
    /**
     * @brief 初始化欢迎屏
     */
    bool initScreen() override;

    /**
     * @brief 更新动画
     */
    void updateAnim();

    /**
     * @brief 清除图片路径字符串的空间
     */
    void cleanImageFileStr();

    virtual void afterCleanScreen() override;
};

#endif // SPLASHSCREEN_H
