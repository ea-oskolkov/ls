#ifndef LS_PROCESSINGFILEENTRIESLONG_H
#define LS_PROCESSINGFILEENTRIESLONG_H

#include <list>
#include "../ProcessingFileEntries.h"

class ProcessingFileEntriesLong: public ProcessingFileEntries {

private:

    struct IdentsInColumns {
        uint8_t hardLink = 0;
        uint8_t groupname = 0;
        uint8_t username = 0;
        uint8_t size = 0;
        uint8_t date1 = 0;
        uint8_t date2 = 0;
        uint8_t date3 = 0;
    };

    FuncSz funcSz = nullptr;
    static constexpr uint64_t defaultBlockSize = 512;
    static constexpr uint64_t defaultBlockSizeLsTool = 1024;
    uint64_t totalBlockCount = 0;
    std::list<std::unique_ptr<FileItem>> listItem;
    IdentsInColumns identsInColumns;

    bool onlyFilepath;

    void sortItems();
    void fillIdentsInColumns();

    static std::string buildFullPath(const char* dirPath, const char* fileName) {
        std::string fullPath;
        fullPath.append(dirPath);
        if (fileName[0]!='\0') {
            if (fullPath.at(fullPath.length()-1)!='/')
                fullPath.append(1, '/');
            fullPath.append(fileName);
        }
        return fullPath;
    }

public:
    explicit ProcessingFileEntriesLong(uint8_t flags);

    void process(const char* dirPath, struct dirent *dirEntry) override;
    void process(struct stat *st, const char* filePath) override;
    void push() override;
    void clear() override;

    void freeList();

    ~ProcessingFileEntriesLong() override;
};


#endif //LS_PROCESSINGFILEENTRIESLONG_H
