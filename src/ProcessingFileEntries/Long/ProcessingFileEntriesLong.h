#ifndef LS_PROCESSINGFILEENTRIESLONG_H
#define LS_PROCESSINGFILEENTRIESLONG_H

#include "../ProcessingFileEntries.h"

#include <list>

class ProcessingFileEntriesLong final: public ProcessingFileEntries {

public:
    explicit ProcessingFileEntriesLong(FlagMode_t flags);

    void process(const char* dirPath, const struct dirent *dirEntry) override;
    void process(struct stat *st, const char* fileName) override;
    void push() override;
    void clear() override;

    ~ProcessingFileEntriesLong() override;

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

    FileItem::FuncSizeToStr funcSizeToStr = nullptr;
    static constexpr uint64_t defaultBlockSize = 512;
    static constexpr uint64_t defaultBlockSizeLsTool = 1024;
    uint64_t totalBlockCount = 0;
    std::list<std::unique_ptr<FileItem>> listItem{};
    IdentsInColumns identsInColumns{};

    bool onlyFilepath = false;

    void sortItems();
    void fillIdentsInColumns();

    static std::string buildFullPath(const char* dirPath, const char* fileName);


};


#endif //LS_PROCESSINGFILEENTRIESLONG_H
