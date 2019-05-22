#include <lvgl.h>
#include <lvgl/lv_core/lv_obj.h>
#include <lvgl/lv_misc/lv_gc.h>

/**
 * @brief 遍历通知样式已经改变
 * @param obj
 */
void styleChanged(lv_obj_t * obj)
{
    lv_obj_t * i;
    LL_READ(obj->child_ll, i) {
        i->signal_func(i, LV_SIGNAL_STYLE_CHG, NULL);

        styleChanged(i);
    }
}

/**
 * @brief 遍历通知主题已经改变
 */
void notificThemeChanged()
{
    lv_obj_t * i;
    LL_READ(LV_GC_ROOT(_lv_scr_ll), i) {
        i->signal_func(i, LV_SIGNAL_STYLE_CHG, NULL);

        styleChanged(i);
    }

    styleChanged(lv_scr_act());
}



