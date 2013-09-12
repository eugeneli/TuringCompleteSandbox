#ifndef SANDBOX_H
#define SANDBOX_H
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <math.h>
using namespace std;

class Sandbox
{
	public:
		Sandbox();
		void prompt();
		void openAndRun(string prog);
		void processInstruction(string instr);
		string getMode();
		bool hasError();
		bool isClosed();
		string getErrorMessage();
		void displayErrorMessage();
		void clearErrors();

	private:
		map<string, string> memory;
		vector<string> instructions;
		string mode;
		int jump;
		bool error;
		bool exit;
		string errorMessage;

		bool containsAlpha(string str);
};

#endif SANDBOX_H
