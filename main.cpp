#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <direct.h>

#include "main.h"
#include "uncompress.h"

int main(int argc, char** argv) {
    switch (argc) {
case 3 : if (argv[1][0] != 'C' && argv[1][0] != 'U' && argv[1][0] != 'I') {
            std::cout << "Bad argument" << std::endl;
            std::cout << "[program] C/U/I [Archive name]";
            return 0;
        }
        break;
case 4 : if (argv[1][0] != 'E') {
            std::cout << "Bad argument" << std::endl;
            std::cout << "[program] E [Archive name] [File to extract]";
            return 0;
        }
        break;
default : std::cout << "Not enough argument" << std::endl;
        std::cout << "[program] C/U [Archive name]";
        return 0;
    }

    switch (argv[1][0]) {
case 'C' : chdc::createArchive(argv[2]);
    break;
case 'U' : chdc::extractFromArchive(argv[2]);
    break;
case 'I' : break;
case 'E' : break;
    }
    return 0;
}

void strToCstr(char *cStr, std::string str) {
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '/') {
            cStr[i] = '\\';
        }
        else {
            cStr[i] = str[i];
        }
    }
}

std::string cStrToStr(char *cStr, int len) {
    std::string str;
    for (int i = 0; i < len; i++) {
        str[i] = cStr[i];
    }
    return str;
}
