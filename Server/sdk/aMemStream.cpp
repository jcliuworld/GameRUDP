#include "aMemStream.h"
 template <>
stBufferCmdQueue::aMemStream():_maxSize(_BLOCKSIZE),_offPtr(0),_currPtr(0),_buffer(_maxSize)
{


}

template <>
stStackCmdQueue::aMemStream():_maxSize(PACKET_ZIP_BUFFER),_offPtr(0),_currPtr(0)
{


}

