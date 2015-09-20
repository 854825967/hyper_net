#ifndef __SENDINGREADY_H__
#define __SENDINGREADY_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendingReady : public olib::State<Connection, NetEvent>, public OSingleton<SendingReady> {
	friend class OSingleton<SendingReady>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendingReady() {}
	virtual	~SendingReady() {}
};

#endif //__SENDINGREADY_H__
