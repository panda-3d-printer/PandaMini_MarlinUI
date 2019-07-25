#include "extuiextend.h"

#include "../../inc/MarlinConfigPre.h"

#include "../ultralcd.h"
#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/endstops.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../libs/duration_t.h"
#include "../../HAL/shared/Delay.h"

#if HAS_FILAMENT_SENSOR
#include "../../feature/runout.h"
#endif


namespace ExtUI
{

bool powerCheckState()
{
    return
    #if ENABLED(POWER_LOSS_RECOVERY) && PIN_EXISTS(POWER_LOSS)          // Save if power loss pin is triggered
            READ(POWER_LOSS_PIN) == POWER_LOSS_STATE;
    #else
            false;
    #endif
}

bool getTriggerState(trigger_t trigger)
{

#define READ_ENDSTOP(S) (READ(S##_PIN) != S##_ENDSTOP_INVERTING)

    switch (trigger)
    {
#if HAS_X_MIN
    case XMIN:   return READ_ENDSTOP(X_MIN);   break;
#endif
#if HAS_X_MAX
    case XMAX:   return READ_ENDSTOP(X_MAX);   break;
#endif
#if HAS_Y_MIN
    case YMIN:   return READ_ENDSTOP(Y_MIN);   break;
#endif
#if HAS_Y_MAX
    case YMAX:   return READ_ENDSTOP(Y_MAX);   break;
#endif
#if HAS_Z_MIN
    case ZMIN:   return READ_ENDSTOP(Z_MIN);   break;
#endif
#if HAS_Z_MAX
    case ZMAX:   return READ_ENDSTOP(Z_MAX);   break;
#endif
#if HAS_X2_MIN
    case X2MIN:  return READ_ENDSTOP(X2_MIN);  break;
#endif
#if HAS_X2_MAX
    case X2MAX:  return READ_ENDSTOP(X2_MAX);  break;
#endif
#if HAS_Y2_MIN
    case Y2MIN:  return READ_ENDSTOP(Y2_MIN);  break;
#endif
#if HAS_Y2_MAX
    case Y2MAX:  return READ_ENDSTOP(Y2_MAX);  break;
#endif
#if HAS_Z2_MIN
    case Z2MIN:  return READ_ENDSTOP(Z2_MIN);  break;
#endif
#if HAS_Z2_MAX
    case Z2MAX:  return READ_ENDSTOP(Z2_MAX);  break;
#endif
#if HAS_Z3_MIN
    case Z3MIN:  return READ_ENDSTOP(Z3_MIN);  break;
#endif
#if HAS_Z3_MAX
    case Z3MAX:  return READ_ENDSTOP(Z3_MAX);  break;
#endif

#if HAS_FILAMENT_SENSOR
    case E0DET:  return FilamentSensorBase::poll_runout_states()>>0&0x1;  break;
    case E1DET:  return FilamentSensorBase::poll_runout_states()>>1&0x1;  break;
    case E2DET:  return FilamentSensorBase::poll_runout_states()>>2&0x1;  break;
    case E3DET:  return FilamentSensorBase::poll_runout_states()>>3&0x1;  break;
    case E4DET:  return FilamentSensorBase::poll_runout_states()>>4&0x1;  break;
    case E5DET:  return FilamentSensorBase::poll_runout_states()>>5&0x1;  break;
#endif

#if HAS_Z_MIN_PROBE_PIN
    case ZPROBE:  return READ_ENDSTOP(Z_MIN_PROBE);   break;
#endif
    case POWERCK: return powerCheckState();                               break;
    default: return false;
    }
}

bool getEndstopState(trigger_t trigger) { return getTriggerState(trigger); }

bool getFilamentDetectState(trigger_t trigger) { return getTriggerState(trigger); }

bool getZProbeState() { return getTriggerState(ZPROBE); }

bool getPowerCheckState() { return getTriggerState(POWERCK); }

}
