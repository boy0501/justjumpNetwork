#include "Sound.h"

int Sound::getindex()
{
	return index;
}

void Sound::setindex(int i)
{
	index = i;
}

void Sound::Sound_Setup()
{
	index = 0;
	char string[100];

	FMOD_System_Create(&System);
	FMOD_System_Init(System, 10, FMOD_INIT_NORMAL, NULL);

	for (int i = 0; i < 5; ++i)
	{
		wsprintfA(string, "sound/bgm%d.mp3", i);
		FMOD_System_CreateSound(System, string, FMOD_LOOP_NORMAL, 0, &bgmSound[i]);
	}
	FMOD_System_CreateSound(System, "sound/Jump.mp3", FMOD_DEFAULT, 0, &effectSound[0]);
	FMOD_System_CreateSound(System, "sound/Portal.mp3", FMOD_DEFAULT, 0, &effectSound[1]);	
	FMOD_System_CreateSound(System, "sound/Tombstone.mp3", FMOD_DEFAULT, 0, &effectSound[2]);
	FMOD_System_CreateSound(System, "sound/BtMouseClick.mp3", FMOD_DEFAULT, 0, &effectSound[3]);	
	FMOD_System_CreateSound(System, "sound/BtMouseOver.mp3", FMOD_DEFAULT, 0, &effectSound[4]);
	
}


void Sound::Sound_Play(int Channel_category, int kind, float volume)
{
	if (Channel_category == 0)
	{
		FMOD_Channel_Stop(Channel[Channel_category]);
		FMOD_System_PlaySound(System, bgmSound[kind], NULL, 0, &Channel[Channel_category]);
		FMOD_Channel_SetVolume(Channel[Channel_category], volume);
	}
	else if (Channel_category == 1)
	{
		FMOD_Channel_Stop(Channel[Channel_category]);
		FMOD_System_PlaySound(System, effectSound[kind], NULL, 0, &Channel[Channel_category]);
		FMOD_Channel_SetVolume(Channel[Channel_category], volume);

	}
}