#include "Starter.h"

Starter * Starter::s_self = nullptr;

bool Starter::Initialize(IKernel * kernel) {
    s_self = this;
    _kernel = kernel;

    return true;
}

bool Starter::Launched(IKernel * kernel) {
    return true;
}

bool Starter::Destroy(IKernel * kernel) {
    DEL this;
    return true;
}

