#include "splashscreen.h"
#include "screentask.h"
#include "marlinui.h"
#include <lvimage.hpp>
#include <lvgl/lv_hal/lv_hal_tick.h>
#include <lvgl>
#include "theme.h"
#include "resources.h"

void draw3DBox();

/**
 * @brief 欢迎动画任务
 * 播放一个动画图片
 * 每秒22帧
 */
class SplashAnimTask : public ScreenTask
{
    LV_MEMAORY_FUNC
public:
    SplashAnimTask(SplashScreen * screen)
        :ScreenTask(screen)
    {
        if(screen)
            setFps(screen->animFps());
    }

    /**
     * @brief 设置动画播放速率
     */
    void setFps(int fps)
    {
        setPeriod( 1000 / fps);
    }

protected:
    /**
     * @brief 执行动画播放任务
     */
    virtual void run() override
    {
        //TODO: 实现动画任务
        SplashScreen * splash = static_cast<SplashScreen*>(screen());
        if(splash)
            splash->updateAnim();
    }
};


int SplashScreen::duration() const
{
    return m_duration;
}

void SplashScreen::setDuration(int duration)
{
    m_duration = duration;
}

SplashScreen::SplashScreen(char * image , int duration, int fps)
    :Screen()
{
    setImageFile(image);
    setAnimFps(fps);
    setDuration(duration);

    //初始化动画任务
    m_animTask = new SplashAnimTask(this);

    //隐藏后自动删除
    //setDeleteAfterHide(true);

    LV_LOG_INFO("SplashScreen create");
}

SplashScreen::~SplashScreen()
{
    cleanImageFileStr();

    delete m_animTask;

    //delete m_canvas;

    LV_LOG_INFO("SplashScreen delete");
}

void SplashScreen::finish()
{
    hide();
    nextScreen();
}

int SplashScreen::animFps()
{
    return  m_animFps;
}

void SplashScreen::setAnimFps(int fps)
{
    m_animFps = fps;
    if(m_animTask)
        m_animTask->setFps(fps);
}

char *SplashScreen::imageFile()
{
    return m_imageFile;
}

void SplashScreen::setImageFile(char *file)
{
    cleanImageFileStr();

    if(!m_imageFile)
        if(strlen(file))
        {
            m_imageFile = new char[strlen(file)];
            strcpy(m_imageFile,file);
        }
}

void SplashScreen::cleanImageFileStr()
{
    if(m_imageFile)
        delete [] m_imageFile;

    m_imageFile = nullptr;
}

//static uint16_t buf[320*240];

