#pragma once
#include <fmod.h>
#include <windows.h>
#define BGMSOUND 0
#define EFFECTSOUND 1

#define MAINMENUBGM 0 
#define FIRSTMAPBGM 1
#define SECONDMAPBGM 2
#define THIRDMAPBGM 3
#define LASTMAPBGM 4
#define LOGINBGM 5

#define JUMPEF 0
#define PORTALEF 1
#define TOMBSTONEEF 2
#define MCLICKEF 3
#define MOVEREF 4

#define BGMVOL 0.0
#define EFVOL 0.5
class Sound {
	int index;
	static class Sound* mSound;
	Sound() {};
public:
	int getindex();
	static class Sound* GetSelf();

	FMOD_SYSTEM *System;
	FMOD_SOUND *bgmSound[6];	//3�� ����ݰ���
	FMOD_SOUND *effectSound[5];	//2�� ����Ʈ��ݰ���
	FMOD_CHANNEL *Channel[2];	//2�� ���+����Ʈ �ؼ� ä��

	void Sound_Setup();
	void setindex(int);
	void Sound_Play(int Channel_category, int kind, float volume);
};
