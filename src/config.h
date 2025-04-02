#pragma once
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctime>
#include <iostream>

double last_modification_time(const std::string& path) 
{
    struct _stat s;
    if (_stat(path.c_str(), &s) < 0) {
        std::cerr << "File not found or could not access: " << path << "\n";
        return -1.0;
    }

    return static_cast<double>(s.st_mtime);
}

double get_seconds() { return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count(); }