#ifndef LS_FILEITEM_H
#define LS_FILEITEM_H

#include <string>
#include <memory>
#include <ctime>
#include <climits>

#include <sys/stat.h>
#include <csignal>

class FileItem {

private:
    std::string access{};
    uint64_t hardLinkCount{};
    std::string userName{};
    std::string groupName{};
    std::string sizeStr{};
    std::string fileName{};
    std::string linkPath{};

    std::string dateColumn1{};
    std::string dateColumn2{};
    std::string dateColumn3{};

    static std::string getAccessStr(__mode_t mode);
    static std::string getUsername(__uid_t uid);
    static std::string getGroupname(__gid_t _gid);

    static bool showYear(
            const std::tm *inputTm,
            const std::tm *currentTm,
            __time_t inputTime,
            __time_t currentTime
    );

    void fillDateTime(__time_t mtime);

public:

    using FuncSizeToStr = std::string(*)(uint64_t size);


    explicit FileItem(
            struct stat* st,
            const char* fileName,
            const char* linkPath,
            FuncSizeToStr funcSize
    );

    virtual ~FileItem(){};

    const std::string &getAccess() const noexcept;

    uint64_t getHardLinkCount() const noexcept;

    const std::string &getUserName() const noexcept;

    const std::string &getGroupName() const noexcept;

    const std::string &getSizeStr() const noexcept;

    const std::string &getFileName() const noexcept;

    const std::string &getLinkPath() const noexcept;

    const std::string &getDateColumn1() const noexcept;

    const std::string &getDateColumn2() const noexcept;

    const std::string &getDateColumn3() const noexcept;

};

#endif //LS_FILEITEM_H
