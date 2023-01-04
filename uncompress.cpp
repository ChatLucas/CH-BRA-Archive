#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <direct.h>

#include "Imported/libs/7z_advancecomp/7z.h"
#include "uncompress.h"

int chdc::extractFromArchive(std::string name) {
    chdc::Header header;

    std::string rootDir = ".\\" + name + "\\";
    std::string arhiveName = name + ".bra";

    std::fstream archive;
    archive.open(arhiveName.c_str(), std::ios::in | std::ios::binary);

    if(!archive.is_open()) {
        fprintf(stdout, "Can't open file.");
        return -1;
    }

    archive.read((char*)&header, sizeof(header));

    chdc::FileInfo files[header.numberOfFiles];

    archive.seekg(header.fileTreeAddress);

    for (int i = 0; i < header.numberOfFiles; i++) {
        archive.read((char*)&files[i], sizeof(int) * 6);
        archive.read(files[i].fileName, files[i].strSize & 0xFF);
        files[i].fileName[files[i].strSize & 0xFF] = 0x00;
    }

    _mkdir(rootDir.c_str());

    for (int i = 0; i < header.numberOfFiles; i++) {
        std::fstream file;
        std::string fileName = rootDir + files[i].fileName;
        chdc::FileHeader fileHeader;

        chdc::makeAllDir(fileName);

        file.open(fileName.c_str(), std::ios::out | std::ios::binary);

        archive.seekg(files[i].startOfFile);
        archive.read((char*)&fileHeader, sizeof(fileHeader));

        char *compressedFile = new char[fileHeader.sizeCompressed];
        char *uncompressedFile = new char[fileHeader.sizeUncompressed];

        archive.read(compressedFile, fileHeader.sizeCompressed);

        int output = decompress_deflate_7z((const unsigned char*)compressedFile, fileHeader.sizeCompressed, (unsigned char*)uncompressedFile, fileHeader.sizeUncompressed);

        if (output = false) {
            std::cout << "Can't uncompress file" << std::endl;
        }

        std::cout << fileName << " " << fileHeader.sizeCompressed << " " << fileHeader.sizeUncompressed << std::endl;

        file.write(uncompressedFile, fileHeader.sizeUncompressed);

        file.close();
        delete compressedFile;
        delete uncompressedFile;
    }

    archive.close();
    return 1;
}

void chdc::makeAllDir(std::string fileName) {
    std::string curDir;
    int posInStr;
    int div = 0;
    for (long unsigned int i = 0; i < fileName.length(); i++) {
        if (fileName[i] == '\\') {
            div++;
        }
    }
    if (div == 0) {
        return;
    }
    posInStr = fileName.find_first_of("\\", 0) + 1;
    curDir = ".\\" + fileName.substr(0, posInStr);
    fileName = fileName.substr(posInStr, fileName.size() - posInStr);
    for (int i = 1; i < div; i++) {
        posInStr = fileName.find_first_of("\\", 0) + 1;
        curDir += fileName.substr(0, posInStr);
        fileName = fileName.substr(posInStr, fileName.size() - posInStr);
        _mkdir(curDir.c_str());
    }
}
