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
		bool isClosed();

		bool hasError();
		void setErrorMessage(string message);
		string getErrorMessage();
		void displayErrorMessage();
		void clearErrors();

	private:
        static const int MAX_MEMORY = 512;
        string mode;
		int jump;
		bool error;
		bool exit;
		string errorMessage;

		map<string, double> memory;
		vector<string> instructions;

		void saveToMemory(string var, double value);
		bool validVariable(string str);
		bool containsAlpha(string str);

};
#endif SANDBOX_H