bool SplashScreen::initScreen()
{
    //TODO: 实现欢迎屏界面布局

    //计算动画运动值
    extrudeY = 7;
    extrudeMinX = 5;
    extrudeMaxX = 59 - splash_extrude_20_24.header.w;
    extrudeStartX = (splash_printer_64_68.header.w/2 - splash_obj_28_16.header.w ) * stepFactor;// - splash_extrude_20_24.header.w/2;
    extrudeEndX = (splash_printer_64_68.header.w/2 ) * stepFactor ;// + splash_extrude_20_24.header.w/2;
    extrudex = extrudeStartX;

    extrudeDir = 1;
    extrudeStep = splash_obj_28_16.header.h*stepFactor / m_animFps * 4;

    objectHeight = 0;

    platformMinY = 29;
    platformMaxY = 48;
    platformStartY = platformMinY;
    platformEndY = platformMinY+splash_obj_28_16.header.h;
    platformStep = splash_obj_28_16.header.h * stepFactor / (m_animFps * ((m_duration-1000)/1000.0));

    platformY = platformStartY * stepFactor;

    this->setStyle(THEME_STYLE_GET(Theme_screen_ButtonArea));

    m_styleTransp = &lv_style_plain;
    m_styleTransp->body.empty = true; //按钮全透明
    m_styleTransp->body.border.width = 0;

    m_imgPrinter = new LVImage(this,nullptr);
    m_imgPrinter->setSrc(&splash_printer_64_68);
    m_imgPrinter->align(LV_ALIGN_CENTER); //,0,-splash_printer_64_68.header.h/3
    m_imgPrinter->setStyle(THEME_STYLE_GET(Theme_screen_Cyan));

    m_imgExtrude = new LVImage(m_imgPrinter,nullptr);
    m_imgExtrude->setSrc(&splash_extrude_20_24);
    m_imgExtrude->align(LV_ALIGN_IN_TOP_LEFT,extrudeStartX,extrudeY);
    m_imgExtrude->setStyle(THEME_STYLE_GET(Theme_screen_Blue));

    m_imgPlatform = new LVImage(m_imgPrinter,nullptr);
    m_imgPlatform->setSrc(&splash_platform_64_3);
    m_imgPlatform->align(LV_ALIGN_IN_TOP_MID,0,platformStartY);
    m_imgPlatform->setStyle(THEME_STYLE_GET(Theme_screen_Green));

    m_imgMask = new LVObject(m_imgPrinter,nullptr);
    m_imgMask->setSize(m_imgPrinter->getWidth(),0);
    m_imgMask->align(m_imgPlatform,LV_ALIGN_OUT_TOP_MID,0,0);
    m_imgMask->setStyle(m_styleTransp);

    m_imgObj = new LVImage(m_imgMask,nullptr);
    m_imgObj->setSrc(&splash_obj_28_16);
    m_imgObj->align(LV_ALIGN_IN_BOTTOM_MID);
    m_imgObj->setStyle(THEME_STYLE_GET(Theme_screen_Red));

    styleBrand = THEME_STYLE_GET(Theme_screen_ButtonArea);
    styleBrand->text.font = &lv_font_dejavu_30;

    m_labBrandName = new LVLabel(this,nullptr);
    m_labBrandName->setStyle(styleBrand);
    m_labBrandName->setStaticText("Panda mini V2.2");
    m_labBrandName->setAlign(LV_LABEL_ALIGN_CENTER);
    m_labBrandName->align(m_imgPrinter,LV_ALIGN_OUT_BOTTOM_MID,0,30);

    //显示一个3D线框
//    LVCanvas * canvas = new LVCanvas(this,nullptr);
//    canvas->setSize(width(),height());
//    canvas->align(LV_ALIGN_CENTER);
//    canvas->setBuffer(buf,320,240,LV_IMG_CF_TRUE_COLOR);
//    m_canvas = canvas;

    LV_LOG_INFO(__FUNCTION__);

    return true;

}



void SplashScreen::updateAnim()
{
    //TODO: 更新动画
    if(m_startTime == 0)
    {
        m_startTime = lv_tick_get();
       LV_LOG_INFO("SplashScreen::updateAnim()");
    }

    platformY += platformStep;
    objectHeight  += platformStep;
    if(platformY >= platformMaxY*stepFactor)
        platformY = platformMaxY*stepFactor;
    else
    {
        extrudex += extrudeStep * extrudeDir;
        if(extrudex >= extrudeEndX)
            extrudeDir = -1;
        else if(extrudex <= extrudeStartX)
            extrudeDir = 1;
    }


    //3D打印机动画
    m_imgExtrude->align(LV_ALIGN_IN_TOP_LEFT,extrudex/stepFactor,extrudeY);
    m_imgPlatform->align(LV_ALIGN_IN_TOP_MID,0,platformY/stepFactor);
    m_imgMask->setHeight(objectHeight/stepFactor);
    m_imgMask->realign();
    m_imgObj->realign();


//    m_canvas->dr(0,0,LV_COLOR_WHITE,LV_COLOR_WHITE);
//    //清屏
//    for (int i = 0; i < 320*240; ++i)
//    {
//        buf[i] = 0xffffff;
//    }
//    //绘制3D线框
//    draw3DBox();
//    m_canvas->invalidate();

    //检查播放时间
    if(m_duration != -1 && (m_startTime + m_duration) < lv_tick_get())
    {
        //停止播放动画
        finish();
    }
}


void SplashScreen::afterCleanScreen()
{
    //清理数据
}


//// define a 3d point structure
//struct pt3d
//{
//  int16_t x, y, z;
//};

//struct surface
//{
//  uint8_t p[4];
//  int16_t z;
//};

//struct pt2d
//{
//  int16_t x, y;
//  unsigned is_visible;
//};


//// define the point at which the observer looks, 3d box will be centered there
//#define MX (MarlinUi::getInstance()->width()/2)
//#define MY (MarlinUi::getInstance()->height()/2)

//// define a value that corresponds to "1"
//#define U 160

//// eye to screen distance (fixed)
//#define ZS U

