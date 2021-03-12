#pragma once
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <Windows.h>
#include <MMSystem.h>
	
using namespace std;

class Sound {

private:

public:
	Sound() {
	}

	void playFile(int i) {
		switch (i) {
		case 0:
			PlaySound(TEXT("Bin/Snd/menu.wav"), NULL, SND_ASYNC);
			break;
		default: 
			break;
		}
	}
};