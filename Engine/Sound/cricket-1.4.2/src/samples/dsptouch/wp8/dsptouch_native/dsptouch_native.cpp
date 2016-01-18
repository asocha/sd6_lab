#include "dsptouch_native.h"
#include "../../../DspTouch.h"

using namespace dsptouch_native;


void DspTouch::Init()
{
    dspTouchInit();
}

void DspTouch::Update()
{
    dspTouchUpdate();
}

void DspTouch::Shutdown()
{
    dspTouchShutdown();
}

void DspTouch::Suspend()
{
    dspTouchSuspend();
}

void DspTouch::Resume()
{
    dspTouchResume();
}

void DspTouch::StartEffect()
{
    dspTouchStartEffect();
}

void DspTouch::SetEffectParams(float x, float y)
{
    dspTouchSetEffectParams(x, y);
}

void DspTouch::StopEffect()
{
    dspTouchStopEffect();
}

