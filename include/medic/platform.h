#ifndef MEDIC_PLATFORM_INCLUDED
#define MEDIC_PLATFORM_INCLUDED

    #ifdef _WIN32
        #define MEDIC_EXPORT __declspec(dllexport)
    #else
        #define MEDIC_EXPORT __attribute__((visibility ("default")))
    #endif


    #ifdef _WIN32
        #include <windows.h>

        #define RTLD_LAZY  0x01
        #define RTLD_LOCAL 0x02

        static void *dlopen(const char *filename, int /*flag*/)
        {
           return LoadLibraryEx(filename, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        }

        static void* dlsym(void *handle, const char *symbol)
        {
           if (handle != 0)
           {
              return GetProcAddress((HMODULE)handle, symbol);
           }
           else
           {
              return 0;
           }
        }

        static int dlclose(void *handle)
        {
           return (FreeLibrary((HMODULE)handle) == TRUE ? 0 : 1);
        }

        static char* dlerror()
        {
           static char buffer[512];

           FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                         NULL,
                         GetLastError(),
                         MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                         (LPSTR)&buffer,
                         sizeof(buffer),
                         NULL);

           return buffer;
        }

    #else
        #include <dlfcn.h>

    #endif

#endif