#ifndef _MEMSTREA_H__
#define _MEMSTREA_H__
#pragma once
/**
* @file aMemStream.h
* @brief  内存流
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

#include "Age.h"
#include "sdkL.h"

#pragma pack(1)
/**
* @brief  内存块大小
*/

const unsigned int _BLOCKSIZE = 64*1024;

#define CHECK_CURR_PTR(size) if( (unsigned int) -1 - _currPtr <= (size)) \
	Age::logger->error(SYS_SDK_ID_000022,__FILE__ ,__LINE__)

/**
* @brief 内存流
*/

template <typename _type>
class aMemStream
{
	public:
		/**
		 * @brief 构造函数
		 */

		aMemStream(){
			_maxSize = _BLOCKSIZE;
			_offPtr = 0;
			_currPtr = 0;
		};
		/**
		 * @brief 析构函数
		 */

		~aMemStream(){};
		/**
		 * @brief 释放内存
		 *
		 * @param buf 位置
		 * @param size 大小
		 */

		inline void put(const unsigned char * buf, const unsigned int size)
		{
			wrReserve(size);
			bcopy(buf, &_buffer[_currPtr],size);
			CHECK_CURR_PTR(size);
			_currPtr += size;
		}
		/**
		 * @brief 获取可写内存位置
		 *
		 * @return 内存指针位置
		 */


		inline unsigned char  * wrBuf()
		{
			return &_buffer[_currPtr];
		}
		/**
		 * @brief 获取可读内存位置
		 *
		 * @return 内存指针位置
		 */


		inline unsigned char  * rdBuf()
		{
			return &_buffer[_offPtr];
		}
		/**
		 * @brief 准备读取
		 *
		 * @return 准好好 true 没准备好 false
		 */

		inline bool rdReady() const
		{
			if( _maxSize < _currPtr )
			{
				Age::logger->error(SYS_SDK_ID_000023 ,__FILE__, __LINE__);
			}
			return _currPtr > _offPtr;
		}

		/**
		 * @brief 获取可读内存大小
		 *
		 * @return 内存大小
		 */

		inline unsigned int rdSize() const
		{
			if( _maxSize < _currPtr )
			{
				Age::logger->error(SYS_SDK_ID_000024 ,__FILE__, __LINE__);
			}
			return _currPtr - _offPtr;
		}

		/**
		 * @brief 读取内存整理
		 *
		 * @param size
		 */

		inline void rdFlip( unsigned int size)
		{
			_offPtr +=size;
			if ( _currPtr > _offPtr )
			{
				unsigned int tmp = _currPtr - _offPtr;
				if( _offPtr >= tmp )
				{
					bcopy(&_buffer[_offPtr],&_buffer[0],tmp);
					_offPtr = 0;
					_currPtr = tmp;
				}
			}
			else
			{
				_offPtr = 0;
				_currPtr = 0;
			}
		}

		/**
		 * @brief 获取可写内存大小
		 *
		 * @return 大小
		 */

		inline unsigned int wrSize()
		{
			return _maxSize - _currPtr;
		}
		/**
		 * @brief 写入内存整理
		 *
		 * @param size
		 */


		inline void wrFlip(const unsigned int size)
		{
			CHECK_CURR_PTR(size);
			_currPtr += size;
		}
		/**
		 * @brief 重置内存标记
		 */


		inline void reset()
		{
			_offPtr = 0;
			_currPtr = 0;
		}

		/**
		 * @brief 最大值
		 *
		 * @return
		 */

		inline unsigned int maxSize()const
		{
			return _maxSize;
		}
		/**
		 * @brief 获取当前指针偏移位置
		 *
		 * @return 指针偏移位置
		 */


		inline unsigned int offPtr()const
		{
			return _offPtr;
		}
		/**
		 * @brief 获取当前指针位置
		 *
		 * @return 当前指针位置
		 */


		inline unsigned int currPtr() const
		{
			return _currPtr;
		}
		/**
		 * @brief 整理内存
		 *
		 * @param size
		 */

		inline void wrReserve(const unsigned int size);

	private:
		/**
		 * @brief 最大内存值
		 */
		unsigned int _maxSize;
		/**
		 * @brief 偏移位置
		 */

		unsigned int _offPtr;
		/**
		 * @brief 当前位置
		 */

		unsigned int _currPtr;
		/**
		 * @brief 内存池
		 */

		_type _buffer;

};


#define unzip_size(zip_size) ((zip_size)*120/100 * 12)

const unsigned int PACKET_ZIP_BUFFER = unzip_size(_BLOCKSIZE-1) + sizeof(unsigned int) + 8;

typedef  aMemStream<std::vector<unsigned char> > stBufferCmdQueue;

template <>
inline void aMemStream<std::vector<unsigned char> >::wrReserve(unsigned int size)
{
	if(_maxSize < _currPtr)
	{
		Age::logger->error(SYS_SDK_ID_000025 ,__FILE__, __LINE__);
	}

	if(wrSize()<size + 8)
	{
		unsigned int count = ( size + _BLOCKSIZE + 7 )/_BLOCKSIZE;
		unsigned int midd = count * _BLOCKSIZE;

		_maxSize += midd;

		_buffer.resize(_maxSize);

	}
}
typedef  aMemStream<unsigned char [PACKET_ZIP_BUFFER]> stStackCmdQueue;

template <>
inline void aMemStream<unsigned char [PACKET_ZIP_BUFFER]>::wrReserve(const unsigned int size)
{

}
#pragma pack()

#endif
