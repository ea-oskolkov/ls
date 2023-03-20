#include "ProcessingFileEntriesCreator.h"
#include "../../Parameters/Parameters.h"
#include "../Long/ProcessingFileEntriesLong.h"
#include "../Simply/ProcessingFileEntriesSimply.h"

std::unique_ptr<ProcessingFileEntries> ProcessingFileEntriesCreator::create(uint8_t flags) {
    if (flags & Parameters::longFlag)
        return std::make_unique<ProcessingFileEntriesLong>(flags);

    return std::make_unique<ProcessingFileEntriesSimply>(flags);
}
