#ifndef __RECVING_H__
#define __RECVING_H__
#include "util.h"
#include "Fsm.h"
#include "Connection.h"
#include "singleton.h"
#include "NetEvent.h"

class Recving : public olib::State<Connection, NetEvent>, public OSingleton<Recving> {
	friend class OSingleton<Recving>;
public:
	bool Ready() { return true; }

	virtual void OnEnter(Connection* entity) {}
	virtual void OnLeave(Connection* entity) {}
	virtual void OnEvent(Connection* entity, const NetEvent& evt);

private:
	Recving() {}
	virtual	~Recving() {}
};

#endif //__RECVING_H__
