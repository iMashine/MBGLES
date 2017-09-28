//-----------------------------------------------------------------------------
// File: mp.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include <math.h>
#include <errno.h>
#include <QFile>

#include "mpemitter.h"

// ------------------------------------------------------------------------------------------

// ru: Событие "Создание частицы"
// en: Event "Creating of particle"
void EventCreation(MAGIC_EVENT *evt)
{
    int a = 0;
    // ...
    // ...
    // ...
}

// ru: Событие "Уничтожение частицы"
// en: Event "Destruction of particle"
void EventDestruction(MAGIC_EVENT *evt)
{
    // ...
    // ...
    // ...
}

// ru: Событие "Существование частицы"
// en: Event "Existence of particle"
void EventExistence(MAGIC_EVENT *evt)
{
    // ...
    // ...
    // ...
}

// ru: Событие "Столкновение с препятствием"
// en: Event "Collision of particle with an obstacle"
void EventCollision(MAGIC_EVENT *evt)
{
    // ...
    // ...
    // ...
}

// ru: Событие "Примагничивание частицы"
// en: Event "Attraction of particle to the magnet"
void EventMagnetism(MAGIC_EVENT *evt)
{
    // ...
    // ...
    // ...
}

void (*function_event[5])(MAGIC_EVENT *evt) = {EventCreation, EventDestruction, EventExistence, EventCollision, EventMagnetism};

MP_Manager *MP_Platform::MP = NULL;

MP_Platform::MP_Platform()
{
}

MP_Platform::~MP_Platform()
{
}

// ru: Конвертация wchar_t в utf8
// en: Conversion of wchar_t in utf8
const char *MP_Platform::wchar_to_utf8(const wchar_t *str)
{
    const char *str8 = NULL;

    int size = sizeof(wchar_t);
    switch (size) {
    case 1:
        wchar_t8 = (const char *)str;
        str8 = wchar_t8.c_str();
        break;

    case 2:
        str8 = (const char *)Magic_UTF16to8((const unsigned short *)str);
        break;

    case 4:
        str8 = (const char *)Magic_UTF32to8((const unsigned int *)str);
        break;
    }
    return str8;
}

// ru: Конвертация utf8 в wchar_t
// en: Conversion of utf8 in wchar_t
const wchar_t *MP_Platform::utf8_to_wchar(const char *str)
{
    wchar_t *strw = NULL;

    int size = sizeof(wchar_t);
    switch (size) {
    case 1:
        wchar_t8 = str;
        strw = (wchar_t *)wchar_t8.c_str();
        break;

    case 2:
        strw = (wchar_t *)Magic_UTF8to16((const unsigned char *)str);
        break;

    case 4:
        strw = (wchar_t *)Magic_UTF8to32((const unsigned char *)str);
        break;
    }

    return strw;
}

// eng: Opens ptc-file
// rus: Открывает ptc-файл
HM_FILE MP_Platform::OpenPTC(const char *ptc_file)
{
    HM_FILE hmFile = 0;

    HM_STREAM hmStream = Magic_StreamOpenFile(ptc_file, MAGIC_STREAM_READ);
    hmFile = Magic_OpenStream(hmStream);
    if (hmFile > 0) {
        MP_PTC *ptc = new MP_PTC;
        ptc->hmFile = hmFile;
        ptc->hmStream = hmStream;
        ptc->data = NULL;
        MP->AddPTC(ptc);
    }
    else {
        Magic_StreamClose(hmStream);
    }

    return hmFile;
}

// Loading of file
// Чтение файла
void *MP_Platform::LoadFile(const char *file)
{
//    QFile *open_file = new QFile(file);

//    if (open_file->open(QIODevice::ReadWrite)) {
//        return open_file;
//    }

//    return Q_NULLPTR;
}

// Saving of file
// Сохранение файла
bool MP_Platform::SaveFile(const char *file)
{
//    FILE *f = fopen(file, "wb");

//    if (f) {
//        fwrite(file_data->data, 1, file_data->length, f);
//        fclose(f);

//        return  true;
//    }

    return false;
}

MP_Device *MP_Manager::device = NULL;

