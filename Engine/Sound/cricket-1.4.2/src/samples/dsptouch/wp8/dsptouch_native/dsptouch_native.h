#pragma once

namespace dsptouch_native
{

    public ref class DspTouch sealed
    {
    public:
        static void Init();
        static void Update();
        static void Shutdown();
        static void Suspend();
        static void Resume();
        static void StartEffect();
        static void SetEffectParams(float x, float y);
        static void StopEffect();

    private:
        DspTouch();
    };

}
