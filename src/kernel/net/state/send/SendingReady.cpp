#include "SendingReady.h"
#include "SendNone.h"
#include "SendingReadyWait.h"
#include "SendClose.h"
#include "SendingWaitClose.h"
#include "Sending.h"

void SendingReady::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_PENDING: {
			if (connection->BeginSend())
				connection->ChangeSendState(Sending::Instance());
			else
				connection->ChangeSendState(SendClose::Instance());
		}
		break;
	case NEVT_SEND_DONE: connection->ChangeSendState(SendNone::Instance()); break;
	case NEVT_SEND_READY: break;
	case NEVT_SEND: connection->ChangeSendState(SendingReadyWait::Instance()); break;
	case NEVT_SEND_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE:
	case NEVT_CLOSE:
	case NEVT_ERROR: connection->ChangeSendState(SendingWaitClose::Instance()); break;
	}
}
