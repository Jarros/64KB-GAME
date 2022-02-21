
#include "main.h"
#include "Sound.h"

void Sound::Clear()
{
	GenBuffer(0, 0);
}
void Sound::Foot()
{
	if (!bsound)return;
	GenBuffer(1, 1);
	Prepare();
}
void Sound::Pickup()
{
	if (!bsound)return;
	GenBuffer(8, 1);
	Prepare();
}
void Sound::Beep()
{
	if (!bsound)return;
	GenBuffer(0, 4);
	Prepare();
}
void Sound::Explode()
{
	if (!bsound)return;
	GenBuffer(0.5, 1);
	Prepare();
}
void Sound::Dead()
{
	if (!bsound)return;
	GenBuffer(0, 2);
	Prepare();
}
bool Sound::InitWave()
{
	if (!bsound)return false;
	// ** Initialize the sound format we will request from sound card **    
	WaveFormat_.wFormatTag = WAVE_FORMAT_PCM;     // Uncompressed sound format
	WaveFormat_.wBitsPerSample = 8;               // Bits per sample per channel
	WaveFormat_.nChannels = 1;
	WaveFormat_.nSamplesPerSec = 8000;//11025;           // Sample Per Second
	WaveFormat_.nBlockAlign = WaveFormat_.nChannels * WaveFormat_.wBitsPerSample / 8;
	WaveFormat_.nAvgBytesPerSec = WaveFormat_.nSamplesPerSec * WaveFormat_.nBlockAlign;
	WaveFormat_.cbSize = sizeof(WAVEFORMATEX);
	// ** Create our "Sound is Done_" event **
	Done_ = CreateEvent(0, FALSE, FALSE, 0);

	MMRESULT a = waveOutOpen(&hWaveOut_, 0, &WaveFormat_, (DWORD)Done_, 0, CALLBACK_EVENT);
	if ((a) != MMSYSERR_NOERROR)
	{
		Message("Sound card cannot be opened.");
		//
		return 1;
	}
	ResetEvent(Done_);

}
bool Sound::Proc()
{
	if (bsound)
	{
		Clear();

		waveOutPause(hWaveOut_);
		waveOutReset(hWaveOut_);
		if (waveOutPrepareHeader(hWaveOut_, &WaveHeader_, sizeof(WaveHeader_)) != MMSYSERR_NOERROR)
		{
			Message("Error preparing Header!");
			return TRUE;
		}
		waveOutWrite(hWaveOut_, &WaveHeader_, sizeof(WaveHeader_));
	}
}
void Sound::GenBuffer(float a, int b)
{
	if (!bsound)return;
	double x;
	int i;
	float t2 = 2.0 * PI / (double)WaveFormat_.nSamplesPerSec, t;
	// ** Make the sound buffer **    
	if (b == 4)
		for (i = 0; i < buffersize_ / 16; i++)
		{
			// ** Generate the sound wave based on FREQUENCY
			// ** x will have a range of -1 to +1
			if (i < 512)
			{
				t = t2 * i;
				x = sin(t * (1000)) / 2.1f;
			}

			// ** scale x to a range of 0-255 (signed char) for 8 bit sound reproduction **
			Data_[i] = (char)(127 * x + 128);
		}

	if (b == 2)
		for (i = 0; i < buffersize_; i++)
		{
			// ** Generate the sound wave based on FREQUENCY
			// ** x will have a range of -1 to +1
			t = t2 * i;
			x = sin(t * (440 + a)) / 3.0f;
			// ** scale x to a range of 0-255 (signed char) for 8 bit sound reproduction **
			Data_[i] = (char)(127 * x + 128);
		}
	if (b == 1)
		for (i = 0; i < 11025; i++)
		{
			// ** Generate the sound wave based on FREQUENCY
			// ** x will have a range of -1 to +1
			t = t2 * i;
			x = 0;//sin(t*(441+a));
			x = ((rnd(0, 15)) * sin(t * (3 + a))) / 128.0; 
			if (i < 512)(x = x * i / 512);
			if (i > 10769)(x = x * (1.0 - (i - 10769) / 256.0));
			// ** scale x to a range of 0-255 (signed char) for 8 bit sound reproduction **
			Data_[i] = (char)(127 * x + 128);
		}
	if (b == 0)
		for (i = 0; i < buffersize_; i++)
		{
			t = t2 * i;
			x = 0;
			Data_[i] = (char)(127 * x + 128);
		}
	// ** Create the wave header for our sound buffer **
	WaveHeader_.lpData = Data_;
	WaveHeader_.dwBufferLength = buffersize_;
	WaveHeader_.dwFlags = 0;
	WaveHeader_.dwLoops = 0;
}
void Sound::Prepare()
{
	if (!bsound)return;
	waveOutReset(hWaveOut_);
	waveOutPrepareHeader(hWaveOut_, &WaveHeader_, sizeof(WaveHeader_));
	waveOutWrite(hWaveOut_, &WaveHeader_, sizeof(WaveHeader_));
}
bool Sound::Close()
{
	waveOutPause(hWaveOut_);
	waveOutReset(hWaveOut_);
	// ** Unprepare our wav header **
	if (waveOutUnprepareHeader(hWaveOut_, &WaveHeader_, sizeof(WaveHeader_)) != MMSYSERR_NOERROR)
	{
		Message("Error unpreparing header!");
		return TRUE;
	}
	// ** close the wav device **
	if (waveOutClose(hWaveOut_) != MMSYSERR_NOERROR)
	{
		Message("sound card cannot be closed!");
		return TRUE;
	}
	// ** Release our event handle **
	CloseHandle(Done_);
}