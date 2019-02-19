#include "TaskProtoBuf.h"

TaskProtoBuf::MSG_NAME_MAP TaskProtoBuf::_msgnames;

DWORD TaskProtoBuf::_id_svrbeat = name2id("Base.msgHeartbeartServer");
DWORD TaskProtoBuf::_id_clibeat = name2id("Base.msgHeartbeartClient");

aRWLock TaskProtoBuf::_msgLock;