//// cube edge length is 2*U
//struct pt3d cube[8] =
//{
//  { -U, -U,  U },
//  {  U, -U,  U },
//  {  U, -U, -U },
//  { -U, -U, -U },
//  { -U,  U,  U },
//  {  U,  U,  U },
//  {  U,  U, -U },
//  { -U,  U, -U },
//};

//// define the surfaces
//struct surface cube_surface[6] =
//{
//  { {0, 1, 2, 3}, 0 },	// bottom
//  { {4, 5, 6, 7}, 0 },	// top
//  { {0, 1, 5, 4}, 0 },	// back

//  { {3, 7, 6, 2}, 0 },	// front
//  { {1, 2, 6, 5}, 0 },	// right
//  { {0, 3, 7, 4}, 0 },	// left
//};

//// define some structures for the copy of the box, calculation will be done there
//struct pt3d cube2[8];
//struct pt2d cube_pt[8];

//// will contain a rectangle border of the box projection into 2d plane
//int16_t x_min, x_max;
//int16_t y_min, y_max;

//int16_t sin_tbl[65] = {
//0,1606,3196,4756,6270,7723,9102,10394,11585,12665,13623,14449,15137,15679,16069,16305,16384,16305,16069,15679,
//15137,14449,13623,12665,11585,10394,9102,7723,6270,4756,3196,1606,0,-1605,-3195,-4755,-6269,-7722,-9101,-10393,
//-11584,-12664,-13622,-14448,-15136,-15678,-16068,-16304,-16383,-16304,-16068,-15678,-15136,-14448,-13622,-12664,-11584,-10393,-9101,-7722,
//-6269,-4755,-3195,-1605,0};

//int16_t cos_tbl[65] = {
//16384,16305,16069,15679,15137,14449,13623,12665,11585,10394,9102,7723,6270,4756,3196,1606,0,-1605,-3195,-4755,
//-6269,-7722,-9101,-10393,-11584,-12664,-13622,-14448,-15136,-15678,-16068,-16304,-16383,-16304,-16068,-15678,-15136,-14448,-13622,-12664,
//-11584,-10393,-9101,-7722,-6269,-4755,-3195,-1605,0,1606,3196,4756,6270,7723,9102,10394,11585,12665,13623,14449,
//15137,15679,16069,16305,16384};


//void copy_cube(void)
//{
//  uint8_t i;
//  for( i = 0; i < 8; i++ )
//  {
//    cube2[i] = cube[i];
//  }
//}

//void rotate_cube_y(uint16_t w)
//{
//  uint8_t i;
//  int16_t x, z;
//  /*
//    x' = x * cos(w) + z * sin(w)
//    z' = - x * sin(w) + z * cos(w)
//  */
//  for( i = 0; i < 8; i++ )
//  {
//    x = ((int32_t)cube2[i].x * (int32_t)cos_tbl[w] + (int32_t)cube2[i].z * (int32_t)sin_tbl[w])>>14;
//    z = (- (int32_t)cube2[i].x * (int32_t)sin_tbl[w] + (int32_t)cube2[i].z * (int32_t)cos_tbl[w])>>14;
//    //printf("%d: %d %d --> %d %d\n", i, cube2[i].x, cube2[i].z, x, z);
//    cube2[i].x = x;
//    cube2[i].z = z;
//  }
//}

//void rotate_cube_x(uint16_t w)
//{
//  uint8_t i;
//  int16_t y, z;
//  for( i = 0; i < 8; i++ )
//  {
//    y = ((int32_t)cube2[i].y * (int32_t)cos_tbl[w] + (int32_t)cube2[i].z * (int32_t)sin_tbl[w])>>14;
//    z = (- (int32_t)cube2[i].y * (int32_t)sin_tbl[w] + (int32_t)cube2[i].z * (int32_t)cos_tbl[w])>>14;
//    cube2[i].y = y;
//    cube2[i].z = z;
//  }
//}

//void trans_cube(uint16_t z)
//{
//  uint8_t i;
//  for( i = 0; i < 8; i++ )
//  {
//    cube2[i].z += z;
//  }
//}

//void reset_min_max(void)
//{
//  x_min =  0x07fff;
//  y_min =  0x07fff;
//  x_max = -0x07fff;
//  y_max = -0x07fff;
//}

