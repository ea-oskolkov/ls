#ifndef LS_PROCESSINGFILEENTRIESCREATOR_H
#define LS_PROCESSINGFILEENTRIESCREATOR_H

#include "../ProcessingFileEntries.h"
#include <memory>

class ProcessingFileEntriesCreator {
public:
    static std::unique_ptr<ProcessingFileEntries> create(uint8_t flags);

};

#endif //LS_PROCESSINGFILEENTRIESCREATOR_H