// eng: Class that is used as storage for Magic Particles emitters
// rus: Класс, который является хранилищем эмиттеров Magic Particles
MP_Manager::MP_Manager()
{
    MP_Device::MP = this;
    MP_Atlas::MP = this;
    MP_Platform::MP = this;

    platform = NULL;

    k_emitter = 0;
    max_emitter = 10;
    m_emitter = new MPEmitter*[max_emitter];
    m_descriptor = new int[max_emitter];
    for (int i = 0; i < max_emitter; i++) {
        m_emitter[i] = NULL;
        m_descriptor[i] = 0;
    }

    k_atlas = 0;
    m_atlas = NULL;

    k_ptc = 0;
    m_ptc = NULL;

    interpolation = MAGIC_INTERPOLATION_ENABLE;
    position_mode = MAGIC_CHANGE_EMITTER_DEFAULT;

    atlas_width = atlas_height = 1024;
    atlas_frame_step = 1;
    atlas_scale_step = 0.01f;

    next_descriptor = 0;
    next_index = -1;

    is_new_atlas = false;
}

MP_Manager::~MP_Manager()
{
    Destroy();
}

MP_Manager &MP_Manager::GetInstance()
{
    static MP_Manager mp;
    return mp;
}

// eng: Cleaning up
// rus: Очистка
void MP_Manager::Destroy()
{
    int i;
    for (i = 0; i < max_emitter; i++) {
        if (m_emitter[i]) {
            delete m_emitter[i];
            m_emitter[i] = NULL;
        }
    }

    if (m_emitter) {
        delete []m_emitter;
        m_emitter = NULL;
    }

    if (m_descriptor) {
        delete []m_descriptor;
        m_descriptor = NULL;
    }

    k_emitter = 0;
    max_emitter = 0;

    for (i = 0; i < k_atlas; i++) {
        m_atlas[i]->Destroy();
        delete m_atlas[i];
        m_atlas[i] = NULL;
    }

    if (m_atlas) {
        delete []m_atlas;
        m_atlas = NULL;
    }

    k_atlas = 0;

    for (i = 0; i < k_ptc; i++) {
        delete m_ptc[i];
        m_ptc[i] = NULL;
    }

    if (m_ptc) {
        delete []m_ptc;
        m_ptc = NULL;
    }

    k_ptc = 0;

    MP_Device::MP = NULL;
    MP_Atlas::MP = NULL;

    if (platform) {
        delete platform;
        platform = NULL;
    }

    MP_Platform::MP = NULL;
}

// eng: Initialization
// rus: Инициализация
void MP_Manager::Initialization(bool *render_states_filters, bool render_states_optimization, MAGIC_AXIS_ENUM axis, MP_Platform *platform, int interpolation, int position_mode, int atlas_width, int atlas_height, int atlas_frame_step, float atlas_starting_scale, float atlas_scale_step, bool copy_mode)
{
    this->platform = platform;

    Magic_SetRenderStateFilter(render_states_filters, render_states_optimization);

    Magic_SetAxis(axis);

    Magic_SetStartingScaleForAtlas(atlas_starting_scale);

    this->interpolation = interpolation;
    this->position_mode = position_mode;

    this->atlas_width = atlas_width;
    this->atlas_height = atlas_height;
    this->atlas_frame_step = atlas_frame_step;
    this->atlas_scale_step = atlas_scale_step;

    this->copy_mode = copy_mode;
}

// eng: Returning descriptor of first emitter.
// rus: Получение дескриптора первого эмиттера
HM_EMITTER MP_Manager::GetFirstEmitter()
{
    next_descriptor = 0;
    next_index = -1;

    if (k_emitter) {
        next_descriptor = m_descriptor[0];
        next_index = 0;
    }

    return next_descriptor;
}

/// eng: Returning descriptor of next emitter.
// rus: Получение дескриптора следующего эмиттера
HM_EMITTER MP_Manager::GetNextEmitter(HM_EMITTER hmEmitter)
{
    if (next_index == -1 || hmEmitter != next_descriptor) {
        next_index = -1;
        for (int i = 0; i < k_emitter; i++) {
            if (m_descriptor[i] == hmEmitter) {
                next_index = i;
                break;
            }
        }
    }

    next_descriptor = 0;

    if (next_index != -1) {
        next_index++;
        if (next_index < k_emitter) {
            next_descriptor = m_descriptor[next_index];
        }
        else
            next_index = -1;
    }

    return next_descriptor;
}

// eng: Returning the emitter by its descriptor
// rus: Возвращение эмиттера по дескриптору
MPEmitter *MP_Manager::GetEmitter(HM_EMITTER hmEmitter)
{
    if (hmEmitter >= 0 && hmEmitter < max_emitter)
        return m_emitter[hmEmitter];
    return NULL;
}

