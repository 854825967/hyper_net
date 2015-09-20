#include "RecvingWait.h"
#include "RecvWaitClose.h"
#include "RecvClose.h"
#include "Recving.h"

void RecvingWait::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_RECV: break;
	case NEVT_RECV_DONE: {
			if (connection->ProcessPacket()) {
				connection->BeginRecv(); 
				connection->ChangeRecvState(Recving::Instance());
			}
			else {
				connection->ChangeRecvState(RecvClose::Instance());
			}
		}
		break;
	case NEVT_ERROR:
	case NEVT_CLOSE:
	case NEVT_SEND_ERROR: connection->ChangeRecvState(RecvWaitClose::Instance());  break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE: connection->ChangeRecvState(RecvClose::Instance()); break;
	}
}
