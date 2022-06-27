#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <direct.h>

#include "main.h"
#include "uncompress.h"

int main(int argc, char** argv) {
    //chdc::extractFromArchive("Script");
    chdc::createArchive("Script");
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