// eng: Returning the emitter by name
// rus: Возвращание эмиттера по имени
MPEmitter *MP_Manager::GetEmitterByName(const char *name)
{
    HM_EMITTER hmEmitter = GetFirstEmitter();
    while (hmEmitter) {
        MPEmitter *emitter = GetEmitter(hmEmitter);
        const char *emitter_name = emitter->GetEmitterName().toLatin1().data();
        if (!strcmp(name, emitter_name)) {
            // eng: name coincides
            // rus: имя совпадает
            return emitter;
        }
        hmEmitter = GetNextEmitter(hmEmitter);
    }

    return NULL;
}

// eng: Loading all emitters from emitters folder
// rus: Загрузка всех эмиттеров из всех файлов
void MP_Manager::LoadAllEmitters()
{
    const char *file = platform->GetFirstFile();
    while (file) {
        LoadEmittersFromFile(file);
        file = platform->GetNextFile();
    }
    RefreshAtlas();
}

// eng: Loading all the emitters and animated folders from the file specified
// rus: Загрузка всех эмиттеров из указанного файла. Загружаются эмиттеры и анимированные папки
HM_FILE MP_Manager::LoadEmittersFromFile(const char *file)
{
    std::string ptc_file = GetPathToPTC();
    ptc_file += file;

    HM_FILE mf = OpenPTC(ptc_file.c_str());
    if (mf > 0) {
        // eng: file was opened
        // rus: файл успешно открыт
        LoadFolder(mf, "");
        return mf;
    }

    return 0;
}

HM_FILE MP_Manager::LoadEmittersFromFileInMemory(const char *address)
{
    HM_STREAM hmStream = Magic_StreamOpenMemory(address, 0, MAGIC_STREAM_READ);

    HM_FILE hmFile = Magic_OpenStream(hmStream);
    if (hmFile > 0) {
        // eng: file was opened
        // rus: файл успешно открыт
        MP_PTC *ptc = new MP_PTC;
        ptc->hmFile = hmFile;
        ptc->hmStream = hmStream;
        ptc->data = address;
        AddPTC(ptc);

        LoadFolder(hmFile, "");
    }
    else {
        Magic_StreamClose(hmStream);
        delete []address;
        address = NULL;
    }
    return hmFile;
}

// eng: Closing file
// rus: Выгрузка одного файла
int MP_Manager::CloseFile(HM_FILE hmFile)
{
    RefreshAtlas();
    return DeletePTC(hmFile);
}

// eng: Closing all files
// rus: Выгрузка всех файлов
void MP_Manager::CloseFiles()
{
    RefreshAtlas();

    if (k_ptc) {
        int i = 0;
        do {
            delete m_ptc[i];
            m_ptc[i] = NULL;
            i++;
        }
        while (i < k_ptc);
        delete []m_ptc;
        m_ptc = NULL;
        k_ptc = 0;
    }
}

// eng: Duplicating specified emitter
// rus: Дублирование указанного эмиттера
HM_EMITTER MP_Manager::DuplicateEmitter(HM_EMITTER hmEmitter)
{
//    Emitter *from = GetEmitter(hmEmitter);
//    if (from) {
//        if (Magic_IsInterval1(hmEmitter) && (!from->copy)) {
//            // eng: it is necessary firstly to create particles copy
//            // rus: необходимо сначала создать копию частиц
//            from->Restart();
//        }

//        Emitter *emitter = device->NewEmitter(0, this);
//        *emitter = *from;
//        AddEmitter(emitter);
//        return emitter->GetEmitter();
//    }
    return 0;
}

// eng: Updating emitters taking into account the passed time
// rus: Обновление эмиттеров по таймеру
int MP_Manager::UpdateByTimer()
{
    static int fps = 0;
    static int fps_counter = 0;
    static unsigned long old_time = 0;

    static unsigned long last_time = 0;
    double rate = 0.01;
    unsigned long new_time = GetTick();
    if (new_time > last_time) {
        rate = new_time - last_time;
        last_time = new_time;
        if (rate > 500)
            rate = 0.01;
    }

    unsigned long rtime = new_time - old_time;
    if (rtime > 1000) {
        // eng: more than 1 second passed
        // rus: прошло больше секунды
        double percent = ((double)rtime) / 10.f;
        fps = (int)(percent * fps_counter / 100);
        fps_counter = 0;
        old_time = new_time;
    }

    fps_counter++;

    Update(rate);

    return fps;
}

