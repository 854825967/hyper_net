#include "Redis.h"

Redis * Redis::s_self = nullptr;

bool Redis::Initialize(IKernel * kernel) {
    s_self = this;
    _kernel = kernel;

    return true;
}

bool Redis::Launched(IKernel * kernel) {
    return true;
}

bool Redis::Destroy(IKernel * kernel) {
    DEL this;
    return true;
}

