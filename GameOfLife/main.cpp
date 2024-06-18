#include "gameOfLife.h"


int main(int argc, char** argv) {
	int commandArg = 0, iterations = 0, gameMode = 1;
	std::string inputFile, outputFile;
	Message mym;
	mym.startMessage();

	if (argc > 1) {
		for (int i = 1; i < argc; i++)
		{
			gameMode = 0;
			if (argv[i] == std::string("-i"))
			{
				iterations = std::stoi(argv[i + 1]);
			}

			if (argv[i] == std::string("-o"))
			{
				std::string temp = argv[i + 1];
				outputFile = std::string(temp.begin(), temp.end());
			}
			if (argv[i] == std::string("-file"))
			{
				std::string temp = argv[i + 1];
				inputFile = std::string(temp.begin(), temp.end());
			}
		}
	}

	std::string file;
	if (inputFile.empty())
		file = "try.txt";
	else
		file = inputFile;

	GameState mylife(file);
	Field field(mylife);
	field.init_world();
	field.show_rools();
	Sleep(1000);
	int command;
	if (gameMode == 1) {
		CommandState com;

		while (true) {
			command = com.readCommand();
			if (command == com.dump) {
				std::cin >> outputFile;
				field.saveField(outputFile);
				std::cout << "file saved in: " << outputFile << std::endl;
			}
			else if (command == com.exit) {
				std::cout << "Thank you for playing, hope to see you again" << std::endl;
				exit(0);
			}
			else if (command == com.help) {
				mym.helpMessage();

			}
			else if (command == com.tick) {
				std::cin >> iterations;
				while (iterations > 0) {
					system("cls");
					field.next_world();
					field.print_world();
					Sleep(100);
					iterations--;

				}

			}
			else if (command == -1) {
				mym.warningMessage();
				mym.unknowmcommand();

			}

		}
	}
	else {
		while (iterations > 0) {
			system("cls");
			field.next_world();
			field.print_world();
			Sleep(100);
			//field.next_world();
			iterations--;


		}
		field.saveField(outputFile);
		std::cout << "file saved in: " << outputFile;
	}

	return 0;
}