// eng: Updating emitters
// rus: Обновление эмиттеров
void MP_Manager::Update(double time)
{
    HM_EMITTER hmEmitter = GetFirstEmitter();

    while (hmEmitter) {
        MPEmitter *emitter = GetEmitter(hmEmitter);
        int state = emitter->GetState();
        if (state == MAGIC_STATE_UPDATE || state == MAGIC_STATE_INTERRUPT) {
            emitter->Update(time);

            MAGIC_EVENT evt;
            while (Magic_GetNextEvent(&evt) == MAGIC_SUCCESS) {
                function_event[evt.event](&evt);    // обработка события
            }
        }

        hmEmitter = GetNextEmitter(hmEmitter);
    }
}

// eng: Rendering all emitters
// rus: Рисование эмиттеров
int MP_Manager::Render()
{
    RefreshAtlas();

    // eng: visualisation of all emitters
    // rus: визуализация всех эмиттеров
    int k_particles = 0;

    HM_EMITTER hmEmitter = GetFirstEmitter();

    while (hmEmitter) {
        MPEmitter *emitter = GetEmitter(hmEmitter);
//        QPointF pos = QPointF(device->window_width / 2,
//                              device->window_height / 2);
        emitter->SetScale(1);
//        emitter->SetPosition(pos);
        k_particles += emitter->Render();
        hmEmitter = GetNextEmitter(hmEmitter);
    }

    return k_particles;
}

// eng: Stopping all the emitters
// rus: Остановка эмиттеров
void MP_Manager::Stop()
{
    HM_EMITTER hmEmitter = GetFirstEmitter();

    while (hmEmitter) {
        MPEmitter *emitter = GetEmitter(hmEmitter);
        emitter->SetState(MAGIC_STATE_STOP);
        hmEmitter = GetNextEmitter(hmEmitter);
    }
}

// eng: Loading folder
// rus: Загрузка папки
void MP_Manager::LoadFolder(HM_FILE file, const char *path)
{
    Magic_SetCurrentFolder(file, path);

    MAGIC_FIND_DATA find;
    const char *name = Magic_FindFirst(file, &find, MAGIC_FOLDER | MAGIC_EMITTER);
    while (name) {
        if (find.animate) {
            LoadEmitter(file, name);
        }
        else
            LoadFolder(file, name);

        name = Magic_FindNext(file, &find);
    }

    Magic_SetCurrentFolder(file, "..");
}

// eng: Loading emitter
// rus: Загрузка конкретного эмиттера
MPEmitter *MP_Manager::LoadEmitter(HM_FILE file, const char *path)
{
    // eng: it is necessary to load emitter from file
    // rus: нужно извлечь эмиттер из файла
    MPEmitter *em = NULL;
    HM_EMITTER emitter = Magic_LoadEmitter(file, path);
    if (emitter) {
        em = device->NewEmitter(emitter, this);
        const char *ptc = Magic_GetFileName(file);
        if (ptc && Magic_HasTextures(file))
            em->restore_file = ptc;

        AddEmitter(em);
        // eng: initialization of emitter by default values
        // rus: инициализация эмиттера значениями по умолчанию
        if (interpolation != MAGIC_INTERPOLATION_DEFAULT) {
            bool _interpolation = false;
            if (interpolation == MAGIC_INTERPOLATION_ENABLE)
                _interpolation = true;
            Magic_SetInterpolationMode(emitter, _interpolation);
        }

        switch (position_mode) {
        case MAGIC_CHANGE_EMITTER_ONLY:
            Magic_SetEmitterPositionMode(emitter, false);
            Magic_SetEmitterDirectionMode(emitter, false);
            break;

        case MAGIC_CHANGE_EMITTER_AND_PARTICLES:
            Magic_SetEmitterPositionMode(emitter, true);
            Magic_SetEmitterDirectionMode(emitter, true);
            break;
        }

        if (Magic_GetStaticAtlasCount(file))
            em->is_atlas = true;
        else
            is_new_atlas = true;
    }
    return em;
}

