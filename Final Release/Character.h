#pragma once
#include <string>

class Character {
private:
	short int maxHealth;
	short int health;
	short int level;
	short int x;
	short int y;

public:
	Character() {}

	Character(int a, int b, int c) : x(a), y(b), maxHealth(c), health(c), level(1) {}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	void setX(int i) {
		x = i;
	}

	void setY(int i) {
		y = i;
	}

	void moveLeft() {
		x-=2;
	}

	void moveRight() {
		x+=2;
	}

	void moveUp() {
		y-=1;
	}

	void moveDown() {
		y+=1;
	}

	int getMaxHealth() {
		return maxHealth;
	}

	void setMaxHealth(int i) {
		maxHealth = i;
	}

	int getHealth() {
		return health;
	}

	void setHealth(int i) {
		health = i;
	}

	int getLevel() {
		return level;
	}

	void setLevel(int i) {
		level = i;
	}

	void levelUp() {
		level++;
	}

	void receiveDamage(int i) {
		health -= i;
	}

};