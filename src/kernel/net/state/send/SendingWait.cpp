#include "SendingWait.h"
#include "SendPending.h"
#include "Sending.h"
#include "SendClose.h"
#include "SendingReadyWait.h"
#include "SendingWaitClose.h"

void SendingWait::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_PENDING: connection->ChangeSendState(SendPending::Instance()); break;
	case NEVT_SEND_DONE: {
			if (connection->BeginSend())
				connection->ChangeSendState(Sending::Instance());
			else
				connection->ChangeSendState(SendClose::Instance());
		}
		break;
	case NEVT_SEND_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	case NEVT_SEND_READY: connection->ChangeSendState(SendingReadyWait::Instance()); break;
	case NEVT_SEND: break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE:
	case NEVT_CLOSE:
	case NEVT_ERROR: connection->ChangeSendState(SendingWaitClose::Instance()); break;
	}
}
