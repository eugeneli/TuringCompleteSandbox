#include "Sandbox.h"

Sandbox::Sandbox()
: mode("interactive"), jump(0), error(false), exit(false), errorMessage("")
{
}

void Sandbox::openAndRun(string prog)
{
    ifstream progFile(prog.c_str());

    if(!progFile)
    {
        mode = "interactive";
        setErrorMessage("Could not open file");
    }
	else
	{
		mode = "interactive"; //start in interactive mode
		string instrLine;

		while(getline(progFile, instrLine)) //Store the program in memory to allow for jumping around
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
				    setErrorMessage("Jump is out of bounds. (Too far back)");
				else if (i >= instructions.size())
				    setErrorMessage("Jump is out of bounds. (Too far ahead)");
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
		else if(keyword == "ADD" || keyword == "SUB" || keyword == "DIV" || keyword == "MUL" || keyword == "POW" || keyword == "SQRT" ||
                keyword == "CP" || keyword == "SET"||keyword == "BEQ" || keyword == "BNE" || keyword == "BGE" || keyword == "BLE" || keyword == "BGT" || keyword == "BLT")
		{
			string param0;
			string param1;
			string param2;

			double param1Double;
			double param2Double;

			iss >> param0;
			iss >> param1;
			iss >> param2;

			if(!validVariable(param0) && !(keyword == "BEQ" || keyword == "BNE" || keyword == "BGE" || keyword == "BLE" || keyword == "BGT" || keyword == "BLT")) //Make exceptions for branch instructions. Really ugly ;_;
			{
			    setErrorMessage("Invalid variable name");
			    break;
			}

			//if the parameters contain a letter, treat it as a variable
			if(containsAlpha(param1))
			{
				if(memory.find(param1) == memory.end())
				{
				    setErrorMessage("Variable '"+ param1 + "' does not exist.");
				    break;
				}
				else
					param1Double = memory[param1];
			}
			else
				param1Double = atof(param1.c_str()); //convert the string input into double

			if(containsAlpha(param2))
			{
				if(memory.find(param2) == memory.end())
				{
				    setErrorMessage("Variable '"+ param2 + "' does not exist.");
				    break;
				}
				else
					param2Double = memory[param2];
			}
			else
				param2Double = atof(param2.c_str());

            //If no error, do the operations
			if(!error)
			{
			    if(keyword == "SET") //Set a variable
                {
                    saveToMemory(param0, param1Double);
                }
				//Arithmetic
				if(keyword == "ADD")
				{
				    saveToMemory(param0, param1Double + param2Double);
				}
				else if(keyword == "SUB")
				{
				    saveToMemory(param0, param1Double - param2Double);
				}
				else if (keyword == "DIV")
				{
					if(param2Double == 0)
					    setErrorMessage("Dividing by zero is illegal. User reported to the NSA.");
					else
                        saveToMemory(param0, param1Double / param2Double);
				}
				else if (keyword == "MUL")
				{
				    saveToMemory(param0, param1Double * param2Double);
				}
				else if (keyword == "POW")
				{
					if(param1Double == 0 && param2Double < 0)
					    setErrorMessage("Result is undefined");
					else if(param1Double < 0 && param2Double < 1 && param2Double > 0)
					    setErrorMessage("Negative numbers cannot be raised by a fractional power");
					else
                        saveToMemory(param0, pow(param1Double,param2Double));
				}
				else if(keyword == "SQRT")
				{
					if(param1Double < 0)
					    setErrorMessage("Cannot square root negative numbers");
					else
                        saveToMemory(param0, sqrt(param1Double));
				}
				else if(keyword == "CP")
				{
				    saveToMemory(param0, param1Double);
				}
				else if(keyword == "BEQ" || keyword == "BNE" || keyword == "BGE" || keyword == "BLE" || keyword == "BGT" || keyword == "BLT")
				{
					int param0Int;
					if(containsAlpha(param0))
					{
						setErrorMessage("Invalid parameter for BEQ: "+param0);
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
					else if(keyword == "BGT")
					{
					    if(param1Double > param2Double)
							jump = param0Int;
					}
					else if(keyword == "BLT")
					{
					    if(param1Double < param2Double)
							jump = param0Int;
					}
				}
			}
			else
				displayErrorMessage();
		}
		else if(keyword == "PRINT")
		{
			string str;
			iss >> str;

			if(memory.find(str) == memory.end())
				setErrorMessage("Variable '"+ str + "' does not exist.");
			else
				cout << floor((memory[str]) * 1000 + 0.5)/1000 << endl; //round it
		}
		else if(keyword == "clear")
		{
			cout << string(100, '\n');
		}
		else if(keyword == "clearmem")
		{
		    memory.clear();
		    cout << "Memory cleared\n";
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

void Sandbox::setErrorMessage(string message)
{
    error = true;
	errorMessage = message;
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

void Sandbox::saveToMemory(string var, double value)
{
    if(memory.size() < MAX_MEMORY)
    {
        if(validVariable(var))
            memory[var] = value;
        else
            setErrorMessage("Invalid variable name");
    }
    else setErrorMessage("Memory limit hit");
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

bool Sandbox::validVariable(string var)
{
    bool valid = true;
    for(string::size_type i = 0; i < var.size(); ++i)
    {
        if(!isalpha(var[i]) && !isdigit(var[i]))
            valid = false;
    }
    return valid;
}