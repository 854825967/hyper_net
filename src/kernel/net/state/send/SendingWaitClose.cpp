#include "SendingWaitClose.h"
#include "SendClose.h"
#include "SendWaitClose.h"

void SendingWaitClose::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_PENDING:
	case NEVT_SEND_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	case NEVT_SEND_DONE: {
			if (connection->BeginSend())
				connection->ChangeSendState(SendWaitClose::Instance());
			else
				connection->ChangeSendState(SendClose::Instance());
		}
		break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE: connection->ChangeSendState(SendWaitClose::Instance()); break;
	}
}
