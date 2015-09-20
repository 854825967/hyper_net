#include "SendWaitClose.h"
#include "SendClose.h"

void SendWaitClose::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_PENDING:
	case NEVT_SEND_DONE:
	case NEVT_SEND_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	}
}
