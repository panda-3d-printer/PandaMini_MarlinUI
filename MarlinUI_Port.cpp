#include <Arduino.h>
#include <lcd.h>
#include <touch.h>

///////////LVGL /////////////////
#include "lvgl/lvgl.h"
#include "lv_port.h"
#include <lvapplication.h>
#include <lvtheme.h>
#include <lvgl>
#include <marlinui.h>
#include <SerialFlash.h>

void memory_monitor(void*)
{}

void marlinUiInit(void)
{
	/***********************
     * Initialize PandaScreen
     ***********************/

	lcddev.Init();		//初始化屏幕
	tp_dev.Init();		//触摸屏初始化
	lcddev.Display_Dir(1); //横屏

	SerialFlash.begin(PIN_SPI2_SS); //初始化图形库用到的SPIFlash资源

	/***********************
     * Initialize LittlevGL
     ***********************/
    lv_init();

    /***********************
     * Tick interface
     ***********************/
    /* Initialize a Timer for 1 ms period and
     * in its interrupt call
     * lv_tick_inc(1); */
    //TIM2_Init(10-1,9000-1);
    /***********************
     * Display interface
     ***********************/
    /* your_display_init();*/                       /*Initialize your display*/

    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/
#if LV_VDB_SIZE != 0
    disp_drv.disp_flush = ex_disp_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
#else
    disp_drv.disp_fill = ex_disp_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    disp_drv.disp_map = ex_disp_map;                /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
#endif

#if USE_LV_GPU
    /*Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)*/
    disp_drv.mem_blend = ex_mem_blend;              /*Blend two color array using opacity*/
    disp_drv.mem_fill = ex_mem_fill;                /*Fill a memory array with a color*/
#endif

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);


    /*************************
     * Input device interface
     *************************/
    /*Add a touchpad in the example*/
    /*touchpad_init();*/                            /*Initialize your touchpad*/
    lv_indev_drv_t indev_drv;                       /*Descriptor of an input device driver*/
    lv_indev_drv_init(&indev_drv);                  /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;         /*The touchpad is pointer type device*/
    indev_drv.read = ex_tp_read;                 /*Library ready your touchpad via this function*/
    lv_indev_drv_register(&indev_drv);              /*Finally register the driver*/

    /*************************************
     * Run the task handler of LittlevGL
     *************************************/

    MarlinUi * marlinUi = MarlinUi::getInstance();
	marlinUi->initMarlinUI();
	marlinUi->showSplashScreen();

	//测试动态语言切换///////////////////////////////
	//static LVTask virtualLangChangTask(10000);
	//virtualLangChangTask.setTaskFunc([]()
	//{
	//	uint16_t langId = LVLang::langActived();
	//	LVLang::langSet((langId + 1) % 2 );
	//	langId = LVLang::langActived();
	//	LV_LOG_INFO("LangChang :");
	//	LV_LOG_INFO((langId == 0 ? "chinese" : "english"));
	//});
	//virtualLangChangTask.start();
}

void marlinUiRun()
{
	lv_task_handler();
}
