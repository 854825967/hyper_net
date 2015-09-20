#ifndef __NETWORKER_H__
#define __NETWORKER_H__
#include "util.h"
#include "CycleQueue.h"
#include <thread>
#include "NetEvent.h"

class Connection;
class NetWorker {
	struct WorkRequest {
		enum class Event {
			SEND,
			RECV,
		};

		Connection * connection;
		Event evt;
	};

	struct WorkResult {
		Connection * connection;
		NetEvent evt;
	};

public:
	NetWorker(const s32 maxConnection);
	~NetWorker();

	bool Initialize();
	void Destroy();

	void Add(Connection* connection);
	void Remove(Connection* connection);

	void Recv(Connection* connection);
	void Send(Connection* connection);

	void ThreadProc();
	void DoRequest(const WorkRequest& request);

	void Process(const s64 frame);
	void DoResult(const WorkResult& result);

	inline s32 GetFreeSlot() { return _maxConnection - _nowConnection; }

private:
	s32 _maxConnection;
	s32 _nowConnection;

	olib::CycleQueue<WorkRequest> _workQueue;
	olib::CycleQueue<WorkResult> _resultQueue;

	std::thread _thread;
	bool _terminated;
};

#endif // __NETWORKER_H__

