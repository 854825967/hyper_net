#ifndef __SENDINGREADYWAIT_H__
#define __SENDINGREADYWAIT_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class SendingReadyWait : public olib::State<Connection, NetEvent>, public OSingleton<SendingReadyWait> {
	friend class OSingleton<SendingReadyWait>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	SendingReadyWait() {}
	virtual	~SendingReadyWait() {}
};

#endif //__SENDINGREADYWAIT_H__
