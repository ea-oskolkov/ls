#ifndef LS_PROCESSINGFILEENTRIESSIMPLY_H
#define LS_PROCESSINGFILEENTRIESSIMPLY_H

#include "../ProcessingFileEntries.h"

#include <list>
#include <string>

class ProcessingFileEntriesSimply final: public ProcessingFileEntries {

public:
    using ProcessingFileEntries::ProcessingFileEntries;

    void process(const char *dirPath, const struct dirent *dirEntry) override;
    void process(struct stat *st, const char *filename) override;
    void push() override;
    void clear() override;

    ~ProcessingFileEntriesSimply() override;

private:
    void sort();
    std::list<std::string> listItem{};

};

#endif //LS_PROCESSINGFILEENTRIESSIMPLY_H
