#include "daisy_patch_sm.h"
#include "daisysp.h"
#include "pitch_frequency_map.h"

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
Oscillator single_osc;
Oscillator multi_osc[2];
Parameter osc_1_freq_ctrl, osc_2_freq_ctrl;

void singleVCO(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size, Oscillator *osc);
void handleMultiVCO(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size, Oscillator *oscillators);
void filter(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size);

void __attribute__((optimize("O0"))) AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    handleMultiVCO(in, out, size, multi_osc);
    // singleVCO(in, out, size, &single_osc);
    filter(in, out, size);
}

void __attribute__((optimize("O0"))) handleMultiVCO(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size, Oscillator *oscillators)
{
    int osc_1_pitch = osc_1_freq_ctrl.Process();
    int osc_2_pitch = osc_2_freq_ctrl.Process();

    oscillators[0].SetFreq(PITCH_FREQ_MAP[MAJOR_SCALE[osc_1_pitch]]);
    oscillators[0].SetWaveform(2);
    oscillators[1].SetFreq(PITCH_FREQ_MAP[MAJOR_SCALE[osc_2_pitch]] - 0.5);
    oscillators[1].SetWaveform(2);

    for (size_t i = 0; i < size; i++)
    {
        float mix = 0;
        // Process and output the three oscillators
        for (size_t chn = 0; chn < 2; chn++)
        {
            float sig = oscillators[chn].Process();
            mix += sig * .35f;
            out[chn][i] = sig;
        }

        // output the mixed oscillators
        out[0][i] = mix;
    }
}

void singleVCO(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size, Oscillator *osc)
{
    float sig, freq, amp;
    size_t wave;

    for (size_t i = 0; i < size; i++)
    {
        // Read Knobs
        freq =440.0f;
        wave = 2;
        amp = 0.5f;
        // Set osc params
        osc->SetFreq(freq);
        osc->SetWaveform(wave);
        osc->SetAmp(amp);
        //.Process
        sig = osc->Process();
        // Assign Synthesized Waveform to all four outputs.
        for (size_t chn = 0; chn < 4; chn++)
        {
            out[chn][i] = sig;
        }
    }
}

void filter(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
    // patch.ProcessAnalogControls();

    // float output_level = patch.GetAdcValue(POT_A);
    // float gain = patch.GetAdcValue(POT_C) * 100 + 1.2;

    // get new control values
    float cutoff = cutoff_ctrl.Process();
    // float res = res_ctrl.Process();
    float drive = drive_ctrl.Process();

    svf.SetFreq(cutoff);
    // svf.SetRes(res);
    svf.SetDrive(drive);

    for (size_t i = 0; i < size; i++)
    {
        svf.Process(out[0][i]);
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

    single_osc.Init(sampleRate);

    // init oscillators
    for (int i = 0; i < 2; i++)
    {
        multi_osc[i].Init(sampleRate);
        multi_osc[i].SetAmp(.7);
    }

    svf.Init(sampleRate);
    
    osc_1_freq_ctrl.Init(patch.controls[POT_C], 0, 55, Parameter::LINEAR);
    osc_2_freq_ctrl.Init(patch.controls[POT_D], 0, 55, Parameter::LINEAR);

    // setup controls
    cutoff_ctrl.Init(patch.controls[POT_A], 20, 20000, Parameter::LOGARITHMIC);
    // res_ctrl.Init(patch.controls[POT_B], .3, 1, Parameter::LINEAR);
    drive_ctrl.Init(patch.controls[POT_B], .3, 1, Parameter::LINEAR);

    patch.StartAdc();
    patch.StartAudio(AudioCallback);
    while(1) {}
}
