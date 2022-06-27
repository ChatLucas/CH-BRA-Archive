#ifndef UNCOMPRESS_H
#define UNCOMPRESS_H

#define CURF 0

namespace chdc {
//namespace arc{

int extractFromArchive(std::string name);
void makeAllDir(std::string fileName);

typedef struct Header Header;
struct Header {
    char type[4];
    int value;
    int fileTreeAddress;
    int numberOfFiles;
};

typedef struct FileInfo FileInfo;
struct FileInfo {
    int value[3];
    int fileSize;
    int strSize;
    int startOfFile;
    char fileName[128];
    bool isDuplicate = false;
    std::string hash_md5;
    int original;
};

typedef struct FileHeader FileHeader;
struct FileHeader {
    int sizeUncompressed;
    int sizeCompressed;
    int value[2];
};

int createArchive(std::string dirName);
int findFiles(std::string dirName, int &nbFile, FileInfo *fileInfo);

//}
}

#endif // UNCOMPRESS_H
