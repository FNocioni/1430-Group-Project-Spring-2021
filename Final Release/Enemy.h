#pragma once
#include "Character.h"

class Enemy : public Character {
private: 
	bool active = false;

public:
	Enemy(int a, int b, int c) : Character(a, b, c) {}

	bool isActive() {
		if (active) {
			return true;
		}
		else {
			return false;
		}
	}

	void setActive(bool b) {
		if (b) {
			active = true;
		}
		else {
			active = false;
		}
	}


};