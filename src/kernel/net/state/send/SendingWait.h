#ifndef __SENDINGWAIT_H__
#define __SENDINGWAIT_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendingWait : public olib::State<Connection, NetEvent>, public OSingleton<SendingWait> {
	friend class OSingleton<SendingWait>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendingWait() {}
	virtual	~SendingWait() {}
};

#endif //__SENDINGWAIT_H__
