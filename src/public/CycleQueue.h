#ifndef __CYCLEQUEUE_H_
#define __CYCLEQUEUE_H_
#include "util.h"

namespace olib {
	template <typename T>
    class CycleQueue {
    public:
		CycleQueue(const s32 size) {
			_front = 0;
			_tail = 0;
			_size = size + 1;
			OASSERT(_size > 0, "size is invalid");

			_queue = (T*)MALLOC(_size * sizeof(T));
		}

		~CycleQueue() {
			FREE(_queue);
		}

		inline bool Read(T& t) {
			if (IsEmpty())
				return false;

			t = _queue[_front];
			_front = (_front + 1) % _size;
			return true;
		}

		inline void Push(const T& t) {
			if (IsFull()) {
				OASSERT(false, "queue is full");
				return;
			}

			_queue[_tail] = t;
			_tail = (_tail + 1) % _size;
		}

		inline bool IsEmpty() {
			return _front == _tail;
		}

		inline bool IsFull() {
			return _front == (_tail + 1) % _size;
		}

    private:
		T * _queue;
		volatile s32 _front;
		volatile s32 _tail;
		s32 _size;
    };
}

#endif //__CYCLEQUEUE_H_
