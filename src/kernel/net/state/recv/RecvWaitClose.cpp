#include "RecvWaitClose.h"
#include "RecvClose.h"

void RecvWaitClose::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_RECV_DONE:
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE: connection->ChangeRecvState(RecvClose::Instance()); break;
	}
}
