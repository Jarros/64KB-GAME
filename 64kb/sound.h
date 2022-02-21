#pragma once

#include <MMSystem.h>	

const int buffersize_ = 64 * 1024 * 16;//8000*60//19440//4860                // 4k sound buffer
const bool bsound = true; // use sound at all or not

class Sound
{
public:
	void Clear();
	void Foot();
	void Pickup();
	void Beep();
	void Explode();
	void Dead();
	
	bool InitWave();
	bool Proc();
	bool Close();

private:
	void GenBuffer(float a, int b);
	void Prepare();

	HWAVEOUT     hWaveOut_;          // Handle to sound card output
	WAVEFORMATEX WaveFormat_;        // The sound format
	WAVEHDR      WaveHeader_;        // WAVE header for our sound data
	char         Data_[buffersize_];  // Sound data buffer

	HANDLE       Done_;              // Event Handle that tells us the sound has finished being played.
	// This is a real efficient way to put the program to sleep
	// while the sound card is processing the sound buffer
};


