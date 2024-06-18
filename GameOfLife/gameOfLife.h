#pragma once
#include "gameOfLife.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <regex>
#include <thread>

typedef struct Point {
	Point() = default;

	Point(bool p_is_life, int xt, int yt)
	{
		point_is_life = p_is_life;
		x = xt;
		y = yt;
	}

	bool point_is_life;
	int x = 0;
	int y = 0;
} point;

class Message {
public:
	void startMessage() {
		std::cout << "Welcome to the game of Life" << std::endl;
		std::cout << "There are two mods: offline and online" << std::endl;
		std::cout << "Online mode: you just start programm and choose commands" << std::endl;
		std::cout << "Offline mode: start it with -file <input file> -i <number of iterations> -o <output file>" << std::endl;
		std::cout << std::endl;
		helpMessage();
//		Sleep(5000);
	}
	void warningMessage() {
		std::cout << "Warning: you must have done something wrong" << std::endl;
		Sleep(500);
	}
	void unknowmcommand() {
		std::cout << "There is no such command" << std::endl;
		helpMessage();
	}
	void helpMessage() {
		std::cout << "There are such commands in the online mode:" << std::endl;
		std::cout << "1. help - show game reference." << std::endl;
		std::cout << "2. dump <filename> - save current universe state." << std::endl;
		std::cout << "3. exit - end the game." << std::endl;
		std::cout << "4. tick <n> - count game state after n iteration." << std::endl;
	}
};

class CommandState {
public:

	~CommandState() = default;

	int readCommand()
	{
		std::cin >> currentCommand;

		//Help command
		if (currentCommand == "help" || currentCommand == "1") {
			return this->help;
		}
		//Dump game field command
		if (currentCommand == "dump" || currentCommand == "2") {
			return this->dump;
		}
		//Show field after n iteration
		if (currentCommand == "exit" || currentCommand == "3") {
			return this->exit;
		}
		//Show game reference
		if (currentCommand == "tick" || currentCommand == "4") {
			return this->tick;
		}
		return -1;
	}
	int help = 1;
	int dump = 2;
	int exit = 3;
	int tick = 4;

private:

	std::string currentCommand;
};

class GameState {
public:

	GameState(std::string file_name)
	{
		std::ifstream fin(file_name.c_str());
		std::regex universeNameRegex("[#N ]([A-Za-z]*)");
		std::regex rulesRegex("(#R )(B[0-9]+\\/S[0-9]+)");
		std::regex sizeRegex("[#Size ]([0-9]+) ([0-9]+)");
		std::regex digits("[0-9]+");
		std::regex letters("[A-Za-z ]+");
		std::smatch s;

		char temp[256];

		//Read a name of universe
		fin.getline(temp, 256);
		if (std::regex_search(temp, universeNameRegex))
		{
			this->universeName = temp;
			this->universeName.erase(this->universeName.find("#N "), 3);
		}

		//Read rules for the game
		fin.getline(temp, 256);
		if (std::regex_search(temp, rulesRegex))
		{
			std::string str(temp);
			auto iter = std::sregex_iterator(str.begin(), str.end(), digits);
			s = *iter;
			this->gameRules = s.str();
			s = *(++iter);
			this->survivalRules = s.str();
		}

		//Read size of field
		fin.getline(temp, 256);
		if (std::regex_search(temp, sizeRegex))
		{
			std::string str(temp);
			auto iter = std::sregex_iterator(str.begin(), str.end(), digits);
			s = *iter;
			this->world_w = std::stol(s.str());
			s = *(++iter);
			this->world_h = std::stol(s.str());
		}

		//Read coordinates of cells
		int x, y;
		while (!fin.eof())
		{
			fin >> x >> y;
			this->cellsCoordinates.push_back(Point(true, x, y));
		}

	}
	std::string gameRules;
	std::string survivalRules;

	int world_h;
	int world_w;

	std::string fileName;
	std::string universeName;
	std::vector <point> cellsCoordinates;

};

class Field {

public:

	Field() = default;
	Field(GameState& _game)
	{
		game = &_game;
	}
	GameState* game;
	~Field() = default;
	std::vector<std::vector<point>> world;

	void init_world();
	void next_world();
	void print_world();
	void show_rools();
	void saveField(std::string outputFile);
private:
	//GameState* game;
	bool should_be_born(int x) {
		size_t found = game->gameRules.find(std::to_string(x));
		if (found != std::string::npos) return true; //found position
		else return false;
	}
	bool should_survive(int x) {
		size_t found = game->survivalRules.find(std::to_string(x));
		if (found != std::string::npos) return true;
		else return false;
	}
};

