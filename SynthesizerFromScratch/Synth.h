#pragma once

#include "olcNoiseMaker.h"
#include "Instrument.h"

namespace SynthesizerFromScratch {

    struct Note
    {
        int id;
        double on;
        double off;
        bool active;

        Note()
        {
            id = 0;             // Position in the scale
            on = 0.0;           // Time note was activated
            off = 0.0;          // Time note was deactivated
            active = false;
        }
    };

    struct Synth
    {
    public:
        Synth();

    private:
        olcNoiseMaker<short>* createSoundMachine();
        static double makeNoise(double dTime);

        // static SynthesizerFromScratch::Harmonica instrument;
    };

}