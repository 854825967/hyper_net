#ifndef __RECVNONE_H__
#define __RECVNONE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class RecvNone : public olib::State<Connection, NetEvent>, public OSingleton<RecvNone> {
	friend class OSingleton<RecvNone>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	RecvNone() {}
	virtual	~RecvNone() {}
};

#endif //__RECVNONE_H__