void Field::show_rools()
{
	std::cout << std::endl;
	std::cout << "Current universe have such rules: " << std::endl;
	std::cout << "#N " << game->universeName << std::endl;
	std::cout << "#R " << "B" << game->gameRules << "/" << "S" << game->survivalRules << std::endl;
	std::cout << "#S " << game->world_w << " " << game->world_h << std::endl;
}
void Field::saveField(std::string outputFile)
{
	std::ofstream fout(outputFile);
	fout << "#N " << game->universeName << std::endl;
	fout << "#R " << "B" << game->gameRules << "/" << "S" << game->survivalRules << std::endl;
	fout << "#S " << game->world_w << " " << game->world_h << std::endl;
	fout << std::endl;

	for (size_t y = 0; y < game->world_h; y++)
	{
		for (size_t i = 0; i < game->world_w; i++)
		{
			if (world[i][y].point_is_life == true)
				fout << "* ";
			else
				fout << "  ";
		}
		fout << std::endl;
	}

}
void Field::next_world()
{
	std::vector<point> temp;

	int neighbours = 0, _x, _y, x_, y_; //x and y on top and bottom(left and right for x axis)

	for (int y = 0; y < game->world_h; y++)
	{
		for (int i = 0; i < game->world_w; i++)
		{
			neighbours = 0;
			bool isAlive = world[i][y].point_is_life;
			if (y == 0 && i == 0) {   //top left
				_x = game->world_w - 1;
				x_ = i + 1;
				_y = game->world_h - 1;
				y_ = y + 1;
			}
			else if (y == 0 && i == game->world_w - 1) { //top right
				_x = i - 1;
				x_ = 0;
				_y = game->world_h - 1;
				y_ = y + 1;
			}
			else if (y == game->world_h - 1 && i == 0) { //bottom left
				_x = game->world_w - 1;
				x_ = i + 1;
				_y = y - 1;
				y_ = 0;
			}
			else if (y == game->world_h - 1 && i == game->world_w - 1) { //bottom right
				_x = i - 1;
				x_ = 0;
				_y = y - 1;
				y_ = 0;
			}
			else if (y == 0) { //top
				_x = i - 1;
				x_ = i + 1;
				_y = game->world_h - 1;
				y_ = y + 1;
			}
			else if (y == game->world_h - 1) { //bottom
				_x = i - 1;
				x_ = i + 1;
				_y = y - 1;
				y_ = 0;
			}
			else if (i == 0) { //left
				_x = game->world_w - 1;
				x_ = i + 1;
				_y = y - 1;
				y_ = y + 1;
			}
			else if (i == game->world_w - 1) { //right
				_x = i - 1;
				x_ = 0;
				_y = game->world_h - 1;
				y_ = y + 1;
			}
			else { //center
				_x = i - 1;
				x_ = i + 1;
				_y = y - 1;
				y_ = y + 1;
			}
			if (world[_x][_y].point_is_life == true) neighbours++;
			if (world[i][_y].point_is_life == true) neighbours++;
			if (world[x_][_y].point_is_life == true) neighbours++;

			if (world[_x][y].point_is_life == true) neighbours++;
			if (world[x_][y].point_is_life == true) neighbours++;

			if (world[_x][y_].point_is_life == true) neighbours++;
			if (world[i][y_].point_is_life == true) neighbours++;
			if (world[x_][y_].point_is_life == true) neighbours++;

			if (should_be_born(neighbours) && !isAlive)
				temp.push_back(Point{ true, i, y });

			if (should_survive(neighbours) && isAlive)
				temp.push_back(Point{ true, i, y });

			if (!should_survive(neighbours) && isAlive)
				temp.push_back(Point{ false, i, y });

		}
	}
	for (auto& x : temp)
	{
		if (x.point_is_life) this->world[x.x][x.y].point_is_life = true;
		else this->world[x.x][x.y].point_is_life = false;
	}
	temp.resize(0);
}



void Field::init_world()
{
	std::vector<point> temp;
	world.reserve(game->world_h);
	for (size_t y = 0; y < game->world_h; y++)
	{
		temp.resize(0);
		for (size_t i = 0; i < game->world_w; i++)
		{
			point z = { 0, 0, 0 };
			temp.push_back(z);
		}
		world.push_back(temp);
	}

	for (auto& x : this->game->cellsCoordinates)
	{
		temp.reserve(game->world_w);
		world[x.x][x.y] = { true, x.x, x.y };
	}

}


void Field::print_world()
{
	for (int y = 0; y < game->world_h; y++) {
		for (int x = 0; x < game->world_w; x++) {
			if (world[x][y].point_is_life == true) {
				std::cout << "* ";
			}
			else
				std::cout << "  ";
		}
		std::cout << std::endl;
	}
}



