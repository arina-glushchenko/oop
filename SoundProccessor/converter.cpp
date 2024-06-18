#include "converter.h"
#include "help.h"

int arg_parser(std::string& str)
{
    std::string arg;
    std::stringstream buff;
    buff << str;
    buff >> arg;
    std::string buff2;
    buff2 = str;
    str.clear();
    std::getline(buff, str);
    if (str == buff2)
        throw std::exception("Not valid argument");
    if (arg[0] == '$')
        arg.erase(arg.begin());
    return std::stoi(arg);
}

void Mix::do_work(std::string convert_info, std::vector<std::string> files) {
	int filenumber = arg_parser(convert_info) - 2;
	int sec = arg_parser(convert_info);
	sec *= buff1->header.sampleRate;

	int end = std::min(sec, (int)buff1->signals.size());
	for (int i = 0; i < end; i++) {
		if (i >= buff2->signals.size())
			break;
		else {
			buff1->signals[i] = (buff1->signals[i] + buff2->signals[i]) / 2;
			buff2->signals[i] = (buff1->signals[i] + buff2->signals[i]) / 2;
		}
	}

}

void Mute::do_work(std::string convert_info, std::vector<std::string> files) {
	int st, en;
	st = arg_parser(convert_info);
	en = arg_parser(convert_info);
	en = std::min((int)buff1->signals.size(), (int)(en * buff1->header.sampleRate));
	st *= buff1->header.sampleRate;
	for (int i = st; i < en; i++) {
		buff1->signals[i] = 0;
		buff2->signals[i] = 0;
	}
}

void Louder::do_work(std::string convert_info, std::vector<std::string> files) {
	int st, en;
	st = arg_parser(convert_info);
	en = arg_parser(convert_info);
	en = std::min((int)buff1->signals.size(), (int)(en * buff1->header.sampleRate));
	st *= buff2->header.sampleRate;
	for (int i = st; i < en; i++)
		buff2->signals[i] *= 1.8;
}

void Converter::set_sstreams(sstream* in1, sstream* in2)
{
	if (in1  == NULL || in2 == NULL)
		throw std::exception("No stream");
	buff1 = in1;
	buff2 = in2;
}

Converter::Converter(sstream* in1, sstream* in2)
{
	if (in1 == NULL || in2 == NULL)
		throw std::exception("No stream");
	buff1 = in1;
	buff2 = in2;
}

Converter::Converter()
{
	buff1 = NULL;
	buff2 = NULL;

}
Converter* Converter::createConv(std::string str)
{
	if (str == "mix")
		return new Mix();
	if (str == "mute")
		return new Mute();
	if (str == "loud")
		return new Louder();
	throw std::exception("Converter not found");
}