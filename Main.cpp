#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

	while (strcmp(command, "exit") != 0)
	{
		command_history.push_back(command);
		std::vector<std::string> command_input = parser(command);

		//PTIME
		if (command_input[0] == "ptime")
		{
			std::cout << "Time (milliseconds) Executing Last Child Process: " << ptime.count() << std::endl;

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

// int execvp(const char *file, char *const argv[]);
		//package ready for process
		std::vector<const char *> char_commands;
		for (int i = 0; i < command_input.size(); i++)
		{
			const char * c_string = command_input[i].c_str();
			char_commands.push_back(c_string);
		}
		char_commands.push_back(NULL);

		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		pid_t pid = fork();

		if (pid < 0)
		{
			perror("Error: ");
		}

		else if (pid > 0)
		{
			//this is the parent
			int * pid_ptr = &pid;

			wait();
			// waitpid(pid_ptr);	//kind of like join for processes
			std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
			ptime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		}

		else
		{
			// std::vector<double> v;
			// double* a = &v[0];

			//this is the child process
			const char * *commmand_array = &char_commands[0];
			execvp(char_commands[0], commmand_array);
			perror("Error: "); 	//someething went terribly wrong if we hit this point
			exit(pid); 	//don't break if bad command
		}

		std::cout << "> ";
		std::cin.getline(command, 200);
	}
}
