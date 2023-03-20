#ifndef LS_FILEITEM_H
#define LS_FILEITEM_H

#include <string>
#include <memory.h>

#include <sys/stat.h>
#include <memory>
#include <ctime>
#include <optional>

using FuncSz = std::string(*)(uint64_t sz);

class FileItem {

private:
    std::string access;
    uint64_t hardLinkCount;
    std::string userName;
    std::string groupName;
    std::string sizeStr;
    std::string fileName;
    std::string linkPath;

    std::string dateColumn1;
    std::string dateColumn2;
    std::string dateColumn3;

    static std::string getAccessStr(__mode_t mode);
    void fillDateTime(__time_t mtime);
    static std::string getUsername(__uid_t uid);
    static std::string getGroupname(__gid_t _gid);

    static bool showYear(std::tm* inputTm, std::tm* currentTm, __time_t inputTime, __time_t currentTime);
    FileItem();
public:

    static std::optional<std::unique_ptr<FileItem>> make(struct stat* st, const char* fileName, const char* fullPath,
            FuncSz funcSz);

    const std::string &getAccess() const;

    uint64_t getHardLinkCount() const;

    const std::string &getUserName() const;

    const std::string &getGroupName() const;

    const std::string &getSizeStr() const;

    const std::string &getFileName() const;

    const std::string &getLinkPath() const;

    const std::string &getDateColumn1() const;

    const std::string &getDateColumn2() const;

    const std::string &getDateColumn3() const;

};


#endif //LS_FILEITEM_H
