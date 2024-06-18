#pragma once
#include "help.h"
#include <algorithm>
#include <sstream>



class Converter
{
public:
	sstream* buff1;
	sstream* buff2;
	Converter(sstream* in1, sstream* in2);
	Converter();
	void set_sstreams(sstream* in1, sstream* in2);
	virtual void do_work(std::string, std::vector<std::string>) = 0;
	static Converter* createConv(std::string);
	virtual ~Converter() = default;
};

class Mix : public Converter
{
public:
	void do_work(std::string, std::vector<std::string>);

};

class Mute : public Converter
{
public:
	void do_work(std::string convert_info, std::vector<std::string> files);
};

class Louder :public Converter
{
public:
	void do_work(std::string convert_info, std::vector<std::string> files);
};