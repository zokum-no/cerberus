#include "includes/basswrap.h"

HWND wnd_;
HSTREAM stream1;

void basswrap_init(HWND wnd)
{
	wnd_ = wnd;
	if (!BASS_Init(1,44100,0,wnd,NULL)) MessageBox(wnd,"BASS error","BASS",0);
}

void basswrap_unload(void)
{
	BASS_StreamFree(stream1);
}

void basswrap_playsample(int s)
{



}
void basswrap_loadsample(char *filename, int s)
{


}



void basswrap_load(char *filename)
{
	if (!(stream1 = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_MP3_SETPOS))) MessageBox(wnd_,"BASS error, file not found?","BASS",0);
}

void basswrap_play_seconds(double in)
{
	BASS_ChannelStop(stream1);
	basswrap_play(BASS_ChannelSeconds2Bytes(stream1, in));
	BASS_StreamPlay(stream1,FALSE,0);

}

void basswrap_play(double ofset)
{
	if (!BASS_StreamPlay(stream1,FALSE,0)) MessageBox(wnd_,"BASS error, can not play stream?","BASS",0);	
	BASS_ChannelSetPosition(stream1, unsigned(44100 * 2 * 2 * ofset));
}
void basswrap_volume(int volume)
{
    BASS_ChannelSetAttributes(stream1, 44100, volume, 0);
}

void basswrap_uninit()
{
	BASS_Free();
}

float basswrap_gettime()
{
	return BASS_ChannelBytes2Seconds(stream1, BASS_ChannelGetPosition(stream1));
}

void basswrap_getsync(float* data)
{
	BASS_ChannelGetData(stream1, data, BASS_DATA_FFT512);
}