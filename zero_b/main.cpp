#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <cctype>


using namespace std;

bool cmp(pair<string, int>& a, pair<string, int>& b) { //комапаратор
    return a.second > b.second;
}

int main(int argc, char* argv[]){
    setlocale(LC_ALL, "Russian");
    ifstream fin;
    string path = argv[1]; //исходный файл txt
    fin.open(path);

    ofstream fout;  
    string path1 = argv[2];  //выходной файл csv
    fout.open(path1);

    map<string, int> freqs;
    string line;

    int wcount = 0;
    fout << "Слово" << ";" << "Частота" << ";" << "Частота (в %)\n";
    string word = "";

    while (getline(fin, line)) {
        for (auto &x : line) {
            if (isalnum(x)) {
                word += x;
            }
            else {
                if (!word.empty()) {
                    ++freqs[word];
                    word = "";
                    wcount++;
                }
            }
        }
        if (!word.empty()) {
            ++freqs[word];
            word = "";
            wcount++;
        }
    }

    vector<pair<string, int>> A;
    for (auto& it : freqs) {
        A.push_back(it);
    }

    sort(A.begin(), A.end(), cmp);

  
    for (auto& it : A) {
        //double percent = ((100 * it.second) / (double)(wcount));
        double percent = ((double)it.second/wcount*100);
        fout << it.first << ";" << it.second << ";" << percent << "%" << "\n";
    }


    fin.close();
    fout.close();

    return 0;
}