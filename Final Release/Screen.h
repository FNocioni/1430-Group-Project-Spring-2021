#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <windows.h>
#include "wtypes.h"
#include <cwchar>
#include <cstdlib>
#include "Sound.h"
#include "Player.h"
#include "Character.h"
#include "Dialog.h"
#include "Enemy.h"
#include "NPC.h"

#define ENTER 13

//Screen class controls all the drawing to the console.
//There will be functions for reading text files and displaying maps from them.
class Screen {
private:
	//Screen Variables
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi;
	CONSOLE_CURSOR_INFO cci;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD c;
	COORD bc;
	COORD np1;
	COORD np2;
	COORD np3;
	COORD np4;
	int fontSize = 20;
	int hostHorizontal = 0;
	int hostVertical = 0;

	//Game States
	bool paused = false;
	bool inMenu = false;
	bool noPrint = false;
	bool inDuel = false;
	bool level[4];
	bool nNPC = false;
	int pauseMenuOption = 0;
	int mainMenuOption = 0;
	NPC *npc[4];
	Sound *snd;
	Player *player;
	Dialog *dialog;
	Enemy *enemy;

	std::string battleDialog = "";

	//Map Info
	std::string *lines;
	std::string *colors;
	std::string *collisions;
	int numOfLines;

public:
	Screen() {
		//set up font with color, size, and type.
		c.X = 0;
		c.Y = 0;

		bc.X = 0;
		bc.Y = 25;

		np1.X = 61;
		np1.Y = 2;

		np2.X = 61;
		np2.Y = 5;

		np3.X = 61;
		np3.Y = 8;

		np4.X = 61;
		np4.Y = 11;

		SetConsoleTextAttribute(hConsole, 15);
		GetConsoleCursorInfo(hConsole, &cci);
		cci.bVisible = false;
		SetConsoleCursorInfo(hConsole, &cci);

		HWND consoleWindow = GetConsoleWindow();
		SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		GetDesktopResolution(hostHorizontal, hostVertical);

		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontSize/2;
		cfi.dwFontSize.Y = fontSize;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		//std::wcscpy(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
		
		snd = new Sound();
		player = new Player(4, 8, 100);
		dialog = new Dialog(40, 8, 100);
		enemy = new Enemy(50, 8, 100);
		enemy->setActive(false);
		
		for (int i = 0; i < 4; i++) {
			npc[i] = new NPC();
			npc[i]->setActive(false);
			npc[i]->setMaxHealth(25);
		}

		//Initialize Levels
		for (int i = 0; i < 10; i++) {
			level[i] = false;
		}

		try {
			GetConsoleScreenBufferInfo(hConsole, &csbi);
			COORD scrollbar = {
				csbi.srWindow.Right - csbi.srWindow.Left + 1,
				csbi.srWindow.Bottom - csbi.srWindow.Top + 1
			};

			SetConsoleScreenBufferSize(hConsole, scrollbar);
		}
		catch (...) {
			SetConsoleTextAttribute(hConsole, 64);
			std::cout << "Error Formatting Screen";
		}
	}

