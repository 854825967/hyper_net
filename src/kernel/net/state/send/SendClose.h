#ifndef __SENDCLOSE_H__
#define __SENDCLOSE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendClose : public olib::State<Connection, NetEvent>, public OSingleton<SendClose> {
	friend class OSingleton<SendClose>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendClose() {}
	virtual	~SendClose() {}
};

#endif //__SENDCLOSE_H__
