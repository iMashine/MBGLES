//-----------------------------------------------------------------------------
// File: mp.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES
#define MAGIC_PARTICLES

#define SHADER_WRAP

#ifdef SHADER_WRAP
#define SHADER_ALPHATEST_WRAP
#endif

#define SEPARATE_ATTRIBUTES_WRAP

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "src/shaderprogram.h"
#include "src/ptextures.h"
#include <QFile>
#include <string>
#include "magic.h"

class MPEmitter;
class MP_Manager;
class MP_Atlas;
class MP_Device;
struct MP_PTC;

#define MAGIC_INTERPOLATION_DEFAULT         0   // eng: interpolation mode is taken from emitter settings
// rus: режим интерполяции берется из эмиттера
#define MAGIC_INTERPOLATION_ENABLE          1   // eng: interpolation is always enabled
// rus: всегда использовать интерполяцию
#define MAGIC_INTERPOLATION_DISABLE         2   // eng: interpolation is always disabled
// rus: всегда отключать интерполяцию

#define MAGIC_CHANGE_EMITTER_ONLY           0   // eng: preserve particle positions while changing emitter position or direction
// rus: при изменении позиции или направления эмиттера частицы остаются на прежнем месте
#define MAGIC_CHANGE_EMITTER_AND_PARTICLES  1   // eng: move all the special effect when changing emitter position or direction
// rus: при изменении позиции или направления эмиттера весь спецэффект перемещается целиком
#define MAGIC_CHANGE_EMITTER_DEFAULT        2   // eng: for each special effect setting specified in editor is used
// rus: не изменять настройку по умолчанию


extern bool MAGIC_VERTEX_FORMAT_IsEqual(MAGIC_VERTEX_FORMAT *f1, MAGIC_VERTEX_FORMAT *f2);

// ------------------------------------------------------------------------------------------

class MP_Platform
{
protected:
    std::string ptc_path;
    std::string texture_path;
    std::string temp_path;

    std::string wchar_t8;

public:
    static MP_Manager *MP;

public:
    MP_Platform();
    virtual ~MP_Platform();

    // eng: Returns time in milliseconds
    // rus: Возвращает время в милисекундах
    virtual unsigned long GetTick() = 0;

    // eng: Returns path to folder with emitters
    // rus: Возвращает путь к папке с ptc-файлами
    virtual const char *GetPathToPTC() = 0;

    // eng: Returns path to folder with textures
    // rus: Возвращает путь к папке с текстурами
    virtual const char *GetPathToTexture() = 0;

    // eng: Returns path to folder which could be used by wrapper to store temporary files
    // rus: Возвращает путь к временной папке
    virtual const char *GetPathToTemp() = 0;

    // eng: Finds the first ptc-file in emitters folder
    // rus: Поиск первого ptc-файла в папке с эмиттерами
    virtual const char *GetFirstFile()
    {
        return NULL;
    }

    // eng: Finds the next ptc-file in emitters folder
    // rus: Поиск очередного ptc-файла в папке с эмиттерами
    virtual const char *GetNextFile()
    {
        return NULL;
    }

    // eng: Converts wchar_t string into utf8
    // rus: Конвертирует строку типа wchar_t в строку типа utf8
    virtual const char *wchar_to_utf8(const wchar_t *str);

    // eng: Converts utf8 string into wchar_t
    // rus: Конвертирует строку типа utf8 в строку типа wchar_t
    virtual const wchar_t *utf8_to_wchar(const char *str);

    // eng: Opens ptc-file
    // rus: Открывает ptc-файл
    virtual HM_FILE OpenPTC(const char *ptc_file);

    // Deleting of file (analogue remove())
    // Удаление файла (аналог remove())
    virtual int RemoveFile(const char *file) = 0;

    // Loading of file
    // Чтение файла
    virtual void *LoadFile(const char *file);

    // Saving of file
    // Сохранение файла
    virtual bool SaveFile(const char *file);
};

// eng: Class that is used as storage for Magic Particles emitters
// rus: Класс, который является хранилищем эмиттеров Magic Particles
class MP_Manager
{
protected:
    MP_Platform *platform;  // eng: object implementing OS specifics
    // rus: объект работающий со спецификой OS

    // eng: number of loaded emitters
    // rus: количество загруженных эмиттеров
    int k_emitter;
    int max_emitter;

    // eng: list of loaded emitters
    // rus: список загруженных эмиттеров
    MPEmitter **m_emitter;
    HM_EMITTER *m_descriptor;

    // eng: number of loaded atlases
    // rus: количество загруженных атласов
    int k_atlas;

    // eng: list of loaded atlases
    // rus: список загруженных атласов
    MP_Atlas **m_atlas;

    // eng: number of open files
    // rus: количество открытых файлов
    int k_ptc;

    // eng: list of open files
    // rus: список открытых файлов
    MP_PTC **m_ptc;

    // eng: settings to initialize emitters by default
    // rus: настройки для инициализации эмиттеров по умолчанию

    int interpolation;          // eng: method of interpolation applied to loaded emitters
    // rus: способ применения интерполяции к загружаемым эмиттерам

