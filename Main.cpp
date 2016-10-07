#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>


std::vector<std::string> parser(std::string command)
{
	std::vector<std::string> command_list;
	auto current = command.begin();
	bool done = false;
	while (!done)
	{
		auto next = std::find(current, command.end(), ' ');
		std::string new_command{ current, next };
		command_list.push_back(new_command);
		if (next != command.end()) current = next + 1;
		else done = true;
	}
	return command_list;
}

int main()
{
	char command[200];
	std::vector < std::string > command_history;
	std::chrono::milliseconds ptime;

	std::cout << "> ";
	std::cin.getline(command, 200);

	//timer example

	//std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	//f();
	//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	//std::chrono::milliseconds result = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	//return result.count();

	while (strcmp(command, "exit") != 0)
	{
		command_history.push_back(command);
		std::vector<std::string> command_input = parser(command);

		//PTIME
		if (command_input[0] == "ptime")
		{
			std::cout << "Total Time (milliseconds) Executing Child Processes: " << ptime.count() << std::endl;

			std::cout << "> ";
			std::cin.getline(command, 200);
			continue;
		}

		//HISTORY
		else if (command_input[0] == "History" || command_input[0] == "history")
		{
			for (int i = 0; i < command_history.size(); i++)
			{
				std::cout << i << ":  " << command_history[i] << std::endl;
			}

			std::cout << "> ";
			std::cin.getline(command, 200);
			continue;
		}

		//RUN ^HISTORY
		else if (command_input[0] == "^")
		{
			int number = std::stoi(command_input[1]);
			command_input = parser(command_history[number]);
		}

		//package ready for process
		std::vector<const char *> char_pointers;
		for (int i = 0; i < command_input.size(); i++)
		{
			const char * c_string = command_input[i].c_str();
			char_pointers.push_back(c_string);
		}
		
		//auto pid = fork();

		//if (pid < 0 ) perror()

		//else if (pid > 0)
		//this is the parent
		//waitpid(pid)
		//add parsed command to the vector of parses

		//else this is the child
		//execpr(blah blah blah ) do stuff HERE!!!
		//perror() (shouldn't hit this point ever!)
		//exit() if it idoes

		//waitpid(pid);

		std::cout << "> ";
		std::cin.getline(command, 200);
	}
}