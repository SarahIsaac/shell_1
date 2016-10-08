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
		// const char * char_array[100];
		// std::vector<const char *> char_commands;
		// for (int i = 0; i < command_input.size(); i++)
		// {
		// 	const char * c_string = command_input[i].c_str();
		// 	//char_commands.push_back(c_string);
		// 	char_array[i] = c_string;
		// }
		// char_commands.push_back(nullptr);

		// std::vector<const char *> char_commands;

		const char * char_array[100];
		int j = 1;
		for (int i = 0; i < command_input.size(); i++)
		{
			const char * c_string = command_input[i].c_str();
			char_array[i] = c_string;
			j++;
			// char_commands.push_back(c_string);
		}
		// char_commands.push_back(nullptr);
		char_array[j] = nullptr;

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
			wait(pid_ptr);
			std::chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();
			ptime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		}

		else
		{
			//this is the child process
			execvp(char_arrray[0], char_arrray);
			perror("Error: "); 	//someething went terribly wrong if we hit this point
			exit(pid); 	//don't break if bad command
		}

		std::cout << "> ";
		std::cin.getline(command, 200);
	}
}
