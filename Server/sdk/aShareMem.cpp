 #include <errno.h>
#include "aShareMem.h"


aShareMem::aShareMem()
{
	_shmID		= 0;
	_shmAddr	= NULL;
}
aShareMem::~aShareMem()
{
}

bool aShareMem::init( int key , int shmMode );
{	
	switch(shmMode)
	{
		case SHM_INIT_POOL:
			{
				initPool( key );		
			}break;
		case SHM_RESUME_POOL:
			{
				resumeSHM( key );		
			}break;
		default:
			{
				return false;	
			}
			break; 
	}	
	return true;			
}
int aShareMem::initPool( int key )
{
	int ret;
	unsigned int size = 0;

	size = calculateSHMSize(); 

	_shmID = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);

	if( _shmID<0 && (EEXIST == errno))
        {
                _shmID = shmget(key, size, 0666);
                if(_shmID < 0)
                {
                        if(EINVAL == errno)
                        {
				_shmID = shmget(key, 0, 0666);
                                if( _shmID < 0)
                                {
                                    Age::logger->error("shm get error,  error no. %d", errno);
                                    return AGEERR_UNKNOW;
                                }	
				shmctl(_shmID, IPC_RMID, 0);

				_shmID = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);
			}
			else
			{
				 return AGEERR_SHM_BADALLOC;
			}
		}
	}
	
	if( _shmID<0 )
	{
		Age::logger->error("SHM 申请失败 %s:%d",__FILE__,__LINE__);
		return AGEERR_SHM_NOT_EXISTS;	
	}	


	void* _shmAddr = shmat(_shmID, NULL, 0);
	if (_shmAddr ==  (void *)-1)
	{
		Age::logger->error("shm init mat fail, error no. %d \n", errno);
		return AGEERR_UNKNOW;

	}	

	Age::logger->debug("SHM  id : %d init size :%d",_shmID,iSize);

	initSHMContiner();

	return 0;
}
unsigned int aShareMem::calculateSHMSize()
{
	int pagesize = getpagesize();
		
	
	return  ((MAX_BLOCKSIZE*(SHM_MINI_SIZE 
				+ SHM_MEDIUM1_SIZE 
				+ SHM_MEDIUM2_SIZE
				+ SHM_MAX_SIZE)/pagesize) + 1 )*pagesize;

}

int aShareMem::getSHMID()
{
	return _shmID;
}
char *  aShareMem::initSHMOneType(char * addr,Type_MemSize type,bool isresume)
{
	LPSHAREMEMHEAD lpHeadData = addr;

	for(int index = 0;index<MAX_BLOCKSIZE - 1;index++)
	{
	    if(type == Mem_MinSize)
		addr += SHM_MINI_SIZE ;//转移到下一block的起始位置
	    else if(type ==Mem_MediumSize1)
		addr += SHM_MEDIUM1_SIZE;
	    else if(type == Mem_MediumSize2)
		addr += SHM_MEDIUM2_SIZE;
	    else if(type == Mem_MaxSize)
		addr += SHM_MAX_SIZ;

		if(isresume && lpHeadData->_isUsed)
		{
			resumeDate( lpHeadData );
			_used_map.insert( std::make_pair(  lpHeadData->_data_addr  , lpHeadData) );
		}
		else
		{
			bzero_Head(lpHeadData);
			_free_vec.push_back(lpHeadData);			
		}

	}
	return addr;

}
void aShareMem::bzero_Head(LPSHAREMEMHEAD headData)
{
	if(NULL == headData)
		return;
	switch (headData->_type)
	{
		case SHM_MinSize:
			{
				bzero(headData,SHM_MINI_SIZE*sizeof(char));
				headData->_type = SHM_MinSize;
			}
			break;
		case SHM_MediumSize1:
			{
				bzero(headData,SHM_MEDIUM1_SIZE*sizeof(char));
				headData->_type = SHM_MediumSize1;
			}
			break;
		case SHM_MediumSize2:
			{
				bzero(headData,SHM_MEDIUM2_SIZE*sizeof(char));
				headData->_type = SHM_MediumSize2
			}
			break;
		case SHM_MaxSize:
			{
				bzero(headData,SHM_MAX_SIZE*sizeof(char));
				headData->_type =  SHM_MaxSize;
			}
			break;
		default:
			break;
	}
}
void aShareMem::initSHMContiner( bool isResume )
{
	if( _shmAddr == NULL )
		return;
		
	
	const char * lpData = static_cast<const char *>_shmAddr;	
	
	lpData = initSHMOneType(lpData,SHM_MinSize,isResume);
	lpData = initSHMOneType(lpData,SHM_MediumSize1,isResume);
	lpData = initSHMOneType(lpData,SHM_MediumSize2,isResume);
	lpData = initSHMOneType(lpData,SHM_MaxSize,isResume);
					
}

int aShareMem::resumeSHM( int key )
{
	unsigned int size = calculateSHMSize(); 
	
	_shmID = shmget(key, size, 0666);

	if( _shmID < 0 )
	{
		return AGEERR_SHM_NOT_EXISTS;	
	}

	_shmAddr = shmat(_shmID, NULL, 0);	

	if(_shmAddr ==  (void *)-1 )
	{
		return AGEERR_UNKNOW;
	}

	initSHMList(true);	
	
	return 0;
}

void aShareMem::cleanUp()
{
	_free_vec.clear();
	_used_map.clear();
	shmdt( _shmAddr );		
}

void * aShareMem::allocate(Type_MemSize size,unsigned short dataid)
{
	for( SHAREMEMHEAD_VEC_ITER it = _free_vec.begin() ; it != _free_vec.end(); ++ it)
	{
		if((*it)->_type == size )
		{
		
			break;
		}
	}
}

void aShareMem::reg_datafunc(unsigned short data_id,LPRESUMECALLBACK call)
{
	
}              

