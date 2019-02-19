#include "UDPTaskProtoBuf.h"

UDPTaskProtoBuf::MSG_NAME_MAP UDPTaskProtoBuf::_msgnames;

DWORD UDPTaskProtoBuf::_id_svrbeat = name2id("Base.msgHeartbeartServer");
DWORD UDPTaskProtoBuf::_id_clibeat = name2id("Base.msgHeartbeartClient");

aRWLock UDPTaskProtoBuf::_msgLock;


