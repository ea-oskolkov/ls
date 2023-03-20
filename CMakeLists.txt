cmake_minimum_required(VERSION 3.24)
project(ls)

set(CMAKE_CXX_STANDARD 17)

add_executable(ls
        main.cpp
        Parameters/Parameters.cpp
        Parameters/Parameters.h
        ProcessingFileEntries/ProcessingFileEntries.cpp
        ProcessingFileEntries/ProcessingFileEntries.h
        Auxiliary/Auxiliary.cpp Auxiliary/Auxiliary.h
        FileItem/FileItem.cpp FileItem/FileItem.h
        ProcessingFileEntries/Simply/ProcessingFileEntriesSimply.cpp
        ProcessingFileEntries/Simply/ProcessingFileEntriesSimply.h
        ProcessingFileEntries/Long/ProcessingFileEntriesLong.cpp
        ProcessingFileEntries/Long/ProcessingFileEntriesLong.h
        ProcessingFileEntries/Creator/ProcessingFileEntriesCreator.cpp
        ProcessingFileEntries/Creator/ProcessingFileEntriesCreator.h)
