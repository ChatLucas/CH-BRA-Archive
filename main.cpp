#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <direct.h>

#include "main.h"
#include "uncompress.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Not enough argument" << std::endl;
        std::cout << "[program] C/U [Archive name]";
        return 0;
    }

    if (argv[1][0] != 'C' && argv[1][0] != 'V') {
        std::cout << "Bad argument";
        std::cout << "[program] C/U [Archive name]";
        return 0;
    }

    switch (argv[1][0]) {
case 'C' : chdc::createArchive(argv[2]);
    break;
case 'V' : chdc::extractFromArchive(argv[2]);
    break;
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
