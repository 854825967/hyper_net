#ifndef __SENDINGWAITCLOSE_H__
#define __SENDINGWAITCLOSE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendingWaitClose : public olib::State<Connection, NetEvent>, public OSingleton<SendingWaitClose> {
	friend class OSingleton<SendingWaitClose>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendingWaitClose() {}
	virtual	~SendingWaitClose() {}
};

#endif //__SENDINGWAITCLOSE_H__
