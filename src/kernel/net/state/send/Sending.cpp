#include "Sending.h"
#include "SendPending.h"
#include "SendingReady.h"
#include "SendingWait.h"
#include "SendWaitClose.h"
#include "SendClose.h"
#include "SendNone.h"

void Sending::OnEvent(Connection* connection, const NetEvent& evt) {
	switch (evt.type) {
	case NEVT_SEND_PENDING: connection->ChangeSendState(SendPending::Instance()); break;
	case NEVT_SEND_DONE: connection->ChangeSendState(SendNone::Instance()); break;
	case NEVT_SEND_ERROR: connection->ChangeSendState(SendClose::Instance()); break;
	case NEVT_SEND_READY: connection->ChangeSendState(SendingReady::Instance()); break;
	case NEVT_SEND: connection->ChangeSendState(SendingWait::Instance()); break;
	case NEVT_RECV_ERROR:
	case NEVT_RECV_CLOSE:
	case NEVT_CLOSE:
	case NEVT_ERROR: connection->ChangeSendState(SendWaitClose::Instance()); break;
	}
}