// eng: Adding new emitter into array
// rus: Добавление нового эмиттера в массив
void MP_Manager::AddEmitter(MPEmitter *emitter)
{
    int i;

    next_descriptor = 0;
    next_index = -1;

    int index = (int)emitter->GetId();

    while (index >= max_emitter) {
        int new_max_emitter = max_emitter + 10;

        MPEmitter **vm_emitter = new MPEmitter*[new_max_emitter];
        for (i = 0; i < max_emitter; i++)
            vm_emitter[i] = m_emitter[i];
        delete []m_emitter;
        m_emitter = vm_emitter;

        int *vm_descriptor = new int[new_max_emitter];
        for (i = 0; i < max_emitter; i++)
            vm_descriptor[i] = m_descriptor[i];
        delete []m_descriptor;
        m_descriptor = vm_descriptor;

        for (i = max_emitter; i < new_max_emitter; i++) {
            m_emitter[i] = NULL;
            m_descriptor[i] = 0;
        }

        max_emitter = new_max_emitter;
    }

    m_emitter[index] = emitter;
    m_descriptor[k_emitter] = index;
    k_emitter++;
}

// ???
// eng: Refreshing textural atlases
// rus: Построение текстурного атласа
void MP_Manager::RefreshAtlas()
{
    int i;

    if (is_new_atlas) {
        // eng: new emitters were added, it is necessary to create new atlases for them
        // rus: были добавлены новые эмиттеры, необходимо создать для них атласы
        is_new_atlas = false;

        int k = GetEmitterCount();
        if (k) {
            HM_EMITTER *hm_emitter = new HM_EMITTER[k];

            k = 0;

            HM_EMITTER hmEmitter = GetFirstEmitter();
            while (hmEmitter) {
                MPEmitter *emitter = GetEmitter(hmEmitter);
                if (!emitter->is_atlas) {
                    emitter->is_atlas = true;
                    hm_emitter[k] = hmEmitter;
                    k++;
                }
                hmEmitter = GetNextEmitter(hmEmitter);
            }

            if (k)
                Magic_CreateAtlasesForEmitters(atlas_width, atlas_height, k, hm_emitter, atlas_frame_step, atlas_scale_step);

            delete []hm_emitter;
            hm_emitter = NULL;
        }
    }

    MAGIC_CHANGE_ATLAS c;
    while (Magic_GetNextAtlasChange(&c) == MAGIC_SUCCESS) {
        int type = c.type;
        switch (type) {
        case MAGIC_CHANGE_ATLAS_LOAD:
            // eng: loading of frame in atlas
            // rus: загрузка кадра в атлас
            m_atlas[c.index]->LoadTexture(&c);
            break;

        case MAGIC_CHANGE_ATLAS_CLEAN:
            // eng: cleaning up of rectangle in atlas
            // rus: очистка прямоугольника в атласе
            m_atlas[c.index]->CleanRectangle(&c);
            break;

        case MAGIC_CHANGE_ATLAS_CREATE:
            // eng: creating of atlas
            // rus: создание атласа
            if (m_atlas) {
                // eng: broadening of atlas array
                // rus: расширение массив атласов
                MP_Atlas **vm_atlas = new MP_Atlas*[k_atlas + 1];
                for (i = 0; i < k_atlas; i++)
                    vm_atlas[i] = m_atlas[i];
                delete []m_atlas;
                m_atlas = vm_atlas;
            }
            else {
                m_atlas = new MP_Atlas*[1];
            }

            m_atlas[k_atlas] = device->NewAtlas(c.width, c.height, c.file);
            k_atlas++;

            break;

        case MAGIC_CHANGE_ATLAS_DELETE:
            // eng: Deleting of atlas
            // rus: удаление атласа
            m_atlas[c.index]->Destroy();
            delete m_atlas[c.index];

            if (k_atlas == 1) {
                delete []m_atlas;
                m_atlas = NULL;
            }
            else {
                MP_Atlas **vm_atlas = new MP_Atlas*[k_atlas - 1];
                for (i = 0; i < c.index; i++)
                    vm_atlas[i] = m_atlas[i];
                for (i = c.index + 1; i < k_atlas; i++)
                    vm_atlas[i - 1] = m_atlas[i];
                delete []m_atlas;
                m_atlas = vm_atlas;
            }
            k_atlas--;
        }
    }

    device->RefreshMaterials();
}

