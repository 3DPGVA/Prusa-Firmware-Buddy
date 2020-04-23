#pragma once

#ifdef __cplusplus
    #include <inttypes.h>
    #include "cmsis_os.h"

class Loadcell {
public:
    Loadcell();

    void Tare(int tareCount = 4);

    void SetScale(float scale);

    float GetScale() const;

    void SetThreshold(float threshold);

    float GetThreshold() const;

    void SetHysteresis(float hysteresis);

    float GetHysteresis() const;

    void ProcessSample(int32_t loadcellRaw);

    bool GetMinZEndstop() const;

    bool GetMaxZEndstop() const;

    void SetTriggerZMaxOnInactiveZMin(bool enabled);

    void ConfigureSignalEvent(osThreadId threadId, int32_t signal);

    // return loadcell load in grams
    float GetLoad() const;

    int32_t GetRawValue() const;

    bool IsSignalConfigured() const;

    void SetHighPrecisionEnabled(bool enable);

    bool IsHighPrecisionEnabled() const;

    void SetFailsOnLoadAbove(float failsOnLoadAbove);

    float GetFailsOnLoadAbove() const;

    void SetFailsOnLoadBelow(float failsOnLoadBelow);

    float GetFailsOnLoadBelow() const;

    class IFailureEnforcer {
    protected:
        Loadcell &lcell;
        float oldErrThreshold;
        IFailureEnforcer(Loadcell &lcell, float oldErrThreshold);
        IFailureEnforcer(const IFailureEnforcer &) = delete;
        IFailureEnforcer(IFailureEnforcer &&) = default;
    };

    class FailureOnLoadAboveEnforcer : public IFailureEnforcer {
    public:
        FailureOnLoadAboveEnforcer(Loadcell &lcell, float grams);
        FailureOnLoadAboveEnforcer(FailureOnLoadAboveEnforcer &&) = default;
        ~FailureOnLoadAboveEnforcer();
    };

    class FailureOnLoadBelowEnforcer : public IFailureEnforcer {
    public:
        FailureOnLoadBelowEnforcer(Loadcell &lcell, float grams);
        FailureOnLoadBelowEnforcer(FailureOnLoadBelowEnforcer &&) = default;
        ~FailureOnLoadBelowEnforcer();
    };

    FailureOnLoadAboveEnforcer CreateLoadAboveErrEnforcer(float grams = 500);

    FailureOnLoadBelowEnforcer CreateLoadBelowErrEnforcer(float grams = -3000);

private:
    bool triggerZmaxOnInactiveZmin;
    float scale;
    float threshold;
    float hysteresis;
    float failsOnLoadAbove;
    float failsOnLoadBelow;
    int32_t offset;
    osThreadId threadId;
    int32_t signal;
    int32_t loadcellRaw;
    bool endstop;
    bool isSignalEventConfigured;
    bool highPrecision;
};

    #define EXTERN_C extern "C"
extern Loadcell loadcell;

#else
    #define EXTERN_C
#endif

#include <stdbool.h>

EXTERN_C bool loadcell_get_min_z_endstop();
EXTERN_C bool loadcell_get_max_z_endstop();
