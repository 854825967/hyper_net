#ifndef __SENDNONE_H__
#define __SENDNONE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendNone : public olib::State<Connection, NetEvent>, public OSingleton<SendNone> {
	friend class OSingleton<SendNone>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendNone() {}
	virtual	~SendNone() {}
};

#endif //__SENDNONE_H__
