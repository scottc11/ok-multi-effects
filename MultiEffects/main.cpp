#include "daisy_patch_sm.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace patch_sm;

#define POT_A CV_3
#define POT_B CV_4
#define POT_C CV_1
#define POT_D CV_2

DaisyPatchSM patch;

void __attribute__((optimize("O0"))) AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    patch.ProcessAnalogControls();

    float output_level = patch.GetAdcValue(POT_A);
    float gain = patch.GetAdcValue(POT_C) * 100 + 1.2;

    for(size_t i = 0; i < size; i++)
    {
        for (int chan = 0; chan < 2; chan++)
        {
            float input = in[chan][i] * output_level;
            float output = input * gain;
            out[chan][i] = SoftClip(output) + input;
        }
    }
}


int main(void)
{
    float sampleRate;
    patch.Init();
    sampleRate = patch.AudioSampleRate();

    patch.StartAdc();
    patch.StartAudio(AudioCallback);
    while(1) {}
}