//// calculate xs and ys from a 3d value
//void convert_3d_to_2d(struct pt3d *p3, struct pt2d *p2)
//{
//  int32_t t;
//  p2->is_visible = 1;
//  if ( p3->z >= ZS )
//  {
//    t = ZS;
//    t *= p3->x;
//    t <<=1;
//    t /= p3->z;
//    if ( t >= -MX && t <= MX-1 )
//    {
//      t += MX;
//      p2->x = t;

//      if ( x_min > t )
//    x_min = t;
//      if ( x_max < t )
//    x_max = t;

//      t = ZS;
//      t *= p3->y;
//      t <<=1;
//      t /= p3->z;
//      if ( t >= -MY && t <= MY-1 )
//      {
//    t += MY;
//    p2->y = t;

//    if ( y_min > t )
//      y_min = t;
//    if ( y_max < t )
//      y_max = t;
//      }
//      else
//      {
//    p2->is_visible = 0;
//      }
//    }
//    else
//    {
//      p2->is_visible = 0;
//    }
//  }
//  else
//  {
//    p2->is_visible = 0;
//  }
//}

//void convert_cube(void)
//{
//  uint8_t i;
//  reset_min_max();
//  for( i = 0; i < 8; i++ )
//  {
//    convert_3d_to_2d(cube2+i, cube_pt+i);
//    //printf("%d: %d %d\n", i, cube_pt[i].x, cube_pt[i].y);
//  }
//}

//void calculate_z(void)
//{
//  uint8_t i, j;
//  uint16_t z;
//  for( i = 0; i < 6; i++ )
//  {
//    z = 0;
//    for( j = 0; j < 4; j++ )
//    {
//      z+=cube2[cube_surface[i].p[j]].z;
//    }
//    z/=4;
//    cube_surface[i].z = z;
//    //printf("%d: z=%d\n", i, z);
//  }
//}

//void draw_cube(void)
//{
//  uint8_t i, ii;
//  uint8_t skip_cnt = 3;		/* it is known, that the first 3 surfaces are invisible */
//  int16_t z, z_upper;


//  z_upper = 32767;
//  for(;;)
//  {
//    ii = 6;
//    z = -32767;
//    for( i = 0; i < 6; i++ )
//    {
//      if ( cube_surface[i].z <= z_upper )
//      {
//    if ( z < cube_surface[i].z )
//    {
//      z = cube_surface[i].z;
//      ii = i;
//    }
//      }
//    }

//    if ( ii >= 6 )
//      break;
//    //printf("%d z=%d upper=%d\n", ii, z, z_upper);
//    z_upper = cube_surface[ii].z;
//    cube_surface[ii].z++;

//    if ( skip_cnt > 0 )
//    {
//      skip_cnt--;
//    }
//    else
//    {
//        lv_point_t rect[4] = {
//            {cube_pt[cube_surface[ii].p[0]].x, cube_pt[cube_surface[ii].p[0]].y},
//            {cube_pt[cube_surface[ii].p[1]].x, cube_pt[cube_surface[ii].p[1]].y},
//            {cube_pt[cube_surface[ii].p[2]].x, cube_pt[cube_surface[ii].p[2]].y},
//            {cube_pt[cube_surface[ii].p[3]].x, cube_pt[cube_surface[ii].p[3]].y}};

//        m_canvas->drawRectangle(rect,LV_COLOR_MAKE(((ii+1)&1)*255,(((ii+1)>>1)&1)*255,(((ii+1)>>2)&1)*255));

//    }
//  }
//}



//void calc_and_draw(uint16_t w, uint16_t v)
//{
//  copy_cube();
//  rotate_cube_y(w);
//  rotate_cube_x(v);
//  trans_cube(U*8);
//  convert_cube();
//  calculate_z();
//  draw_cube();
//}


//uint16_t w = 0;
//uint16_t v = 0;

////void loop(void)
////{
////  calc_and_draw(w, v>>3);

////  v+=3;
////  v &= 511;

////  w++;
////  w &= 63;
////  delay(30);

////  ucg.setColor(0,0,0);
////  ucg.drawBox(x_min, y_min, x_max-x_min+1, y_max-y_min+1);
////}

///**
// * @brief 绘制一个3D的立方体
// */
//void draw3DBox()
//{
//  calc_and_draw(w, v>>3);

//  v++;
//  v &= 511;

//  w++;
//  w &= 63;
//}


