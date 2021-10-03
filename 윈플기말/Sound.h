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
	FMOD_SOUND *bgmSound[5];	//3는 배경브금갯수
	FMOD_SOUND *effectSound[5];	//2는 이펙트브금갯수
	FMOD_CHANNEL *Channel[2];	//2는 배경+이펙트 해서 채널

	void Sound_Setup();
	void setindex(int);
	void Sound_Play(int Channel_category, int kind, float volume);
};
