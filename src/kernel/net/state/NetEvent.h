#ifndef __NETEVENT_H__
#define __NETEVENT_H__
#include "util.h"

enum NetEventType {
	NEVT_SEND,
	NEVT_SEND_DONE,
	NEVT_SEND_ERROR,
	NEVT_SEND_READY,
	NEVT_SEND_PENDING,

	NEVT_RECV,
	NEVT_RECV_DONE,
	NEVT_RECV_ERROR,
	NEVT_RECV_CLOSE,

	NEVT_CLOSE,
	NEVT_ERROR,
};

struct NetEvent {
	s32 type;
};

#endif //__NETEVENT_H__
