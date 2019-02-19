#ifndef _SHAREMEM_H__
#define _SHAREMEM_H__
/** 
 *  @file tMemCache.h
 *  @brief 简单共享内存管理 不支持动态扩展(有时间还可以针对不同的区块大小区分数量)
 *  @author ljc jc.liuworld@hotmail.com
 *  @date 2012-08-13
 * 
 */
//这个地方应该根据项目中实际内存使用情况对大小进行处理，这里只做简单的实例性质的分配

#include <map>

#define SHM_KEY 8008

#define MAX_BLOCK_SIZE 2048*12 //每种内存分配数量 从小到大 依次分布
/** 
 *
 * @brief 每种内存的大小 
 *
 */

#define SHM_TYPE_SIZE 4

#define SHM_MINI_SIZE 256
#define SHM_MEDIUM1_SIZE  SHM_MINI_SIZE*4
#define SHM_MEDIUM2_SIZE  SHM_MINI_SIZE*16
#define SHM_MAX_SIZE SHM_MINI_SIZE*256
/** 
 *
 * @brief  申请的内存类型 
 *
 */
enum Type_ShareMem
{
	SHM_MinSize = 1,// SHM_MINI_SIZE
	SHM_MediumSize1 = 2,// SHM_MEDIUM1_SIZE
	SHM_MediumSize2  = 3,// SHM_MEDIUM2_SIZE
	SHM_MaxSize = 4,// SHM_MAX_SIZE
};

struct tagMemBlock;
typedef struct tagMemBlock MEMBLOCK;
typedef struct tagMemBlock *LPMEMBLOCK;

struct tagShareMemHead;
typedef struct tagShareMemHead SHAREMEMHEAD;
typedef struct tagShareMemHead *LPSHAREMEMHEAD;

struct tagResumeCallBack;
typedef struct tagResumeCallBack RESUMECALLBACK;
typedef struct tagResumeCallBack *LPRESUMECALLBACK;

struct tagShareMemHead
{
	unsigned char _type; 	/** <内存类型 > */
	unsigned short _version; /** <版本号 > */
	unsigned char _isUsed;  /** <是否在使用  resume时使用 > */
	unsigned short _datatype_id; /** <存储的数据类型 > */
	void * _data_addr;	/** <数据地址> */
};

/**
 * 
 * @ brief 数据加载回调函数 
 *
 */
typedef void ( * DATA_CALL_FUNC)( void * addr); 

/**
 * 
 * @ brief 回调注册结构 
 *
 */

struct tagResumeCallBack
{
	unsigned short _data_id;
	unsigned char _size;
	DATA_CALL_FUNC func;

};

class aShareMem
{
	public:
		enum {
			SHM_INIT_POOL,
			SHM_RESUME_POOL,
		};

		aShareMem();
		~aShareMem();

		bool init( int key = SHM_KEY )		

		int initPool( int key );

		int resumeSHM( int key );

		int getSHMID();
		
		unsigned int calculateSHMSize();

		void initSHMContiner( bool isResume = false );

		void cleanUp();

		inline void * allocate(Type_MemSize size,unsigned short dataid);

		inline void  free(void * object);

		inline void bzero_Head(LPSHAREMEMHEAD headData)

		inline void detached(void * mem);

		int isSHMExit(int shmid);

		inline void  * getFreeBlock(LPMEMBLOCK  block, unsigned short dataid);	
		
	
		void regDataFunc(unsigned short data_id,LPRESUMECALLBACK call);

		LPRESUMECALLBACK getDataFunc(unsigned short data_id);

		void removeDataFunc(unsigned short data_id);

		void resumeDate(LPSTOREHEAD data);

	private:
		/**
		*
		* @ free共享内存定义
		*
		*/	
		typedef std::vectro<LPSHAREMEMHEAD> SHAREMEMHEAD_VEC;  
		typedef SHAREMEMHEAD_VEC::iterator SHAREMEMHEAD_VEC_ITER;
		typedef SHAREMEMHEAD_VEC::iterator_const SHAREMEMHEAD_VEC_CONST_ITER

		SHAREMEMHEAD_VEC _free_vec; /** < 空闲的共享内存> */

		/**
		 *
		 * @ used共享内存定义
		 *
		 */	
		typedef std::map<void *,LPSHAREMEMHEAD> SHAREMEMHEAD_MAP;
		typedef SHAREMEMHEAD_MAP SHAREMEMHEAD_MAP_ITER;
		typedef SHAREMEMHEAD_MAP SHAREMEMHEAD_MAP_CONST_ITER;
		SHAREMEMHEAD_MAP _used_map; /** < 正在使用的共享内存 key为数据的使用地址即tagStoredHead 中  _data_addr的地址 方便回收> */
		
		void * _shmAddr; /** <共享内存的地址> */ 
			
		int _shmID;		

		typedef std::map<unsigned short,LPRESUMECALLBACK > DATA_CALLBACK_MAP;

		typedef  DATA_CALLBACK_MAP::iterator DATA_CALLBACK_ITER;
		typedef  DATA_CALLBACK_MAP::const_iterator DATA_CALLBACK_CONST_ITER;

		static DATA_CALLBACK_MAP _datacall_map; /** < 回调处理> */ 
		
};

#endif
