#ifndef _aLogger_H__
#define _aLogger_H__
/** 
* @file aLogger.h
* @brief 日志系统 基于log4cxx
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/


#include "aMutex.h"
#include "aDef.h"

#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/dailyrollingfileappender.h>
#include <log4cxx/helpers/timezone.h>
#include <log4cxx/helpers/dateformat.h>

#define _MSGBUF_MAX 4096
/** 
* @brief 日志系统封装
*/

class aLogger
{
	public:
		/** 
		 * @brief 日志等级封装
		 */

		class aLevel
		{
			friend class aLogger;

			private:
			
			enum aLevelInt
			{
				ALARM_INT	=	log4cxx::Level::ERROR_INT,
				IFFY_INT	=	log4cxx::Level::WARN_INT,
				TRACE_INT	=	log4cxx::Level::INFO_INT,
				GBUG_INT	=	log4cxx::Level::DEBUG_INT

			};

			static const log4cxx::LevelPtr LEVELALARM;
			static const log4cxx::LevelPtr LEVELIFFY;
			static const log4cxx::LevelPtr LEVELTRACE;
			static const log4cxx::LevelPtr LEVELDEBUG;

			log4cxx::LevelPtr alevel;

			aLevel(log4cxx::LevelPtr level);

			public:

			static const aLevel * OFF;

			static const aLevel * FATAL;

			static const aLevel * ERROR;

			static const aLevel * ALARM;

			static const aLevel * WARN;

			static const aLevel * IFFY;

			static const aLevel * INFO;

			static const aLevel * TRACE;

			static const aLevel * DEBUG;

			static const aLevel * GBUG;

			static const aLevel * ALL;


		};
		/** 
		 * @brief 构造函数
		 * 
		 * @param name 日志文件头
		 */

		aLogger(const std::string & name="Age");
		/** 
		 * @brief 析构函数
		 */

		~aLogger();
		/** 
		 * @brief 获取日志文件名 
		 * 
		 * @return 日志文件名
		 */

		const std::string  getName();
		/** 
		 * @brief 设置日志文件名
		 * 
		 * @param setName 文件名
		 */

		void setName(const std::string & setName );
		/** 
		 * @brief 添加控制台日志
		 * 
		 * @return 成功 true 失败 false
		 */

		bool addConsoleLog();
		/** 
		 * @brief 移除控制台日志
		 */

		void removeConsoleLog();
		/** 
		 * @brief 添加本地日志文件
		 * 
		 * @param file 文件
		 * 
		 * @return 成功 true 失败 false
		 */

		bool addLocalFileLog( const std::string & file );
		/** 
		 * @brief 添加日期日志文件
		 * 
		 * @param file 文件
		 * 
		 * @return 成功 true 失败 false
		 */

		bool addDailyLocalFileLog( const std::string & file );
		/** 
		 * @brief 移除本地日志文件
		 * 
		 * @param file 文件
		 */

		void removeLocalFileLog( const std::string & file );
		/** 
		 * @brief 设置日志等级
		 * 
		 * @param aLevelPtr 日志等级
		 */

		void setLevel( const aLevel * aLevelPtr);
		/** 
		 * @brief 设置日志等级
		 * 
		 * @param level 日志等级
		 */

		void setLevel( const std::string & level);
		/** 
		 * @brief 写log
		 * 
		 * @param aLevelPtr 日志等级
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false
		 */

		bool log( const aLevel * aLevelPtr,const char * pattern, ... );
		/** 
		 * @brief 写log
		 * 
		 * @param aLevelPtr 日志等级
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false
		 */


		bool forceLog( const aLevel  * aLevelPtr, const char * pattern, ...);
		/** 
		 * @brief debug 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */

		bool debug( const char * pattern , ... );
		/** 
		 * @brief error 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool error( const char * pattern , ... );
		/** 
		 * @brief info 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool info( const char * pattern , ... );
		/** 
		 * @brief fatal 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool fatal( const char * pattern , ... );
		/** 
		 * @brief warn 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool warn( const char * pattern , ... );
		/** 
		 * @brief alarm 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool alarm( const char * pattern , ... );
		/** 
		 * @brief iffy 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool iffy( const char * pattern , ... );
		/** 
		 * @brief trace 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool trace( const char * pattern , ... );
		/** 
		 * @brief gbug 级别日志
		 * 
		 * @param pattern 参数
		 * @param ... 参数列表
		 * 
		 * @return 成功 true 失败 false 
		 */
		bool gbug( const char * pattern , ... );

	private:
#if 0 //升级log4cxx 取消代码
		class aLoggerLocalFileAppender : public log4cxx::DailyRollingFileAppender
		{
			public:
				aLoggerLocalFileAppender();
				~aLoggerLocalFileAppender();

				void setMyFile( const log4cxx::String &file);

				void setTimeZone(const log4cxx::String &timeZone);

				void activateOptions();

			private:

				static log4cxx::helpers::TimeZonePtr tz;

				log4cxx::String my_fileName;

				void myrollOver();

				void subAppend( const log4cxx::spi::LoggingEventPtr& event );

		};

		class powerLogger : public log4cxx::Logger
		{

			public:

				void setName( const log4cxx::String &s )
				{
					name = s;
				}

			protected:
				powerLogger(const log4cxx::String &s ):Logger( s ){}

		};
#endif
		log4cxx::LoggerPtr logger;
		char message[_MSGBUF_MAX];
		aMutex _mutex;

};

#endif
