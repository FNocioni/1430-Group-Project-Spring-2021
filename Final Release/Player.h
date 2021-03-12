#pragma once
#include <iostream>
#include "Character.h"

class Player : public Character {
private:
	int xp;
	std::string *collisions;
	int damage;
	bool key = false;

public:
	Player() { }

	Player(int a, int b, int c) : Character(a, b, c), xp(0), damage(20) {}
	
	int getXP() {
		return xp;
	}

	void setXP(int x) {
		xp = x;
	}

	void addXP(int x) {
		xp += x;
	}

	void copyCollisions(std::string s[], int lines) {
		collisions = new std::string[lines];
		for (int i = 0; i < lines; i++) {
			collisions[i] = s[i];
		}
	}

	void checkCollision(int dir) {
		switch (dir) {
		case 0:
			if (collisions[getY()][getX()+1] != '1') {
				moveRight();
			}
			break;
		case 1:
			if (collisions[getY()-1][getX()] != '1') {
				moveUp();
			}
			break;
		case 2:
			if (collisions[getY()][getX()-1] != '1') {
				moveLeft();
			}
			break;
		case 3:
			if (collisions[getY()+1][getX()] != '1') {
				moveDown();
			}
			break;
		}
	}

	int getDamage() {
		return damage;
	}

	void setDamage(int i) {
		damage = i;
	}

	void levelUpDamage() {
		damage += 10;
		setHealth(getMaxHealth());
		levelUp();
	}

	void levelUpHealth() {
		setMaxHealth(getMaxHealth() + 25);
		setHealth(getMaxHealth());
		levelUp();
	}

	bool getKey() {
		if (key) {
			return true;
		}
		else {
			return false;
		}
	}

	void setKey(bool b) {
		if (b) {
			key = true;
		}
		else {
			key = false;
		}
	}

};