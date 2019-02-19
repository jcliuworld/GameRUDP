#include "aArg.h"
/** 
* @file aArg.cpp
* @brief 命令行解析 
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

/** 
* @brief 说明
*/

static const char age_args_doc[] = "";
/** 
* @brief 说明 
*/

static const char age_doc[] = "the defalut age doc";
/** 
* @brief 命令行结构
*/

static struct argp_option age_options[] =
{
	{0,0,0,0,0,0}
};
/** 
 * @brief 命令行解析 
 * 
 * @param key option 
 * @param arg 内容 
 * @param state  
 * 
 * @return 
 */

error_t aparse_opt(int key ,char * arg,struct argp_state *state)
{
	switch(key)
	{
		default:
			if (aArg::getArg()->user_parser != 0)
				return aArg::getArg()->user_parser(key,arg,state);
			else
				return ARGP_ERR_UNKNOWN;

	}
	return 0;
}
/** 
 * @brief 单件模式下的aArg实例 
 */

aArg * aArg::argInstance = NULL;
/** 
 * @brief 构造函数 
 */

aArg::aArg()
{
	user_parser = 0;
	allOptions = 0;

	stargp.children = 0;
	stargp.help_filter = 0;
	stargp.argp_domain = 0;

	stargp.parser =  aparse_opt;
	stargp.args_doc =   age_args_doc;
	stargp.doc = 	age_doc;
	addOption(age_options);
}
/** 
 * @brief 析构函数 
 */

aArg::~aArg()
{
	SAFE_DELETE_VEC(allOptions);
}
/** 
 * @brief 单件模式 获取对象 
 * 
 * @return aArg 对象 
 */

aArg * aArg::getArg()
{
	if(argInstance == NULL)
		argInstance = new aArg();
	return argInstance;
}
/** 
 * @brief 单件模式 删除对象 
 */

void aArg::removeArg()
{
	SAFE_DELETE(argInstance);
}
/** 
 * @brief 添加命令行参数
 * 
 * @param options 命令行参数
 */

void aArg::addOption(const struct argp_option *options)
{

	if(options == 0) return;

	int icount = 0;
	while(options[icount].name != 0 )
		icount ++ ;
	if (allOptions == 0)
	{
		allOptions = new struct argp_option[icount+1];
		memcpy(allOptions,options,sizeof(argp_option)*(icount +1));
	}else
	{
		int ocount = 0;
		while(allOptions[ocount].name != 0)
			ocount ++ ;
		struct argp_option * otemp =allOptions;
		allOptions = new argp_option[icount + ocount +1];
		if(ocount > 0) memcpy(allOptions,otemp,sizeof(argp_option)*ocount);
		memcpy(allOptions + ocount,options,sizeof(argp_option)*(icount+1));
		SAFE_DELETE_VEC(otemp);

	}
	stargp.options =allOptions;
}
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

bool aArg::add(const struct argp_option * options /* = 0 */,argsParser func /* = 0 */,const char * args_doc /* = 0 */,const char * doc /* = 0 */)
{
	if(func != 0)
		user_parser = func;
	if(options != 0)
		addOption(options);
	if(args_doc != 0 )
		stargp.args_doc = args_doc;
	if(doc != 0 )
		stargp.doc = doc;
	return true;
}
/** 
 * @brief 解析  
 * 
 * @param argc 参数个数 
 * @param argv 参数内容
 * 
 * @return 成功 true
 */

bool aArg::parse(int argc, char* argv[])
{
	argp_parse(&stargp,argc,argv,0,0,0);
	return true;
}
