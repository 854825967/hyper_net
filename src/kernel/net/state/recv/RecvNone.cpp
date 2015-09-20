#include "RecvNone.h"
#include "RecvClose.h"
#include "Recving.h"

void RecvNone::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_RECV: connection->BeginRecv(); connection->ChangeRecvState(Recving::Instance()); break;
	case NEVT_ERROR:
	case NEVT_CLOSE:
	case NEVT_SEND_ERROR:
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE: connection->ChangeRecvState(RecvClose::Instance()); break;
	}
}
