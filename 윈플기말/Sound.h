#pragma once
#include <fmod.h>
#include <windows.h>
#define BGMSOUND 0
#define EFFECTSOUND 1
class Sound {
	int index;
public:
	int getindex();

	FMOD_SYSTEM *System;
	FMOD_SOUND *bgmSound[5];	//3�� ����ݰ���
	FMOD_SOUND *effectSound[5];	//2�� ����Ʈ��ݰ���
	FMOD_CHANNEL *Channel[2];	//2�� ���+����Ʈ �ؼ� ä��

	void Sound_Setup();
	void setindex(int);
	void Sound_Play(int Channel_category, int kind, float volume);
};
