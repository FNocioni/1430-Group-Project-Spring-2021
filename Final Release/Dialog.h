#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Character.h"

using namespace std;

class Dialog : public Character{
private:
	char *c;
	ifstream ifs; 
	string *dialogs;
	int numOfLines;

public:
	Dialog() { }

	Dialog(int a, int b, int c) : Character(a, b, c) {
		readDialog();
	}

	void readDialog() {
		ifs.open("Bin/Dlg/Dialog.txt");
		numOfLines = getLines(ifs);
		dialogs = new string[getLines(ifs)];

		for (int i = 0; i < numOfLines; i++) {
			getline(ifs, dialogs[i]);
		}
	}

	string readLine(int i) {
		return dialogs[i];
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


};