// ???
// eng: Restoring textural atlas in cases of loosing textures
// rus: Восстановление текстурного атласа в случае потери текстур
void MP_Manager::RestoreAtlas()
{
//    if (k_emitter) {
//        int i;

//        // eng: Recreating of static atlas
//        // rus: Перестраиваем статические атласы
//        for (i = 0; i < k_atlas; i++) {
//            MP_Atlas *atlas = m_atlas[i];
//            std::string file = atlas->GetFile();
//            if (!(file.empty())) {
//                // eng: it is necessary to reload texture
//                // rus: необходимо перегрузить текстуру
//                int width, height;
//                atlas->GetSize(width, height);

//                atlas->Destroy();
//                delete atlas;
//                m_atlas[i] = device->NewAtlas(width, height, file.c_str());
//            }
//        }

//        // eng: Recreating of dynamic atlas
//        // rus: Перестраиваем динамические атласы
//        int k_restore_file = 0;
//        std::string **m_restore_file = new std::string*[k_emitter];

//        HM_EMITTER hmEmitter = GetFirstEmitter();
//        while (hmEmitter) {
//            Emitter *emitter = GetEmitter(hmEmitter);

//            bool add = true;

//            if (emitter->restore_file.empty())
//                add = false;
//            else {
//                for (int j = 0; j < k_restore_file; j++) {
//                    if (*(m_restore_file[j]) == emitter->restore_file) {
//                        add = false;
//                        break;
//                    }
//                }
//            }

//            if (add) {
//                m_restore_file[k_restore_file] = &(emitter->restore_file);
//                k_restore_file++;
//            }

//            hmEmitter = GetNextEmitter(hmEmitter);
//        }

//        HM_FILE *m_opened_file = NULL;
//        if (k_restore_file) {
//            // rus: loading all found files with textures
//            // rus: загружаем все отобранные файлы с текстурами
//            m_opened_file = new HM_FILE[k_restore_file];
//            for (i = 0; i < k_restore_file; i++) {
//                std::string path = GetPathToPTC();
//                path += *(m_restore_file[i]);
//                m_opened_file[i] = OpenPTC(path.c_str());
//            }
//        }

//        // eng: Recreating of dynamic atlas
//        // rus: Перестраиваем динамические атласы
//        Magic_CreateAtlases(atlas_width, atlas_height, atlas_frame_step, atlas_scale_step);

//        RefreshAtlas();

//        if (k_restore_file) {
//            // eng: Unload ptc-file with textures
//            // rus: выгружаем ptc-файлы с текстурами
//            for (i = 0; i < k_restore_file; i++)
//                CloseFile(m_opened_file[i]);

//            delete []m_opened_file;
//            m_opened_file = NULL;
//        }

//        delete []m_restore_file;
//        m_restore_file = NULL;
//        k_restore_file = 0;
//    }
}

// eng: Deleting specified emitter
// rus: Удаление указанного эмиттера
int MP_Manager::DeleteEmitter(HM_EMITTER hmEmitter)
{
    int result = MAGIC_ERROR;
    next_descriptor = 0;
    next_index = -1;

    for (int j = 0; j < k_emitter; j++) {
        HM_EMITTER hme = m_descriptor[j];
        if (hme == hmEmitter) {
            // it is necessary to delete this element from index array
            // нужно удалить данный элемент из индексного массива
            for (int k = j + 1; k < k_emitter; k++) {
                m_descriptor[k - 1] = m_descriptor[k];
            }

            k_emitter--;

            m_descriptor[k_emitter] = 0;

            delete m_emitter[hmEmitter];
            m_emitter[hmEmitter] = NULL;

            result = MAGIC_SUCCESS;

            break;
        }
    }

    return result;
}

// eng: Adding open file
// rus: Добавление открытого файла
void MP_Manager::AddPTC(MP_PTC *ptc_file)
{
    if (m_ptc) {
        MP_PTC **vm_ptc = new MP_PTC*[k_ptc + 1];
        for (int i = 0; i < k_ptc; i++)
            vm_ptc[i] = m_ptc[i];
        delete []m_ptc;
        m_ptc = vm_ptc;
    }
    else
        m_ptc = new MP_PTC*[1];

    m_ptc[k_ptc] = ptc_file;
    k_ptc++;
}

