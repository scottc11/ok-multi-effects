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
Parameter cutoff_ctrl, res_ctrl, drive_ctrl;
Svf svf;


void __attribute__((optimize("O0"))) AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    patch.ProcessAnalogControls();

    float output_level = patch.GetAdcValue(POT_A);
    float gain = patch.GetAdcValue(POT_C) * 100 + 1.2;

    // get new control values
    float cutoff = cutoff_ctrl.Process();
    float res = res_ctrl.Process();
    float drive = drive_ctrl.Process();

    svf.SetFreq(cutoff);
    svf.SetRes(res);
    svf.SetDrive(drive);

    for(size_t i = 0; i < size; i++)
    {
        svf.Process(in[0][i]);
        // for (int chan = 0; chan < 2; chan++)
        // {
        //     float input = in[chan][i] * output_level;
        //     float output = input * gain;
        //     out[chan][i] = svf.Low();
        // }
        out[0][i] = svf.Low();
        out[1][i] = svf.High();
        out[2][i] = svf.Band();
        out[3][i] = svf.Notch();
    }
}


int main(void)
{
    float sampleRate;
    patch.Init();
    sampleRate = patch.AudioSampleRate();

    svf.Init(sampleRate);
    
    // setup controls
    cutoff_ctrl.Init(patch.controls[POT_C], 20, 20000, Parameter::LOGARITHMIC);
    res_ctrl.Init(patch.controls[POT_B], .3, 1, Parameter::LINEAR);
    drive_ctrl.Init(patch.controls[POT_D], .3, 1, Parameter::LINEAR);

    patch.StartAdc();
    patch.StartAudio(AudioCallback);
    while(1) {}
}
