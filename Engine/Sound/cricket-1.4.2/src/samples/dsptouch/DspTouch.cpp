#include "DspTouch.h"
#include "ck/ck.h"
#include "ck/bank.h"
#include "ck/sound.h"
#include "ck/effect.h"
#include "ck/effectparam.h"
#include "ck/effectbus.h"

#include <stddef.h>

namespace
{
    CkBank* g_bank = NULL;
    CkSound* g_sound = NULL;
    CkEffect* g_effect = NULL;
}

#ifdef CK_PLATFORM_ANDROID
void dspTouchInit(JNIEnv* env, jobject activity)
#else
void dspTouchInit()
#endif
{
#ifdef CK_PLATFORM_ANDROID
    CkConfig cfg(env, activity);
#else
    CkConfig cfg;
#endif
    CkInit(&cfg);

    g_bank = CkBank::newBank("dsptouch.ckb");
    g_sound = CkSound::newBankSound(g_bank, "hiphoppiano");
    g_sound->setLoopCount(-1);
    g_sound->play();

    g_effect = CkEffect::newEffect(kCkEffectType_BitCrusher);
//    g_effect = CkEffect::newEffect(kCkEffectType_RingMod);
//    g_effect = CkEffect::newEffect(kCkEffectType_Distortion);

    g_effect->setBypassed(true);
    CkEffectBus::getGlobalEffectBus()->addEffect(g_effect);
}

void dspTouchUpdate()
{
    CkUpdate();
}

void dspTouchShutdown()
{
    g_sound->destroy();
    g_bank->destroy();
    g_effect->destroy();

    CkShutdown();
}

void dspTouchSuspend()
{
    CkSuspend();
}

void dspTouchResume()
{
    CkResume();
}

void dspTouchStartEffect()
{
    g_effect->setBypassed(false);
}

void dspTouchSetEffectParams(float x, float y)
{
    g_effect->setParam(kCkBitCrusherParam_BitResolution, x*25.0f);
    g_effect->setParam(kCkBitCrusherParam_HoldMs, y*2.0f);

//    g_effect->setParam(kCkRingModParam_Freq, x*1000.0f);

//    g_effect->setParam(kCkDistortionParam_Drive, x*10.0f);
//    g_effect->setParam(kCkDistortionParam_Offset, y);
}

void dspTouchStopEffect()
{
    g_effect->setBypassed(true);
}