// eng: Deleting open file
// rus: Удаление открытого файла
int MP_Manager::DeletePTC(HM_FILE hmFile)
{
    int result = MAGIC_ERROR;
    for (int i = 0; i < k_ptc; i++) {
        if (m_ptc[i]->hmFile == hmFile) {
            result = m_ptc[i]->Close();
            delete m_ptc[i];

            if (k_ptc == 1) {
                delete []m_ptc;
                m_ptc = NULL;
            }
            else {
                MP_PTC **vm_ptc = new MP_PTC*[k_ptc - 1];
                int j;
                for (j = 0; j < i; j++)
                    vm_ptc[j] = m_ptc[j];
                for (j = i + 1; j < k_ptc; j++)
                    vm_ptc[j - 1] = m_ptc[j];
                delete []m_ptc;
                m_ptc = vm_ptc;
            }

            k_ptc--;

            break;
        }
    }
    return result;
}

// ------------------------------------------------------------------------------------------

MP_PTC::MP_PTC()
{
    hmFile = 0;
    hmStream = 0;
    data = NULL;
}

MP_PTC::~MP_PTC()
{
    Close();
}

int MP_PTC::Close()
{
    int result = MAGIC_ERROR;

    if (hmFile) {
        result = Magic_CloseFile(hmFile);
        hmFile = 0;
    }

    if (hmStream) {
        int result2 = Magic_StreamClose(hmStream);
        if (result == MAGIC_SUCCESS)
            result = result2;
        hmStream = 0;
    }

    if (data) {
        delete []data;
        data = NULL;
    }

    return result;
}

// ------------------------------------------------------------------------------------------

// eng: Class storing the textural atlas. This class will be abstract
// rus: Класс, который хранит текстурный атлас. Этот класс будет абстрактным

MP_Manager *MP_Atlas::MP = NULL;
MP_Device *MP_Atlas::device = NULL;

QImage LoadTextureFromFile(MP_Manager *MP, const char *file_name)
{
    std::string texture_file = MP->GetPathToTexture();
    texture_file += file_name;
    QImage image(QString(texture_file.data()));
    return image;
}

MP_Atlas::MP_Atlas(int width, int height, const char *file)
{
    address_u = address_v = -1;

    MP_Manager *mp = &MP_Manager::GetInstance();
    mp->m_textures->CreateTexture(QOpenGLTexture::Target2D);

    if (!file) {
        int size = width * height * 4;
        unsigned char *null_data = new unsigned char[size];

        for (int i = 0; i < size; i++)
            null_data[i] = 0;

        QImage image = QImage(null_data, width, height, QImage::Format_ARGB32);
        mp->m_textures->list.last()->setData(image);

        delete []null_data;
        null_data = NULL;
    }
    else {
        QImage image = LoadTextureFromFile(MP, file);
        mp->m_textures->list.last()->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        mp->m_textures->list.last()->setData(image);
    }

    texture = mp->m_textures->list.last()->textureId();
}

void MP_Atlas::Destroy()
{
    if (texture) {
        texture = 0;
    }
}

void MP_Atlas::CleanRectangle(const MAGIC_CHANGE_ATLAS *c)
{
    address_u = address_v = -1;

    MP_Manager *mp = &MP_Manager::GetInstance();
    mp->m_textures->CreateTexture(QOpenGLTexture::Target2D);

    int size = c->width * c->height * 4;
    unsigned char *null_data = new unsigned char[size];

    for (int i = 0; i < size; i++)
        null_data[i] = 0;

    QImage image = QImage(null_data, c->width, c->height, QImage::Format_ARGB32);
    mp->m_textures->list.last()->setData(image);

    delete []null_data;
    null_data = NULL;

    texture = mp->m_textures->list.last()->textureId();
}

// ------------------------------------------------------------------------------------------

// eng: Class controlling drawing. This class will be abstract
// rus: Класс, который управляет рисованием. Этот класс будет абстрактным

MP_Manager *MP_Device::MP = NULL;

MP_Device::MP_Device(int width, int height)
{
    MP_Manager::device = (MP_Device *)this;
    MP_Atlas::device = (MP_Device *)this;

    window_width = width;
    window_height = height;

    k_material = 0;
    m_material = NULL;
}

MP_Device::~MP_Device()
{
    MP_Manager::device = NULL;
    MP_Atlas::device = NULL;
}

// eng: Creating
// rus: Создание
bool MP_Device::Create()
{
    Destroy();
    return true;
}

// eng: Destroying
// rus: Уничтожение
void MP_Device::Destroy()
{
    int i;
    if (MP) {
        int k_atlas = MP->GetAtlasCount();
        for (i = 0; i < k_atlas; i++)
            MP->GetAtlas(i)->Destroy();
    }

    for (i = 0; i < k_material; i++) {
        delete m_material[i];
        m_material[i] = NULL;
    }

    if (m_material) {
        delete []m_material;
        m_material = NULL;
    }

    k_material = 0;
}

