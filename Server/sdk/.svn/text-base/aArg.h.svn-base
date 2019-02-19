#ifndef _aArg_H__
#define _aArg_H__
/** 
* @file aArg.h
* @brief 命令行解析类 
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/
 #include <string.h>
#include <argp.h>
#include "aFunc.h"
#include "aNoneCopyable.h"
#include "aDef.h"

typedef error_t(* argsParser)(int key,char * arg,struct argp_state *state);

/** 
* @brief 命令行解析 单件模式
*/
class aArg:private aNoneCopyable
{
	/** 
	 * @brief 命令行解析 
	 * 
	 * @param key option 
	 * @param arg 内容 
	 * @param state  
	 * 
	 * @return 
	 */

	friend error_t aparse_opt(int key ,char * arg,struct argp_state *state);

protected:
	/** 
	 * @brief argp 环境对象 
	 */

	struct argp stargp;
	/** 
	 * @brief 解析函数  
	 */

	argsParser user_parser;
	/** 
	 * @brief 构造函数 
	 */

	aArg();
	/** 
	 * @brief 析构函数 
	 */

	~aArg();
public:
	/** 
	 * @brief 单件模式 删除对象 
	 */

	static void removeArg();
	/** 
	 * @brief 单件模式 获取对象 
	 * 
	 * @return aArg 对象 
	 */

	static aArg * getArg();
	/** 
	 * @brief 添加命令行解析内容 
	 * 
	 * @param options option 
	 * @param func  解析函数
	 * @param args_doc 说明
	 * @param doc 说明
	 * 
	 * @return 成功 true 
	 */

	bool add(const struct argp_option * options = 0,argsParser func = 0,const char * args_doc = 0,const char * doc = 0);
	/** 
	 * @brief 解析  
	 * 
	 * @param argc 参数个数 
	 * @param argv 参数内容
	 * 
	 * @return 成功 true
	 */

	bool parse(int argc, char* argv[]);
private:
	/** 
	 * @brief 单件模式下的aArg实例 
	 */

	static aArg * argInstance;
	/** 
	 * @brief 添加命令行参数
	 * 
	 * @param options 命令行参数
	 */

	void addOption(const struct argp_option *options);
	/** 
	 * @brief  命令行参数结构
	 */

	struct argp_option *allOptions;

};
#endif

