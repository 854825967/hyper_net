#ifndef __RECVCLOSE_H__
#define __RECVCLOSE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class RecvClose : public olib::State<Connection, NetEvent>, public OSingleton<RecvClose> {
	friend class OSingleton<RecvClose>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	RecvClose() {}
	virtual	~RecvClose() {}
};

#endif //__RECVCLOSE_H__
