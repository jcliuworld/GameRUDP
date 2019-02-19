#include "TCPProtoBufClient.h"

TCPProtoBufClient::MSG_NAME_MAP TCPProtoBufClient::_msgnames;

DWORD TCPProtoBufClient::_id_svrbeat = name2id("Base.msgHeartbeartServer");
DWORD TCPProtoBufClient::_id_clibeat = name2id("Base.msgHeartbeartClient");

aRWLock TCPProtoBufClient::_msgLock;


