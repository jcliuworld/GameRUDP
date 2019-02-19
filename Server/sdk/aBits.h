#ifndef _aBits_H__
#define _aBits_H__
/**
* @file aBits.h
* @brief 比特位状态标记
* @author ljc jc.liuworld@hotmail.com
* @version 
* @date 2017-08-09
*/

#include "aDef.h"

template <DWORD MAX_BITS> 
class aBits
{
	public:		
		aBits()
		{
			bzero(_bits,sizeof(_bits));
		}
		
		/**
		 * @brief 设置状态
		 *
		 * @param bit 状态号
		 *	
		 * @return  
		 */
		bool set(DWORD bit)
		{
			if(bit >= MAX_BITS)
				return false;
			_bits[bit/32] |= 1 << (bit%32);

			return true;
		}

		/**
		 * @brief 是否设置了某个状态
		 *
		 * @param bit 状态号
		 *
		 * @return 
		 */
		bool is(DWORD bit)
		{
			if(bit >= MAX_BITS)
				return false;
			return  _bits[bit / 32] & (1 << (bit%32));
		}

		/**
		 * @brief 删除状态
		 *
		 * @param bit 状态号
		 *
		 * @return 
		 */
		bool clean(DWORD bit)
		{
			if(bit >= MAX_BITS)
				return false;
			_bits[bit/32] &= ~(1 << (bit%32));
			return true;
		}
		
	private:

		DWORD _bits[MAX_BITS/8+1];

};
#endif
