#ifndef __OSTREAM_H_
#define __OSTREAM_H_
#include "util.h"

namespace olib {
    class OStream {
    public:
        OStream(const s32 size);
        ~OStream();

        inline char * buffer() { return _buffer; }
        inline s32 size() const {
            return ;
        }

    private:
        char * _buffer;
        s32 _size;
        s32 _offset;
    };
}

#endif //__OSTREAM_H_
