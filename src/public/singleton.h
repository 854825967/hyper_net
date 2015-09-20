#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED
#include "util.h"

template <typename T>
class OSingleton {
public:
    OSingleton() {}
    virtual ~OSingleton() {}

    inline static T * Instance() {
        static T * instance = nullptr;

        if (instance == nullptr) {
            instance = NEW T;
            if (!instance->Ready()) {
                delete instance;
                instance = nullptr;
            }
        }

        return instance;
    }
};

#endif // SINGLETON_H_INCLUDED
