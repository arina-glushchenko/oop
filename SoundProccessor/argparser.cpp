#include "argparser.h"

void use_cnf(std::ifstream& in, sstream* stream, std::vector<std::string> files) {
    while (!in.eof())
    {
        std::string buff, buff2;
        std::getline(in, buff);
        if (buff[0] == '#')
            continue;
        std::stringstream str;
        str << buff;
        str >> buff; //������ ����� �� ������
        std::getline(str, buff2); //��� ��������� ����� ������
        Converter* conv = Converter::createConv(buff);
        conv->set_sstream(stream);
        conv->work(buff2, files);
    }
}

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
	if (str == buff2) //Nothing changed
		throw std::exception("Not valid argument");
	if (arg[0] == '$')
		arg.erase(arg.begin());
	return std::stoi(arg);
}