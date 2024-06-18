#include "parser.h"

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