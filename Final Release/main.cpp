#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>

#include "Character.h"
#include "Player.h"
#include "NPC.h"
#include "Screen.h"

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESCAPE 27
#define ENTER 13

//gameloop runs forever until the program is terminated.
void gameLoop(Screen scr) {
	int current = -1;
	int key = 0;
	int chance;
	int npcchance;
	do {
		//cout << "ID: " << key << endl;
		key = _getch();

		chance = rand() % 20;
		npcchance = rand() % 25;
		if (chance == 0 && !scr.isMenu() && !scr.isPaused() && scr.getCurrentLevel() > 0) {
			cout << "\nYOU HAVE ENCOUNTERED AN ENEMY\n";
			system("pause");
			scr.startDuel();
			scr.print(1);
			continue;
		}
		if (npcchance == 0 && !scr.isMenu() && !scr.isPaused()) {
			scr.newNPC();
		}
		if (key == char(61)) {
			scr.nextLevel();
			continue;
		}
		if (key == KEY_RIGHT) {
			if (!scr.isPaused() && !scr.isMenu() && !scr.isDuel()) {
				scr.processMovement(0);
			}
		}

		if (key == KEY_LEFT) {
			if (!scr.isPaused() && !scr.isMenu() && !scr.isDuel()) {
				scr.processMovement(2);
			}
		}

		if (key == KEY_DOWN) {
			if (!scr.isPaused() && !scr.isMenu() && !scr.isDuel()) {
				scr.processMovement(3);
			}

			if (scr.isPaused()) {
				scr.changePauseMenuOption(1);
				continue;
			}

			if (scr.isMenu()) {
				scr.changeMainMenuOption(1);
				continue;
			}
		}

		if (key == KEY_UP) {
			if (!scr.isPaused() && !scr.isMenu() && !scr.isDuel()) {
				scr.processMovement(1);
			}

			if (scr.isPaused()) {
				scr.changePauseMenuOption(0);
				continue;
			}

			if (scr.isMenu()) {
				scr.changeMainMenuOption(0);
				continue;
			}
		}

		if (key == ESCAPE) {
			if (!scr.isPaused() && !scr.isMenu() && !scr.isDuel()) {
				scr.displayPauseMenu(1);
				continue;
			}
		}

		if (key == ENTER) {
			if (scr.isPaused()) {
				scr.changePauseMenuOption(2);
				continue;
			}
			if (scr.isMenu()) {
				scr.changeMainMenuOption(2);
				continue;
			}
		}

		if (!scr.isPaused() && !scr.isMenu() && !scr.isDuel()) {
			if (scr.getNoPrint()) {
				scr.changeNoPrint();
				scr.print(1);
				continue;
			}
			else {
				scr.print(0);
			}
		}
		
	} while (key != 45);
}

int main() {
	/*
	for (int i = 0; i < 16; i+=1) {
		SetConsoleTextAttribute(hConsole, i);
		cout << "ID: " << i << endl;
		Sleep(5);
		if (i == 254) {
			i = 0;
		}
	}*/

	Screen scr;
	scr.displayMainMenu(1);
	
	gameLoop(scr);
	system("cls");
	scr.changeFontColor(64);
	cout << "Game Loop Terminated, Program will now close...";
	Sleep(2000);

	return 0;
}
