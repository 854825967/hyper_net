#include "Recving.h"
#include "RecvWaitClose.h"
#include "RecvClose.h"
#include "RecvingWait.h"
#include "RecvNone.h"

void Recving::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_RECV: connection->ChangeRecvState(RecvingWait::Instance()); break;
	case NEVT_RECV_DONE: {
			if (connection->ProcessPacket())
				connection->ChangeRecvState(RecvNone::Instance());
			else
				connection->ChangeRecvState(RecvClose::Instance());
		}
		break;
	case NEVT_ERROR:
	case NEVT_CLOSE:
	case NEVT_SEND_ERROR: connection->ChangeRecvState(RecvWaitClose::Instance());  break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE: connection->ChangeRecvState(RecvClose::Instance()); break;
	}
}
