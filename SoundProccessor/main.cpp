#include "help.h"
#include "converter.h"

void read_file(std::ifstream& in, sstream* stream1, sstream* stream2, std::vector<std::string> files) { //считываем файл конфига
    while (!in.eof())
    {
        std::string buff, buff2;
        std::getline(in, buff);
        if (buff[0] == '#')
            continue;
        std::stringstream str;
        str << buff;
        str >> buff;
        std::getline(str, buff2);
        Converter* conv = Converter::createConv(buff);
        conv->set_sstreams(stream1, stream2);
        conv->do_work(buff2, files);
    }
}


int main(int argc, char** argv) {
    sstream res1;
    sstream res2;
    std::vector<std::string> files;
    if (argc == 2 && (std::string)argv[1] == "-h") {
        std::cout << "sound_processor -c <config.txt> <output.wav> <input1.wav> [<input2.wav> Е]";
        exit(0);
    }
    if (argc > 4 && (std::string)argv[1] == "-c")
    {
        std::ifstream conf(argv[2]);
        if (!conf.is_open())
            throw std::exception("No config file");

        res1.read(argv[4]);
        res2.read(argv[5]);


        for (int i = 5; i < argc; i++)
            files.push_back(argv[i]);

        read_file(conf, &res1, &res2, files);
        res2.save(argv[3]);
        exit(0);
    }
    throw std::exception("Wrong command, print -h for help");
};