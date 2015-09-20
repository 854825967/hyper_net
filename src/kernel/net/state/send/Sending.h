#ifndef __SENDING_H__
#define __SENDING_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class Sending : public olib::State<Connection, NetEvent>, public OSingleton<Sending> {
	friend class OSingleton<Sending>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	Sending() {}
	virtual	~Sending() {}
};

#endif //__SENDING_H__
