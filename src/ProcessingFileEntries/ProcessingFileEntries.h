#ifndef LS_PROCESSINGFILEENTRIES_H
#define LS_PROCESSINGFILEENTRIES_H

#include "../FileItem//FileItem.h"
#include "../FlagsMode.h"

#include <functional>
#include <cstdint>
#include <memory>
#include <list>
#include <string>

#include <dirent.h>

class ProcessingFileEntries {

public:
    explicit ProcessingFileEntries(FlagMode_t flags);

    virtual void process(const char *dirPath, const struct dirent *dirEntry) = 0;

    virtual void process(struct stat *st, const char *filename) = 0;

    virtual void push();

    virtual void clear() = 0;

    virtual ~ProcessingFileEntries();

protected:
    const FlagMode_t flags;
};

#endif //LS_PROCESSINGFILEENTRIES_H
