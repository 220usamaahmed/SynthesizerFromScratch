#include <iostream>
#include "olcNoiseMaker.h"
#include "Instrument.h"

olcNoiseMaker<short>* soundMachine;

struct Note
{
	uint8_t id = 0;
	double onTime = 0.0;
	double offTime = 0.0;
	bool active = false;
};

vector<Note> notes;
mutex muxNotes;

typedef bool(*lambda)(Note const& item);
template<class T>
void safe_remove(T& v, lambda f)
{
	auto n = v.begin();
	while (n != v.end())
		if (!f(*n))
			n = v.erase(n);
		else
			++n;
}

Instrument* instrument = new Bell();
double dCurrentFrequency = 0.0;

double userFunction(double dTime)
{
	unique_lock<mutex> lm(muxNotes);

	double dMixedOutput = 0.0;

	for (auto& n : notes)
	{
		bool bNoteFinished = false;
		double dSound = instrument->playSound(8 * pow(1.0594630943592952645618252949463, n.id), dTime, n.onTime, n.offTime, bNoteFinished);

		dMixedOutput += dSound;
	
		if (bNoteFinished)
			n.active = false;
	}

	safe_remove<vector<Note>>(notes, [](Note const& item) { return item.active; });
	return dMixedOutput;
}

void initSynth()
{
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	for (auto d : devices)
		std::wcout << "Found Output Device: " << d << std::endl;
	std::wcout << "Selecting: " << devices[0] << std::endl;

	soundMachine = new olcNoiseMaker<short>(devices[0], 44100, 1, 8, 512);

	soundMachine->SetUserFunction(userFunction);
}

int main()
{
	std::cout << "--- Synthesizer From Scratch ---" << std::endl;

	initSynth();

	while (1)
	{
		double dTimeNow = soundMachine->GetTime();

		for (int k = 0; k < 16; ++k)
		{
			short nKeyState = GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf "[k]));
			
			muxNotes.lock();
			
			auto noteFound = find_if(notes.begin(), notes.end(), [&k](Note const& item) { return item.id == k + 64; });
		
			if (noteFound == notes.end()) // Note not found in vector
			{
				if (nKeyState & 0x8000) // Key has been pressed so create new note
				{
					Note n;
					n.id = k + 64;
					n.onTime = dTimeNow;
					n.active = true;

					notes.emplace_back(n);
				}
			}
			else // Note already in vector
			{
				if (nKeyState & 0x8000) // Key is also pressed
				{
					if (noteFound->offTime > noteFound->onTime) // Key was pressed during release phase
					{
						noteFound->onTime = dTimeNow;
						noteFound->active = true;
					}
				}
				else // Key has been released
				{
					if (noteFound->offTime < noteFound->onTime)
					{
						noteFound->offTime = dTimeNow;
					}
				}
			}
			muxNotes.unlock();
		}
	}
}
