#ifndef __SENDPENDING_H__
#define __SENDPENDING_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendPending : public olib::State<Connection, NetEvent>, public OSingleton<SendPending> {
	friend class OSingleton<SendPending>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendPending() {}
	virtual	~SendPending() {}
};

#endif //__SENDPENDING_H__
