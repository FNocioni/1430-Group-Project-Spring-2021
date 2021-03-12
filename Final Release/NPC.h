#pragma once
#include "Character.h"

class NPC : public Character {
private:
	bool active;

public:
	NPC() : active(false) {}
	
	void setActive(bool b) {
		if (b) {
			active = true;
		}
		else {
			active = false;
		}
	}

	bool isActive() {
		if (active) {
			return true;
		}
		else {
			return false;
		}
	}
};