    int position_mode;          // eng: mode of changing emitter position
    // rus: режим изменения позиции эмиттера

    int atlas_width, atlas_height;
    int atlas_frame_step;
    float atlas_scale_step;

    bool copy_mode;             // eng: mode for working with files containing particle copies. false - file is deleted, when there are no emitters, that use it, true - file is deleted at program termination
    // rus: режим работы файлов с копиями частиц с копиями. false - файл удаляется, когда нет эмиттеров, которые его используют, true - файл удаляется при завершении работы программы

    HM_EMITTER next_descriptor;
    int next_index;

    bool is_new_atlas;

public:
    static MP_Device *device;
    ShaderProgram *m_program;
    TextureList *m_textures;
    QOpenGLFunctions *m_functions;
protected:
    MP_Manager();
    virtual ~MP_Manager();

public:
    static MP_Manager &GetInstance();

    // eng: Cleaning up
    // rus: Очистка
    virtual void Destroy();

    // eng: Initialization
    // rus: Инициализация
    void Initialization(bool *render_states_filters, bool render_states_optimization, MAGIC_AXIS_ENUM axis, MP_Platform *platform, int interpolation, int position_mode, int atlas_width = 1024, int atlas_height = 1024, int atlas_frame_step = 1, float atlas_starting_scale = 1.f, float atlas_scale_step = 0.1f, bool copy_mode = true);

    // eng: Returning the number of emitters
    // rus: Возвращение количества эмиттеров
    int GetEmitterCount()
    {
        return k_emitter;
    }

    // eng: Returning descriptor of first emitter.
    // rus: Получение дескриптора первого эмиттера
    HM_EMITTER GetFirstEmitter();

    // eng: Returning descriptor of next emitter.
    // rus: Получение дескриптора следующего эмиттера
    HM_EMITTER GetNextEmitter(HM_EMITTER hmEmitter);

    // eng: Returning the emitter by its descriptor
    // rus: Возвращение эмиттера по дескриптору
    MPEmitter *GetEmitter(HM_EMITTER hmEmitter);

    // eng: Returning the emitter by name
    // rus: Возвращание эмиттера по имени
    MPEmitter *GetEmitterByName(const char *name);

    // eng: Loading all emitters from emitters folder
    // rus: Загрузка всех эмиттеров из всех файлов
    void LoadAllEmitters();

    // eng: Loading all the emitters and animated folders from the file specified
    // rus: Загрузка всех эмиттеров из указанного файла. Загружаются эмиттеры и анимированные папки
    HM_FILE LoadEmittersFromFile(const char *file);
    HM_FILE LoadEmittersFromFileInMemory(const char *address);


    // eng: Deleting specified emitter
    // rus: Удаление указанного эмиттера
    int DeleteEmitter(HM_EMITTER hmEmitter);

    // eng: Closing file
    // rus: Выгрузка одного файла
    int CloseFile(HM_FILE hmFile);

    // eng: Closing all files
    // rus: Выгрузка всех файлов
    void CloseFiles();

    // eng: Duplicating specified emitter
    // rus: Дублирование указанного эмиттера
    HM_EMITTER DuplicateEmitter(HM_EMITTER hmEmitter);

    // eng: Updating emitters taking into account the passed time
    // rus: Обновление эмиттеров по таймеру
    int UpdateByTimer();

    // eng: Updating emitters
    // rus: Обновление эмиттеров
    void Update(double time);

    // eng: Rendering all emitters
    // rus: Рисование эмиттеров
    int Render();

    // eng: Stopping all the emitters
    // rus: Остановка эмиттеров
    void Stop();

    // eng: Refreshing textural atlases
    // rus: Построение текстурного атласа
    void RefreshAtlas();

    // eng: Restoring textural atlas in cases of loosing textures
    // rus: Восстановление текстурного атласа в случае потери текстур
    void RestoreAtlas();

    // eng: Returns the number of textural atlases
    // rus: Возвращает количество текстурных атласов
    int GetAtlasCount()
    {
        return k_atlas;
    }

    // eng: Returns textural atlas
    // rus: Возвращает текстурный атлас
    MP_Atlas *GetAtlas(int index)
    {
        return m_atlas[index];
    }

    // eng: Adding new emitter into array
    // rus: Добавление нового эмиттера в массив
    void AddEmitter(MPEmitter *emitter);

    // eng: Loading emitter
    // rus: Загрузка конкретного эмиттера
    MPEmitter *LoadEmitter(HM_FILE file, const char *path);

    // eng: Loading folder
    // rus: Загрузка папки
    void LoadFolder(HM_FILE file, const char *path);

    // eng: Getting mode of working with files containing particle copies
    // rus: Получение режима работы файлов копий частиц
    bool GetCopyMode()
    {
        return copy_mode;
    }

    // eng: Adding open file
    // rus: Добавление открытого файла
    void AddPTC(MP_PTC *ptc_file);

    // eng: Deleting open file
    // rus: Удаление открытого файла
    int DeletePTC(HM_FILE hmFile);