	void GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}

	void changeFontColor(int a) {
		SetConsoleTextAttribute(hConsole, a);
	}

	void readMap(std::string fileName) {
		try {
			std::ifstream inputStream("Bin/Map/" + fileName + ".MAP");
			if (!inputStream) {
				throw std::runtime_error("Map File Not Found; Please Check The Address: Bin/Map/" + fileName + ".MAP");
			}
			numOfLines = getLines(inputStream);
			if (numOfLines == 0) {
				throw std::runtime_error("Map has 0 lines (Map Is Empty)");
			}
			
			lines = new std::string[numOfLines];
			
			for (int i = 0; i < numOfLines; i++) {
				getline(inputStream, lines[i]);
			}
			readColors(fileName);
			readCollisions(fileName);
		}
		catch (std::exception e) {
			SetConsoleTextAttribute(hConsole, 64);
			std::cout << e.what() << std::endl;
			system("pause");
		}
	}

	void readColors(std::string fileName) {
		try {
			std::ifstream inputStream("Bin/Clr/" + fileName + ".CLR");
			if (!inputStream) {
				throw std::runtime_error("Color File Not Found; Please Check The Address: Bin/Clr/" + fileName + ".CLR");
			}
			if (numOfLines == 0) {
				throw std::runtime_error("Color File is Empty");
			}

			colors = new std::string[numOfLines];

			for (int i = 0; i < numOfLines; i++) {
				getline(inputStream, colors[i]);
			}
		}
		catch (std::exception e) {
			SetConsoleTextAttribute(hConsole, 64);
			std::cout << e.what() << std::endl;
			system("pause");
		}
	}

	void readCollisions(std::string fileName) {
		try {
			std::ifstream inputStream("Bin/Meta/" + fileName + ".META");
			if (!inputStream) {
				throw std::runtime_error("Collisions File Not Found; Please Check The Address: Bin/Meta/" + fileName + ".meta");
			}
			if (numOfLines == 0) {
				throw std::runtime_error("Collisions File is Empty");
			}

			collisions = new std::string[numOfLines];

			for (int i = 0; i < numOfLines; i++) {
				getline(inputStream, collisions[i]);
			}
		}

		catch (std::exception e) {
			SetConsoleTextAttribute(hConsole, 64);
			std::cout << e.what() << std::endl;
			system("pause");
		}

		player->copyCollisions(collisions, numOfLines);
	}

	int getLines(std::ifstream& ifs) {
		int temp = 0;
		std::string s = "";
		while (!ifs.eof()) {
			getline(ifs, s);
			if (s != "") {
				temp++;
			}
		}
		ifs.seekg(0, std::ios::beg);
		ifs.clear();
		return temp;
	}

	int decodeColor(char i) {
		switch (i) {
		case '0':
			return 0;
			break;
		case '1':
			return 128;
			break;
		case '2':
			return 1;
			break;
		case '3':
			return 4;
			break;
		case '4':
			return 42;
			break;
		case '5':
			return 162;
			break;
		case '6':
			return 143;
			break;
		case '7':
			return 3;
			break;
		default:
			return 8;
		}
	}

	int decodeAlt(char i) {
		switch (i) {
		case '0':
			return 32;
			break;
		case '1':
			return 219;
			break;
		case '2':
			return 221;
			break;
		case '3':
			return 222;
			break;
		case '4':
			return 220;
			break;
		case '5':
			return 223;
			break;
		case '6':
			return 254;
			break;
		case '7':
			return 177;
			break;
		case '8':
			return 176;
			break;
		case 'L':
			return 185;
			break;
		case 'l':
			return 204;
			break;
		case 'B':
			return 249;
			break;
		default:
			return 63;
		}
	}

	void displayPauseMenu(int state) {
		if (state == 1) {
			SetConsoleTextAttribute(hConsole, 0);
			pauseMenuOption = 0;
		}
		system("cls");
		paused = true;
		if (pauseMenuOption == 0) {
			SetConsoleCursorPosition(hConsole, c);
			SetConsoleTextAttribute(hConsole, 6);
			std::cout << char(176) << " G A M E  P A U S E D " << char(176) << std::endl;
			SetConsoleTextAttribute(hConsole, 240);
			std::cout << "->Return To Game" << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "Save Game" << std::endl;
			std::cout << "Load Game" << std::endl;
			std::cout << "Main Menu" << std::endl;
		}
		else if (pauseMenuOption == 1) {
			SetConsoleCursorPosition(hConsole, c);
			SetConsoleTextAttribute(hConsole, 6);
			std::cout << char(176) << " G A M E  P A U S E D " << char(176) << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "Return To Game" << std::endl;
			SetConsoleTextAttribute(hConsole, 240);
			std::cout << "->Save Game" << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "Load Game" << std::endl;
			std::cout << "Main Menu" << std::endl;
		}
		else if (pauseMenuOption == 2) {
			SetConsoleCursorPosition(hConsole, c);
			SetConsoleTextAttribute(hConsole, 6);
			std::cout << char(176) << " G A M E  P A U S E D " << char(176) << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "Return To Game" << std::endl;
			std::cout << "Save Game" << std::endl;
			SetConsoleTextAttribute(hConsole, 240);
			std::cout << "->Load Game" << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "Main Menu" << std::endl;
		}
		else if (pauseMenuOption == 3) {
			SetConsoleCursorPosition(hConsole, c);
			SetConsoleTextAttribute(hConsole, 6);
			std::cout << char(176) << " G A M E  P A U S E D " << char(176) << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			std::cout << "Return To Game" << std::endl;
			std::cout << "Save Game" << std::endl;
			std::cout << "Load Game" << std::endl;
			SetConsoleTextAttribute(hConsole, 240);
			std::cout << "->Main Menu" << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}

	void changePauseMenuOption(int i) {
		if (i == 1 && pauseMenuOption != 3) {
			pauseMenuOption++;
			displayPauseMenu(0);
			snd->playFile(0);
		}
		else if (i == 0 && pauseMenuOption != 0) {
			pauseMenuOption--;
			displayPauseMenu(0);
			snd->playFile(0);
		}
		if (i == 2) {
			if (pauseMenuOption == 0) {
				paused = false;
				refreshLevel();
			}
			if (pauseMenuOption == 1) {
				saveGame();
			}
			if (pauseMenuOption == 2) {
				loadGame();
			}
			if (pauseMenuOption == 3) {
				paused = false;
				inMenu = true;
				displayMainMenu(1);
			}
		}
	}

	bool isPaused() {
		if (paused) {
			return true;
		}
		else {
			return false;
		}
	}

	void displayMainMenu(int state) {
		if (state == 1) {
			mainMenuOption = 0;
			system("cls");
		}
		inMenu = true;
		
		SetConsoleCursorPosition(hConsole, c);
		readMap("mainMenu");
		for (int i = 0; i < numOfLines; i++) {
			for (unsigned int j = 0; j < lines[i].size(); j++) {
				if (i == 20 && j == 32) {
					if (mainMenuOption == 0) {
						SetConsoleTextAttribute(hConsole, 240);
					}
					else {
						SetConsoleTextAttribute(hConsole, 15);
					}
					std::cout << "START GAME";
				}
				if (i == 22 && j == 32) {
					if (mainMenuOption == 1) {
						SetConsoleTextAttribute(hConsole, 240);
					}
					else {
						SetConsoleTextAttribute(hConsole, 15);
					}
					std::cout << "LOAD GAME";
				}
				if (i == 24 && j == 32) {
					if (mainMenuOption == 2) {
						SetConsoleTextAttribute(hConsole, 240);
					}
					else {
						SetConsoleTextAttribute(hConsole, 15);
					}
					std::cout << "SETTINGS";
				}
				if (i == 26 && j == 32) {
					if (mainMenuOption == 3) {
						SetConsoleTextAttribute(hConsole, 240);
					}
					else {
						SetConsoleTextAttribute(hConsole, 15);
					}
					std::cout << "EXIT GAME";
				}
				SetConsoleTextAttribute(hConsole, decodeColor(colors[i][j]));
				std::cout << char(decodeAlt(lines[i][j]));
			}
			std::cout << "\n";
		}

	}

	void changeMainMenuOption(int i) {
		//0 for up, 1 for down, 2 to refresh scene
		if (i == 1 && mainMenuOption != 3) {
			mainMenuOption++;
			displayMainMenu(0);
			snd->playFile(0);
		}
		else if (i == 0 && mainMenuOption != 0) {
			mainMenuOption--;
			displayMainMenu(0);
			snd->playFile(0);
		}
		
		if (i == 2) {
			if (mainMenuOption == 0) {
				inMenu = false;
				if (getCurrentLevel() == -1) {
					nextLevel();
				}
				else {
					refreshLevel();
				}
			}
			if (mainMenuOption == 1) {
				loadGame();
			}
			if (mainMenuOption == 3) {
				exit(0);
			}
		}
	}

	bool isMenu() {
		if (inMenu) {
			return true;
		}
		else {
			return false;
		}
	}

	bool checkCollision(int x, int y) {
		if (collisions[x][y] == 1) {
			return true;
		}
		return false;
	}

	void print(int state) {
		if (state == 1) {
			system("cls");
		}
		else {
			SetConsoleCursorPosition(hConsole, c);
		}
		std::string d = "";
		for (int i = 0; i < numOfLines; i++) {
			for (unsigned int j = 0; j < lines[i].size(); j++) {
				if (!paused && !inMenu && !inDuel) {	
					if (player->getY() == dialog->getY() && player->getX() == dialog->getX() && getCurrentLevel() <= 1) {
						d = dialog->readLine(getCurrentLevel());
					}
					else {
						d = "";
					}
					if (player->getY() == 8 && player->getX() == 58) {
						if (getCurrentLevel() > 0 && player->getKey()) {
							nextLevel();
							refreshLevel();
							break;
						}
						if (getCurrentLevel() == 0) {
							nextLevel();
							refreshLevel();
							break;
						}
						
					}					
					if (player->getY() == i && player->getX() == j) {
						SetConsoleTextAttribute(hConsole, 1);
						std::cout << char(64);
						continue;
					}
					if (dialog->getY() == i && dialog->getX() == j && getCurrentLevel() <= 1) {
						SetConsoleTextAttribute(hConsole, 14);
						std::cout << char(64);
						continue;
					}
					if (i == 8 && j == 58) {
						SetConsoleTextAttribute(hConsole, 5);
						std::cout << char(178);
						continue;
					}
				}
				SetConsoleTextAttribute(hConsole, decodeColor(colors[i][j]));
				std::cout << char(decodeAlt(lines[i][j]));
			}
			if (npc[0]->isActive()) {
				if (i == 1) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " NPC 1 STATS:";
				}
				if (i == 2) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " HEALTH: " << npc[0]->getHealth() << "/" << npc[0]->getMaxHealth();
				}
			}
			if (npc[1]->isActive()) {
				if (i == 4) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " NPC 2 STATS:";
				}
				if (i == 5) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " HEALTH: " << npc[1]->getHealth() << "/" << npc[1]->getMaxHealth();
				}
			}
			if (npc[2]->isActive()) {
				if (i == 7) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " NPC 3 STATS:";
				}
				if (i == 8) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " HEALTH: " << npc[2]->getHealth() << "/" << npc[2]->getMaxHealth();
				}
			}
			if (npc[3]->isActive()) {
				if (i == 10) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " NPC 4 STATS:";
				}
				if (i == 11) {
					SetConsoleTextAttribute(hConsole, 11);
					std::cout << " HEALTH: " << npc[3]->getHealth() << "/" << npc[3]->getMaxHealth();
				}
			}
			std::cout << "\n";
		}
		if (!paused && !inMenu) {
			printGUI();
			std::cout << "\n";
			if (d != "") {
				SetConsoleTextAttribute(hConsole, 6);
				std::cout << d;
			}
			else {
				for (int i = 0; i < 150; i++) {
					std::cout << " ";
				}
			}
		}
	}

	void nextLevel() {
		system("cls");
		player->setKey(false);
		noPrint = true;
		int current = -1;
		for (int i = 0; i < 4; i++) {
			if (level[i]) {
				current = i;
				break;
			}
		}
		if (current != 3) {
			level[current] = false;
			level[current + 1] = true;
		}
		if (current == 3) {
			endGame();
		}
		std::string temp = std::to_string(current+2);
		readMap("level" + temp);
		player->setX(4);
		player->setY(8);
		if (getCurrentLevel() > 0) {
			enemy->setActive(true);
		}
		print(1);
	}

	void refreshLevel() {
		system("cls");
		int current = 0;
		for (int i = 0; i < 4; i++) {
			if (level[i]) {
				current = i;
				break;
			}
		}
		std::string temp = std::to_string(current + 1);
		readMap("level" + temp);
		print(1);
	}

	int getCurrentLevel() {
		for (int i = 0; i < 4; i++) {
			if (level[i]) {
				return i;
			}
		}
		return -1;
	}

	void printGUI() {
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << "Health: ";
		if (player->getHealth() <= (player->getMaxHealth() / 2)) {
			SetConsoleTextAttribute(hConsole, 6);
		}
		if (player->getHealth() <= (player->getMaxHealth() / 2 - (player->getMaxHealth() / 4))) {
			SetConsoleTextAttribute(hConsole, 4);
		}
		std::cout << player->getHealth() << "/" << player->getMaxHealth();
		if (inDuel) {
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "\t\t\t\t     " << "Health: " << enemy->getHealth() << "/" << enemy->getMaxHealth();
		}


		std::cout << "\n";
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << "XP: ";
		SetConsoleTextAttribute(hConsole, 1);
		int nextLevel = player->getLevel() * 25;
		std::cout << player->getXP() << "/" << nextLevel << " (LEVEL: " << player->getLevel() << ")";
		if (inDuel) {
			printDuel();
		}
		if (!inDuel) {
			SetConsoleTextAttribute(hConsole, 6);
			std::cout << "\n"; 
			if (player->getKey()) {
				std::cout << "Key Acquired!";
			}
			else {
				std::cout << "Key Not Acquired!";
			}
		}
		if (nNPC) {
			std::cout << "\nA New NPC Has Joined Your Party!";
			nNPC = false;
			char n = _getch();
		}
		std::cout << "\n                                     \n                                     ";
	}

	void processMovement(int dir) {
		switch (dir) {
		case 0:
			player->checkCollision(0);
			break;
		case 1:
			player->checkCollision(1);
			break;
		case 2:
			player->checkCollision(2);
			break;
		case 3:
			player->checkCollision(3);
			break;
		default:
			break;
		}
	}

	bool getNoPrint() {
		if (noPrint) {
			return true;
		}
		else {
			return false;
		}
	}

	void changeNoPrint() {
		noPrint = false;
	}

	bool isDuel() {
		if (inDuel) {
			return true;
		}
		else {
			return false;
		}
	}

	void startDuel() {
		inDuel = true;
		enemy->setMaxHealth(75 + (getCurrentLevel() * 25));
		enemy->setHealth(enemy->getMaxHealth());
		readMap("duel");
	}

	void refreshHealth() {
		SetConsoleTextAttribute(hConsole, 11);
		if (npc[0]->isActive()) {
			SetConsoleCursorPosition(hConsole, np1);
			std::cout << "HEALTH: " << npc[0]->getHealth() << "/" << npc[0]->getMaxHealth() << "   ";
		}
		if (npc[1]->isActive()) {
			SetConsoleCursorPosition(hConsole, np2);
			std::cout << "HEALTH: " << npc[1]->getHealth() << "/" << npc[1]->getMaxHealth() << "   ";
		}
		if (npc[2]->isActive()) {
			SetConsoleCursorPosition(hConsole, np3);
			std::cout << "HEALTH: " << npc[2]->getHealth() << "/" << npc[2]->getMaxHealth() << "   ";
		}
		
		if (npc[3]->isActive()) {
			SetConsoleCursorPosition(hConsole, np4);
			std::cout << "HEALTH: " << npc[3]->getHealth() << "/" << npc[3]->getMaxHealth() << "   ";
		}

		SetConsoleCursorPosition(hConsole, bc);
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << "Health: ";
		if (player->getHealth() <= (player->getMaxHealth() / 2)) {
			SetConsoleTextAttribute(hConsole, 6);
		}
		if (player->getHealth() <= (player->getMaxHealth() / 2 - (player->getMaxHealth() / 4))) {
			SetConsoleTextAttribute(hConsole, 4);
		}
		std::cout << player->getHealth() << "/" << player->getMaxHealth() << "     ";
		if (inDuel) {
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "                        " << "Health: " << enemy->getHealth() << "/" << enemy->getMaxHealth() << "      ";
		}


		std::cout << "\n";
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << "XP: ";
		SetConsoleTextAttribute(hConsole, 1);
		int nextLevel = player->getLevel() * 25;
		std::cout << player->getXP() << "/" << nextLevel;
		SetConsoleTextAttribute(hConsole, 6);
		std::cout << battleDialog << "                                         ";
	}

	void playerAttack() {
		for (int i = 0; i < 4; i++) {
			if (npc[i]->isActive()) {
				int damage = rand() % 10 + 1;
				battleDialog = "\nNPC " + std::to_string(i+1) + " Dealt " + std::to_string(damage) + " Damage!";
				enemy->setHealth(enemy->getHealth() - damage);
				refreshHealth();
				std::cout << "\n                                                ";
				std::cout << "\n                                                ";
				char n = _getch();
			}
		}

		int damage = player->getDamage();
		battleDialog = "\nYou Dealt " + std::to_string(damage) + " Damage!";
		enemy->setHealth(enemy->getHealth() - damage);
		refreshHealth();
	}

	void enemyAttack() {
		for (int i = 0; i < 4; i++) {
			if (npc[i]->isActive()) {
				int damage = rand() % 10 + 1;
				battleDialog = "\nThe Enemy Dealt " + std::to_string(damage) + " Damage to NPC " + std::to_string(i+1);
				npc[i]->setHealth(npc[i]->getHealth() - damage);
				refreshHealth();
				std::cout << "\n                                                ";
				std::cout << "\n                                                ";
				char n = _getch();
			}
		}

		int damage = 9 * getCurrentLevel();
		battleDialog = "\nThe Enemy Dealt " + std::to_string(damage) + " Damage!";
		player->setHealth(player->getHealth() - damage);
		refreshHealth();
	}

	void printDuel() {
		bool playerTurn = true;
		SetConsoleTextAttribute(hConsole, 2); 
		std::cout << "\nStart The Duel...";
		while (inDuel) {
			char k = _getch();
			if (k == ENTER) {
				if (enemy->getHealth() <= 0) {
					std::cout << "\nEnemy Defeated! ";
					if (!player->getKey()) {
						int i = rand() % 3;
						if (i == 0) {
							std::cout << "You have Received A Dungeon Key!";
							player->setKey(true);
						}
					}

					system("pause");
					player->setXP(player->getXP() + 10);
					if (player->getXP() >= (player->getLevel() * 25)) {
						std::cout << "You have leveled up! Would you like to increase Max Health (1) or Damage (2)";
						while(true){
							char n = _getch();
							if (n == '1') {
								player->levelUpHealth();
								break;
							}
							if (n == '2') {
								player->levelUpDamage();
								break;
							}
						}
					}
					inDuel = false;
					refreshLevel();
					break;
				}
				if (npc[0]->getHealth() <= 0 && npc[0]->isActive()) {
					std::cout << "\nNPC 1 Has Been Defeated And Left Your Party! ";
					char l = _getch();
					npc[0]->setActive(false);
				}
				if (npc[1]->getHealth() <= 0 && npc[1]->isActive()) {
					std::cout << "\nNPC 2 Has Been Defeated And Left Your Party! ";
					char l = _getch();
					npc[1]->setActive(false);
				}
				if (npc[2]->getHealth() <= 0 && npc[2]->isActive()) {
					std::cout << "\nNPC 3 Has Been Defeated And Left Your Party! ";
					char l = _getch();
					npc[2]->setActive(false);
				}
				if (npc[3]->getHealth() <= 0 && npc[3]->isActive()) {
					std::cout << "\nNPC 4 Has Been Defeated And Left Your Party! ";
					char l = _getch();
					npc[3]->setActive(false);
				}
				if (player->getHealth() <= 0) {
					system("cls");
					SetConsoleTextAttribute(hConsole, 4);
					std::cout << "You Died! \nGame Over!\n";
					system("pause");
					exit(0);
				}
				if (playerTurn) {
					playerAttack();
					playerTurn = false;
					continue;
				}
				else {
					enemyAttack();
					playerTurn = true;
					continue;
				}
			}
		}
	}

	void newNPC() {
		if (!npc[0]->isActive()) {
			npc[0]->setActive(true);
			npc[0]->setHealth(npc[0]->getMaxHealth());
			nNPC = true;
		}
		else {
			if (!npc[1]->isActive()) {
				npc[1]->setActive(true);
				npc[1]->setHealth(npc[1]->getMaxHealth());
				nNPC = true;
			}
			else {
				if (!npc[2]->isActive()) {
					npc[2]->setActive(true);
					npc[2]->setHealth(npc[2]->getMaxHealth());
					nNPC = true;
				}
				else {
					if (!npc[3]->isActive()) {
						npc[3]->setActive(true);
						npc[3]->setHealth(npc[3]->getMaxHealth());
						nNPC = true;
					}
				}
			}
		}
	}

	void saveGame() {
		std::ofstream ofs("Bin/saveGame.txt");
		ofs << getCurrentLevel() << "\n";
		ofs << player->getMaxHealth() << "\n";
		ofs << player->getHealth() << "\n";
		ofs << player->getDamage() << "\n";
		ofs << player->getXP() << "\n";
		ofs << player->getLevel() << "\n";
		ofs << npc[0]->isActive() << "\n";
		ofs << npc[0]->getHealth() << "\n";
		ofs << npc[1]->isActive() << "\n";
		ofs << npc[1]->getHealth() << "\n";
		ofs << npc[2]->isActive() << "\n";
		ofs << npc[2]->getHealth() << "\n";
		ofs << npc[3]->isActive() << "\n";
		ofs << npc[3]->getHealth() << "\n";
		ofs << player->getKey() << "\n";
		ofs << player->getX() << "\n";
		ofs << player->getY() << "\n";
	}

	void loadGame() {
		std::ifstream ifs("Bin/saveGame.txt");
		int in;
		ifs >> in;
		for (int i = 0; i < 4; i++) {
			level[i] = false;
		}
		level[in] = true;
		ifs >> in;
		player->setMaxHealth(in);
		ifs >> in;
		player->setHealth(in);
		ifs >> in;
		player->setDamage(in);
		ifs >> in;
		player->setXP(in);
		ifs >> in;
		player->setLevel(in);
		ifs >> in;
		if (in == 1) {
			npc[0]->setActive(true);
		}
		else {
			npc[0]->setActive(false);
		}
		ifs >> in;
		npc[0]->setHealth(in);

		ifs >> in;
		if (in == 1) {
			npc[1]->setActive(true);
		}
		else {
			npc[1]->setActive(false);
		}
		ifs >> in;
		npc[1]->setHealth(in);

		ifs >> in;
		if (in == 1) {
			npc[2]->setActive(true);
		}
		else {
			npc[2]->setActive(false);
		}
		ifs >> in;
		npc[2]->setHealth(in);

		ifs >> in;
		if (in == 1) {
			npc[3]->setActive(true);
		}
		else {
			npc[3]->setActive(false);
		}
		ifs >> in;
		npc[3]->setHealth(in);
		
		ifs >> in;
		if (in == 1) {
			player->setKey(true);
		}
		else {
			player->setKey(false);
		}
		ifs >> in;
		player->setX(in);
		ifs >> in;
		player->setY(in);
	}

	void endGame() {
		system("cls");
		SetConsoleTextAttribute(hConsole, 10);
		std::cout << "CONGRATULATIONS! \nYou Have Gone Through The Entire Dungeon And Have Won The Game!\n";
		char c = _getch();
		system("pause");
		exit(0);
	}
};