#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

#include <direct.h>
#include <dirent.h>

#include "Imported/libs/7z_advancecomp/7z.h"
#include "uncompress.h"
#include "main.h"
#include "md5.h"

int chdc::createArchive(std::string dirName) {
    std::fstream archive;
    std::string rootDir = dirName + "\\";
    std::string arhiveName = dirName + ".bra";
    chdc::Header header;
    chdc::FileInfo fileInfo[3000];

    unsigned int uncSize = 0;
    unsigned int cSize = 0;

    header.value = 2;
    header.numberOfFiles = 0;
    header.fileTreeAddress = 0;
    strToCstr(header.type, "PDA");
    header.type[3] = 0x00;

    findFiles(dirName, header.numberOfFiles, fileInfo);

    archive.open(arhiveName.c_str(), std::ios::out | std::ios::binary);
/*
    for (int i = 0; i < header.numberOfFiles; i++) {
        std::string curFileName = rootDir + fileInfo[i].fileName;
        std::fstream file;
        file.open(curFileName, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Can't open file : " << curFileName << std::endl;
            return -1;
        }
        fileInfo[i].fileSize = file.tellg();
        file.seekg(0, std::ios::end);
        fileInfo[i].fileSize = file.tellg() - fileInfo[i].fileSize;
        file.seekg(0, std::ios::beg);

        file.close();
    }
*/
    for (int i = 0; i < header.numberOfFiles; i++) {
        std::string curFileName = rootDir + fileInfo[i].fileName;
        std::fstream file;
        file.open(curFileName, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Can't open file : " << curFileName << std::endl;
            return -1;
        }
        fileInfo[i].fileSize = file.tellg();
        file.seekg(0, std::ios::end);
        fileInfo[i].fileSize = file.tellg() - fileInfo[i].fileSize;
        file.seekg(0, std::ios::beg);

        char *cFile = new char[fileInfo[i].fileSize];
        file.read(cFile, fileInfo[i].fileSize);

        std::string sFile(cFile, fileInfo[i].fileSize);

        fileInfo[i].hash_md5 = md5(sFile);

        //std::cout << fileInfo[i].fileSize << std::endl;

        file.close();
    }

    for (int i = 0; i < header.numberOfFiles - 1; i++) {
        for (int j = i + 1; j < header.numberOfFiles; j++) {
            if (fileInfo[i].hash_md5 == fileInfo[j].hash_md5) {
                std::cout << "Same : " << fileInfo[i].fileName << ", " << fileInfo[j].fileName << std::endl;
                fileInfo[j].isDuplicate = true;
                fileInfo[j].original = i;
                break;
            }
        }
    }

    archive.write((char*)&header, sizeof(header));

    for (int i = 0; i < header.numberOfFiles; i++) {
        std::fstream curFile;
        std::string curFileName = rootDir + fileInfo[i].fileName;
        chdc::FileHeader fileHeader;

        curFile.open(curFileName.c_str(), std::ios::in | std::ios::binary);

        if (!curFile.is_open()) {
            std::cout << "Can't open file." << std::endl;
            std::cout << curFileName << std::endl;
            return -1;
        }

        if (!fileInfo[i].isDuplicate) {
            fileHeader.sizeUncompressed = fileInfo[i].fileSize;
            char *uncompressedFile = new char[fileHeader.sizeUncompressed];
            curFile.read(uncompressedFile, fileHeader.sizeUncompressed);
            char *compressedFile = new char[fileHeader.sizeUncompressed];
            unsigned sizeCompressed = fileHeader.sizeUncompressed;

            int output = compress_deflate_7z((const unsigned char*)uncompressedFile, (unsigned)fileHeader.sizeUncompressed, (unsigned char*)compressedFile, sizeCompressed, 1, 258);

            fileHeader.sizeCompressed = sizeCompressed;
            fileHeader.value[1] = 6;

            fileInfo[i].startOfFile = archive.tellg();

            if (output == false) {
                std::cout << curFileName << " " << "Can't compress file." << std::endl;
            }

            archive.write((char*)&fileHeader, sizeof(fileHeader));
            archive.write(compressedFile, fileHeader.sizeCompressed);

            uncSize += fileHeader.sizeUncompressed;
            cSize += fileHeader.sizeCompressed;
            float comp = (float)cSize / (float)uncSize;
            delete compressedFile;
            delete uncompressedFile;
        }
        else {
            fileInfo[i].startOfFile = fileInfo[fileInfo[i].original].startOfFile;
            std::cout << "Skip : ";
        }

        std::cout << curFileName << std::endl;

        //break;

        curFile.close();
    }

    header.fileTreeAddress = archive.tellg();

    for (int i = 0; i < header.numberOfFiles; i++) {
        archive.write((char*)&fileInfo[i], sizeof(int) * 6);
        archive.write(fileInfo[i].fileName, fileInfo[i].strSize);
    }

    archive.seekg(0, std::ios::beg);
    archive.write((char*)&header, sizeof(header));

    archive.close();
    return 1;
}

int chdc::findFiles(std::string dirName, int &nbFile, chdc::FileInfo *fileInfo) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(dirName.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != '.') {
                chdc::findFiles(dirName + "/" + ent->d_name, nbFile, fileInfo);
            }
        }
    }
    else {
        int posInStr = dirName.find_first_of("/", 0) + 1;
        dirName = dirName.substr(posInStr, dirName.size() - posInStr);
        strToCstr(fileInfo[nbFile].fileName, dirName);
        fileInfo[nbFile].strSize = dirName.size();
        //std::cout << fileInfo[nbFile].fileName << std::endl;
        nbFile++;
        //std::cout << dirName.substr(posInStr, dirName.size() - posInStr) << std::endl;
        return -1;
    }
    return 0;
}

