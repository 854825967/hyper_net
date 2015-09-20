#ifndef __RECVINGWAIT_H__
#define __RECVINGWAIT_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class RecvingWait : public olib::State<Connection, NetEvent>, public OSingleton<RecvingWait> {
	friend class OSingleton<RecvingWait>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	RecvingWait() {}
	virtual	~RecvingWait() {}
};

#endif //__RECVINGWAIT_H__