// eng: Creating of atlas object
// rus: Создание объекта атласа
MP_Atlas *MP_Device::NewAtlas(int width, int height, const char *file)
{
    return new MP_Atlas(width, height, file);
}

// eng: Creating of emitter object
// rus: Создание объекта эмиттера
MPEmitter *MP_Device::NewEmitter(HM_EMITTER emitter, MP_Manager *manager)
{
    return new MPEmitter(emitter, manager);
}

// eng: Creating of material
// rus: Создание материала
MP_MATERIAL *MP_Device::NewMaterial(MAGIC_MATERIAL *mat)
{
    return new MP_MATERIAL(mat, this);
}

// eng: Refreshing materials
// rus: Построение материалов
void MP_Device::RefreshMaterials()
{
    int k_material_api = Magic_GetMaterialCount();
    if (k_material < k_material_api) {
        // необходимо расширить массив материалов
        int i;
        if (m_material) {
            // eng: broadening of material array
            // rus: расширение массива материалов
            MP_MATERIAL **vm_material = new MP_MATERIAL*[k_material_api];
            for (i = 0; i < k_material; i++)
                vm_material[i] = m_material[i];
            delete []m_material;
            m_material = vm_material;
        }
        else {
            m_material = new MP_MATERIAL*[k_material_api];
        }

        MAGIC_MATERIAL mat;
        for (i = k_material; i < k_material_api; i++) {
            Magic_GetMaterial(i, &mat);
            m_material[i] = NewMaterial(&mat);
        }

        k_material = k_material_api;
    }
}

// --------------------------------------------------------------------------------------------------

bool MAGIC_VERTEX_FORMAT_IsEqual(MAGIC_VERTEX_FORMAT *f1, MAGIC_VERTEX_FORMAT *f2)
{
    return (f1->attributes == f2->attributes && f1->UVs == f2->UVs);
}

// --------------------------------------------------------------------------------------------------

MP_MATERIAL::MP_MATERIAL(MAGIC_MATERIAL *m, MP_Device *device)
{
    material = *m;
}

struct TargaFileHeader {
    char idLength;
    char colourMapType;
    char dataTypeCode;
    short colourMapOrigin;
    short colourMapLength;
    char colourMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

// eng: Saves TGA for testing
// rus: Сохраняет TGA для тестирования
void SaveTGA(int w, int h, unsigned char *pixel, const char *file)
{
    int components = 4;
    TargaFileHeader fileHeader;
    memset(&fileHeader, 0, sizeof(TargaFileHeader));
    fileHeader.dataTypeCode = 2;
    fileHeader.bitsPerPixel = components * 8;
    fileHeader.width = w;
    fileHeader.height = h;
    fileHeader.imageDescriptor = components == 4 ? 8 : 0;

    FILE *f = fopen(file, "wb");
    fwrite(&fileHeader.idLength, sizeof(fileHeader.idLength), 1, f);
    fwrite(&fileHeader.colourMapType, sizeof(fileHeader.colourMapType), 1, f);
    fwrite(&fileHeader.dataTypeCode, sizeof(fileHeader.dataTypeCode), 1, f);
    fwrite(&fileHeader.colourMapOrigin, sizeof(fileHeader.colourMapOrigin), 1, f);
    fwrite(&fileHeader.colourMapLength, sizeof(fileHeader.colourMapLength), 1, f);
    fwrite(&fileHeader.colourMapDepth, sizeof(fileHeader.colourMapDepth), 1, f);
    fwrite(&fileHeader.xOrigin, sizeof(fileHeader.xOrigin), 1, f);
    fwrite(&fileHeader.yOrigin, sizeof(fileHeader.yOrigin), 1, f);
    fwrite(&fileHeader.width, sizeof(fileHeader.width), 1, f);
    fwrite(&fileHeader.height, sizeof(fileHeader.height), 1, f);
    fwrite(&fileHeader.bitsPerPixel, sizeof(fileHeader.bitsPerPixel), 1, f);
    fwrite(&fileHeader.imageDescriptor, sizeof(fileHeader.imageDescriptor), 1, f);

    fwrite(pixel, 1, w * h * components, f);

    fclose(f);
}
