#ifndef __RECVWAITCLOSE_H__
#define __RECVWAITCLOSE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class RecvWaitClose : public olib::State<Connection, NetEvent>, public OSingleton<RecvWaitClose> {
	friend class OSingleton<RecvWaitClose>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	RecvWaitClose() {}
	virtual	~RecvWaitClose() {}
};

#endif //__RECVWAITCLOSE_H__
