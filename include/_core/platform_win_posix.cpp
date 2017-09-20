//-----------------------------------------------------------------------------
// File: platform_win_posix.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include "platform_win_posix.h"

#ifndef _WINDOWS
#include <sys/time.h>
#include <QUrl>
#include <QApplication>
#include <unistd.h>

#define MAX_PATH    2048
#endif

MP_Platform_WIN_POSIX::MP_Platform_WIN_POSIX()
{
#ifndef _WINDOWS
    dir = NULL;
#endif
}

MP_Platform_WIN_POSIX::~MP_Platform_WIN_POSIX()
{
}

// eng: Returns time in milliseconds
// rus: Возвращает время в милисекундах
unsigned long MP_Platform_WIN_POSIX::GetTick()
{
    timeval time_val;
    gettimeofday(&time_val, NULL);
    return time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
}

// eng: Returns path to folder with emitters
// rus: Возвращает путь к папке с ptc-файлами
const char *MP_Platform_WIN_POSIX::GetPathToPTC()
{
    if (ptc_path.empty()) {
        QString pwd("");
        QUrl appPath(QString("%1").arg(QApplication::applicationDirPath()));
        pwd = appPath.toDisplayString();
        ptc_path = pwd.toLatin1().data();
        ptc_path += '/';
        ptc_path += "particles2d";
        ptc_path += '/';
    }

    return ptc_path.c_str();
}

// eng: Returns path to folder with textures
// rus: Возвращает путь к папке с текстурами
const char *MP_Platform_WIN_POSIX::GetPathToTexture()
{
    if (texture_path.empty()) {
        texture_path = GetPathToPTC();
        texture_path += "textures/";
    }

    return texture_path.c_str();
}

// eng: Returns path to folder which could be used by wrapper to store temporary files. If NULL is returned the temporary files are saved in RAM.
// rus: Возвращает путь к временной папке. Если вернуть NULL, то временные файлы будут сохраняться в ОЗУ.
const char *MP_Platform_WIN_POSIX::GetPathToTemp()
{
    return NULL;
}

// eng: Finds the first ptc-file in emitters folder
// rus: Поиск первого ptc-файла в папке с эмиттерами
const char *MP_Platform_WIN_POSIX::GetFirstFile()
{
    std::string ptc_path = GetPathToPTC();

    dir = opendir(ptc_path.c_str());
    if (dir)
        return GetNextFile();

    return NULL;
}

// eng: Finds the next ptc-file in emitters folder
// rus: Поиск следующего ptc-файла в папке с эмиттерами
const char *MP_Platform_WIN_POSIX::GetNextFile()
{
    dirent *result = readdir(dir);
    while (result) {
        int len = strlen(result->d_name);
        if (len > 4) {
            const char *ext = &(result->d_name[len - 4]);
            if (strcmp(ext, ".ptc") == 0) {
                return result->d_name;
            }
        }
        result = readdir(dir);
    }
    closedir(dir);
    dir = NULL;

    return NULL;
}

// ru: Удаление файла (аналог "remove")
// en: Deleting of file (analogue "remove")
int MP_Platform_WIN_POSIX::RemoveFile(const char *file)
{
    return remove(file);
}
