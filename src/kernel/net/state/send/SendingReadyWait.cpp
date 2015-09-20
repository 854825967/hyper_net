#include "SendingReadyWait.h"
#include "Sending.h"
#include "SendClose.h"
#include "SendingWaitClose.h"

void SendingReadyWait::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_PENDING:
	case NEVT_SEND_DONE: {
			if (connection->BeginSend())
				connection->ChangeSendState(Sending::Instance());
			else
				connection->ChangeSendState(SendClose::Instance());
		}
		break;
	case NEVT_SEND_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	case NEVT_SEND_READY:
	case NEVT_SEND: break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE:
	case NEVT_CLOSE:
	case NEVT_ERROR: connection->ChangeSendState(SendingWaitClose::Instance()); break;
	}
}
