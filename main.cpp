#include "Sandbox.h"
using namespace std;

int main()
{
	Sandbox sandbox = Sandbox();

	cout << "==========================================================================\n";
	cout << "    Welcome to Box of Sand for Running of Programs and Such (BSRPS). \n        Read the attached documentation for a list of commands. \n";
	cout << "==========================================================================\n";

	while(!sandbox.isClosed())
	{
		if(sandbox.hasError())
			sandbox.displayErrorMessage();

		if(sandbox.getMode() == "interactive")
		{
			cout << "> ";
			string command;
			getline(cin, command);
			
			sandbox.processInstruction(command);
		}
		else if(sandbox.getMode() == "run_program")
		{
			string program;
			cout << "Enter the filename of your program: ";
			cin >> program;

			sandbox.openAndRun(program);
		}
	}
}