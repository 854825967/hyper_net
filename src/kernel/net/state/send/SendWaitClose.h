#ifndef __SENDWAITCLOSE_H__
#define __SENDWAITCLOSE_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendWaitClose : public olib::State<Connection, NetEvent>, public OSingleton<SendWaitClose> {
	friend class OSingleton<SendWaitClose>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendWaitClose() {}
	virtual	~SendWaitClose() {}
};

#endif //__SENDWAITCLOSE_H__
