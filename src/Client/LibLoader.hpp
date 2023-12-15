/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** LibLoader.hpp
*/

#ifndef LIBLOADER_HPP
    #define LIBLOADER_HPP

    #ifdef _WIN32
        #include <windows.h>
    #else
        #include <dlfcn.h>
    #endif
    #include <iostream>

class LibLoader {
    public:
        LibLoader(const char* libName) {
            #ifdef _WIN32
                _lib = LoadLibrary(libName);
            #else
                _lib = dlopen(libName, RTLD_LAZY);
            #endif
            if (!_lib) {
                std::cerr << "Failed to load library: " << libName << std::endl;
            }
        }

        virtual ~LibLoader() {
            if (_lib) {
                #ifdef _WIN32
                    FreeLibrary((HMODULE)_lib);
                #else
                    dlclose(_lib);
                #endif
            }
        }

        void* getFunction(const char* funcName) {
            #ifdef _WIN32
                return GetProcAddress((HMODULE)_lib, funcName);
            #else
                return dlsym(_lib, funcName);
            #endif
        }
    private:
        void* _lib = nullptr;
};

#endif // LIBLOADER_HPP
