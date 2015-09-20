#include "SendPending.h"
#include "SendClose.h"
#include "Sending.h"

void SendPending::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_READY: {
			if (connection->BeginSend())
				connection->ChangeSendState(Sending::Instance());
			else
				connection->ChangeSendState(SendClose::Instance());
		}
		break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE:
	case NEVT_CLOSE:
	case NEVT_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	}
}