    // eng: Returns time in milliseconds
    // rus: Возвращает время в милисекундах
    unsigned long GetTick()
    {
        return platform->GetTick();
    }

    // eng: Returns path to folder with emitters
    // rus: Возвращает путь к папке с ptc-файлами
    const char *GetPathToPTC()
    {
        return platform->GetPathToPTC();
    }

    // eng: Returns path to folder with textures
    // rus: Возвращает путь к папке с текстурами
    const char *GetPathToTexture()
    {
        return platform->GetPathToTexture();
    }

    // eng: Returns path to folder which could be used by wrapper to store temporary files
    // rus: Возвращает путь к временной папке
    const char *GetPathToTemp()
    {
        return platform->GetPathToTemp();
    }

    // eng: Finds the first ptc-file in emitters folder
    // rus: Поиск первого ptc-файла в папке с эмиттерами
    const char *GetFirstFile()
    {
        return platform->GetFirstFile();
    }

    // eng: Finds the next ptc-file in emitters folder
    // rus: Поиск очередного ptc-файла в папке с эмиттерами
    const char *GetNextFile()
    {
        return platform->GetNextFile();
    }

    // eng: Converts wchar_t string into utf8
    // rus: Конвертирует строку типа wchar_t в строку типа utf8
    const char *wchar_to_utf8(const wchar_t *str)
    {
        return platform->wchar_to_utf8(str);
    }

    // eng: Converts utf8 string into wchar_t
    // rus: Конвертирует строку типа utf8 в строку типа wchar_t
    const wchar_t *utf8_to_wchar(const char *str)
    {
        return platform->utf8_to_wchar(str);
    }

    // eng: Opens ptc-file
    // rus: Открывает ptc-файл
    HM_FILE OpenPTC(const char *ptc_file)
    {
        return platform->OpenPTC(ptc_file);
    }

    // Deleting of file (analogue remove())
    // Удаление файла (аналог remove())
    int RemoveFile(const char *file)
    {
        return platform->RemoveFile(file);
    }

    // Loading of file
    // Чтение файла
    void *LoadFile(const char *file)
    {
        return platform->LoadFile(file);
    }

    // Saving of file
    // Сохранение файла
    bool SaveFile(const char *file)
    {
        return platform->SaveFile(file);
    }
};

// eng: Class storing the textural atlas. This class will be abstract
// rus: Класс, который хранит текстурный атлас. Этот класс будет абстрактным
class MP_Atlas
{
protected:
    GLuint texture;

public:
    int address_u, address_v;

public:
    static MP_Manager *MP;
    static MP_Device *device;

    GLuint GetTexture()
    {
        return texture;
    }


public:
    MP_Atlas(int width, int height, const char *file);
    ~MP_Atlas() {}

    // eng: Destroy atlas texture
    // rus: Уничтожить текстуру атласа
    void Destroy();

    // eng: Cleaning up of rectangle
    // rus: Очистка прямоугольника
    void CleanRectangle(const MAGIC_CHANGE_ATLAS *c);

    void LoadTexture(const MAGIC_CHANGE_ATLAS *c) {}
};

struct MP_PTC {
    HM_FILE hmFile;
    HM_STREAM hmStream;
    const char *data;

    MP_PTC();
    virtual ~MP_PTC();
    int Close();
};

// ---------------------------------------------------------------------------------------

struct MP_MATERIAL {
    MAGIC_MATERIAL material;
    int technique_index;

    MP_MATERIAL(MAGIC_MATERIAL *m, MP_Device *device);
};

// eng: Class controlling drawing. This class will be abstract
// rus: Класс, который управляет рисованием. Этот класс будет абстрактным
class MP_Device
{
protected:
    MAGIC_VERTEX_FORMAT vertex_format;

    int k_material;
    MP_MATERIAL **m_material;

public:
    static MP_Manager *MP;
    int window_width;
    int window_height;

public:
    MP_Device(int width, int height);
    virtual ~MP_Device();

    // eng: Returns window size
    // rus: Возвращает размер окна
    void GetWindowSize(int &wi, int &he)
    {
        wi = window_width;
        he = window_height;
    }

    // eng: Creating
    // rus: Создание
    virtual bool Create();

    // eng: Destroying
    // rus: Уничтожение
    virtual void Destroy();

    // eng: Creating of atlas object
    // rus: Создание объекта атласа
    virtual MP_Atlas *NewAtlas(int width, int height, const char *file);

    // eng: Creating of emitter object
    // rus: Создание объекта эмиттера
    virtual MPEmitter *NewEmitter(HM_EMITTER emitter, MP_Manager *manager);

    // eng: Creating of material
    // rus: Создание материала
    virtual MP_MATERIAL *NewMaterial(MAGIC_MATERIAL *mat);

    // eng: Refreshing materials
    // rus: Построение материалов
    void RefreshMaterials();
};

// ---------------------------------------------------------------------------------------

// eng: Saves TGA for testing
// rus: Сохраняет TGA для тестирования
extern void SaveTGA(int w, int h, unsigned char *pixel, const char *file);

#endif
