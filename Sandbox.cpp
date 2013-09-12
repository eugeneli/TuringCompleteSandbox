#include "Sandbox.h"

Sandbox::Sandbox()
	: mode("interactive"), jump(0), error(false), exit(false), errorMessage("")
{
}

void Sandbox::openAndRun(string prog)
{
	ifstream progFile(prog);

    if(!progFile)
    {
		error = true;
		errorMessage = "Could not open file.";
    }
	else
	{
		mode = "interactive"; //start in interactive mode
		string instrLine;

		while(getline(progFile, instrLine)) //Store the program in memory to allow for jumping
			instructions.push_back(instrLine);

		for(size_t i = 0; i < instructions.size(); i++)
		{
			processInstruction(instructions[i]); //run the instruction

			//Check if need to jump to other lines of program
			if(jump != 0)
			{
				i += jump;
				jump = 0;

				if(i < 0)
				{
					error = true;
					errorMessage = "Jump is out of bounds. (Too far back)";
				}
				else if (i >= instructions.size())
				{
					error = true;
					errorMessage = "Jump is out of bounds. (Too far ahead)";
				}
			}
		}

		cout << '\n';

		//clear instructions/memory after running them
		instructions.clear();
		memory.clear();
	}
}

void Sandbox::processInstruction(string instr)
{
    istringstream iss(instr);

	while(iss)
	{
		string keyword;
		iss >> keyword;

		if(keyword == "LOAD" || keyword == "load")
		{
			mode = "run_program";
		}
		else if(keyword == "SET") //Set a variable
		{
			string variable;
			string value;

			iss >> variable;
			iss >> value;

			memory[variable] = value;
		}
		else if(keyword == "ADD" || keyword == "SUB" || keyword == "DIV" || keyword == "MUL" || keyword == "POW" || keyword == "SQRT" || keyword == "CP" || 
				keyword == "BEQ" || keyword == "BNE" || keyword == "BGE" || keyword == "BLE")
		{
			string param0;
			string param1;
			string param2;

			double param1Double;
			double param2Double;

			iss >> param0;
			iss >> param1;
			iss >> param2;

			//if the parameters contain a letter, treat it as a variable
			if(containsAlpha(param1))
			{
				if(memory.find(param1) == memory.end())
				{
					error = true;
					errorMessage = "Variable '"+ param1 + "' does not exist.";
					break;
				}
				else
				{
					param1Double = atof(memory[param1].c_str());
				}
			}
			else
				param1Double = atof(param1.c_str());

			if(containsAlpha(param2)) 
			{
				if(memory.find(param2) == memory.end())
				{
					error = true;
					errorMessage = "Variable '"+ param2 + "' does not exist.";
					break;
				}
				else
				{
					param2Double = atof(memory[param2].c_str());
				}
			}
			else
				param2Double = atof(param2.c_str());

			if(!error)
			{
				//Arithmetic
				if(keyword == "ADD")
				{
					double d = param1Double + param2Double;
					memory[param0] = to_string(static_cast<double long>(d));
				}
				else if(keyword == "SUB")
				{
					double d = param1Double - param2Double;
					memory[param0] = to_string(static_cast<double long>(d));
				}
				else if (keyword == "DIV")
				{
					if(param2Double == 0)
					{
						error = true;
						errorMessage = "Dividing by zero is illegal. User reported to the NSA.";
					}
					else
					{
						double d = param1Double / param2Double;
						memory[param0] = to_string(static_cast<double long>(d));
					}
				}
				else if (keyword == "MUL")
				{
					double d = param1Double * param2Double;
					memory[param0] = to_string(static_cast<double long>(d));
				}
				else if (keyword == "POW")
				{
					if(param1Double == 0 && param2Double < 0)
					{
						error = true;
						errorMessage = "Result is undefined";
					}
					else if(param1Double < 0 && param2Double < 1 && param2Double > 0)
					{
						error = true;
						errorMessage = "Negative numbers cannot be raised by a fractional power";
					}
					else
					{
						double d = pow(param1Double,param2Double);
						memory[param0] = to_string(static_cast<double long>(d));
					}
				}
				else if(keyword == "SQRT")
				{
					if(param1Double < 0)
					{
						error = true;
						errorMessage = "Cannot square root negative numbers";
					}
					else
					{
						double d = sqrt(param1Double);
						memory[param0] = to_string(static_cast<double long>(d));
					}
				}
				else if(keyword == "CP")
				{
					memory[param0] = to_string(static_cast<double long>(param1Double));
				}
				else if(keyword == "BEQ" || keyword == "BNE" || keyword == "BGE" || keyword == "BLE")
				{
					int param0Int;
					if(containsAlpha(param0))
					{
						error = true;
						errorMessage = "Invalid parameter for BEQ: "+param0;
						break;
					}
					else
						param0Int = atoi(param0.c_str());

					if(keyword == "BEQ")
					{
						if(param1Double == param2Double)
							jump = param0Int;
					}
					else if(keyword == "BNE")
					{
						if(param1Double != param2Double)
							jump = param0Int;
					}
					else if(keyword =="BGE")
					{
						if(param1Double >= param2Double)
							jump = param0Int;
					}
					else if(keyword == "BLE")
					{
						if(param1Double <= param2Double)
							jump = param0Int;
					}
				}
			}
			else
			{
				displayErrorMessage();
				break;
			}
		}
		else if(keyword == "PRINT")
		{
			string str;
			iss >> str;

			if(memory.find(str) == memory.end())
			{
				error = true;
				errorMessage = "Variable '"+ str + "' does not exist.";
				break;
			}
			else
			{
				cout << floor(atof(memory[str].c_str()) * 1000 + 0.5)/1000 << endl; //convert string from memory to double, then round it
			}
		}
		else if(keyword == "clear")
		{
			cout << string(100, '\n');
		}
		else if (keyword == "exit")
		{
			exit = true;
		}
	}
}

string Sandbox::getMode()
{
	return mode;
}

bool Sandbox::hasError()
{
	return error;
}

bool Sandbox::isClosed()
{
	return exit;
}

string Sandbox::getErrorMessage()
{
	return errorMessage;
}

void Sandbox::clearErrors()
{
	error = false;
	errorMessage = "";
}

void Sandbox::displayErrorMessage()
{
	if(error)
	{
		cout << "Error: " + getErrorMessage() << endl;
		clearErrors();
		instructions.clear();
	}
}

bool Sandbox::containsAlpha(string str)
{
	for(string::size_type i = 0; i < str.size(); ++i)
	{
		if(isalpha(str[i]))
			return true;
	}
	return false;
}