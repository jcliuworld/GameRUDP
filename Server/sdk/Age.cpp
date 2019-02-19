#include "Age.h"
/** 
* @file Age.cpp
* @brief  公共包含文件 用于包含公共头文件信息 及 全局的变量和函数
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

namespace Age{
	/** 
	 * @brief 日志系统指针
	 */

	aLogger * logger = NULL;
	/** 
	 * @brief 全局变量存储对象
	 */

	aGlobal global;
	/** 
	 * @brief 随机种子
	 */

	__thread unsigned int seedp = 0;

};
