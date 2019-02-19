#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <strings.h>
#include <stdio.h>

#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/helpers/dateformat.h>
#include <log4cxx/helpers/stringhelper.h>
#include <log4cxx/helpers/loglog.h>
#include <log4cxx/helpers/system.h>

#include "aLogger.h"

#include "aTime.h"

/** 
* @file aLogger.cpp
* @brief 日志系统 基于log4cxx
* @author ljc jc.liuworld@hotmail.com
* @date 2010-05-12
*/

const log4cxx::LevelPtr aLogger::aLevel::LEVELALARM(new log4cxx::Level(ALARM_INT,"ALARM",3));

const log4cxx::LevelPtr aLogger::aLevel::LEVELIFFY(new log4cxx::Level(IFFY_INT, "IFFY",3));

const log4cxx::LevelPtr aLogger::aLevel::LEVELTRACE(new log4cxx::Level(TRACE_INT, "TRACE",3));

const log4cxx::LevelPtr aLogger::aLevel::LEVELDEBUG(new log4cxx::Level(GBUG_INT, "GBUG",3));



const aLogger::aLevel * aLogger::aLevel::OFF = new aLogger::aLevel(log4cxx::Level::getOff());
const aLogger::aLevel * aLogger::aLevel::FATAL = new aLogger::aLevel(log4cxx::Level::getFatal());
const aLogger::aLevel * aLogger::aLevel::ALARM = new aLogger::aLevel(LEVELALARM);
const aLogger::aLevel * aLogger::aLevel::ERROR = new aLogger::aLevel(log4cxx::Level::getError());
const aLogger::aLevel * aLogger::aLevel::IFFY = new aLogger::aLevel(LEVELIFFY);
const aLogger::aLevel * aLogger::aLevel::WARN = new aLogger::aLevel(log4cxx::Level::getWarn());
const aLogger::aLevel * aLogger::aLevel::TRACE = new aLogger::aLevel(LEVELTRACE);
const aLogger::aLevel * aLogger::aLevel::INFO = new aLogger::aLevel(log4cxx::Level::getInfo());
const aLogger::aLevel * aLogger::aLevel::GBUG = new aLogger::aLevel(LEVELDEBUG);
const aLogger::aLevel * aLogger::aLevel::DEBUG = new aLogger::aLevel(log4cxx::Level::getDebug());
const aLogger::aLevel * aLogger::aLevel::ALL = new aLogger::aLevel(log4cxx::Level::getAll());

aLogger::aLevel::aLevel(log4cxx::LevelPtr level):alevel(level)
{

}
#if 0 //更新log4cxx 去掉部分代码
log4cxx::helpers::TimeZonePtr aLogger::aLoggerLocalFileAppender::tz(log4cxx::helpers::TimeZone::getDefault());


aLogger::aLoggerLocalFileAppender::aLoggerLocalFileAppender()
{

}

aLogger::aLoggerLocalFileAppender::~aLoggerLocalFileAppender()
{
	SAFE_DELETE(df)
}

#define remove_and_link(oldpath,newpath) \
	do {\
		remove(T2A(newpath.c_str()));\
		symlink(T2A(oldpath.c_str()),T2A(newpath.c_str()));\
	}while(false)

void aLogger::aLoggerLocalFileAppender::myrollOver()
{
	if( datePattern.empty())
	{
		errorHandler->error(_T("Missing Date Pattern option in myrollOver()."));
		return;
	}

	log4cxx::String dateFilename = my_fileName + df->format(now);


	if (scheduledFilename == dateFilename )
	{
		return;
	}

	this->closeWriter();

	try
	{
		this->setFile(dateFilename , this->fileAppend , this->bufferedIO,this->bufferSize );

		remove_and_link(dateFilename,my_fileName );
	}
	catch ( log4cxx::helpers::Exception &)
	{
		errorHandler->error(_T("setFile(") + dateFilename + _T(",false ) call failed "));
	}

	scheduledFilename = dateFilename;

}

void aLogger::aLoggerLocalFileAppender::subAppend( const log4cxx::spi::LoggingEventPtr& event)
{
	int64_t n = log4cxx::helpers::System::currentTimeMillis();

	if ( n >= nextCheck )
	{
		now = n;

		nextCheck = rc.getNextCheckMillis(now);


		try
		{
			myrollOver();
		}
		catch (log4cxx::helpers::Exception &e)
		{
			log4cxx::helpers::LogLog::error(_T("myrollPver() failed "),e);

		}
	}

	FileAppender::subAppend(event);

}

void aLogger::aLoggerLocalFileAppender::setMyFile(const log4cxx::String& file)
{
	my_fileName = log4cxx::helpers::StringHelper::trim(file);
}

void aLogger::aLoggerLocalFileAppender::setTimeZone(const log4cxx::String & timeZone)
{
	std::string tzstr;

	aRealTime::saveTimeZone(tzstr);

	tz = log4cxx::helpers::TimeZone::getTimeZone(timeZone);

	aRealTime::restoreTimeZone(tzstr);
}

void aLogger::aLoggerLocalFileAppender::activateOptions()
{
	rc.setTimeZone(tz);

	if (!datePattern.empty())
	{
		std::string tzstr;

		aRealTime::saveTimeZone(tzstr);

		rc.setType(rc.computeTriggeringPeriod(datePattern));

		aRealTime::restoreTimeZone(tzstr);

		now = log4cxx::helpers::System::currentTimeMillis();

		df = new log4cxx::helpers::DateFormat(datePattern,tz);

		if ( !my_fileName.empty())
		{
			scheduledFilename = my_fileName + df->format(now);

			try
			{
				struct stat my_fileStats;

				if (::lstat(T2A(my_fileName.c_str()),&my_fileStats) == 0 && S_ISREG(my_fileStats.st_mode))
				{
					log4cxx::String newFileName = my_fileName + df->format((int64_t)my_fileStats.st_mtime*1000);

					struct stat newFileStats;
					if (::lstat(T2A(newFileName.c_str()),&newFileStats) == 0 && S_ISREG(newFileStats.st_mode))
					{
						log4cxx::helpers::LogLog::error( my_fileName + _T("not soft link,must be rename to ") + newFileName + _T(",but the new file has been exists, you need to rename or delete or remove the same name file"));

						log4cxx::helpers::LogLog::error(_T("programm exit!"));
						exit(0);
					}
					{
						remove(T2A(newFileName.c_str()));

						std::string my_fileNametmp = T2A(my_fileName.c_str());
						std::string newFileNametmp = T2A(newFileName.c_str());
						if (::rename(my_fileNametmp.c_str(),newFileNametmp.c_str())==0)
						{
							log4cxx::helpers::LogLog::debug(my_fileName +_T("->") + newFileName);
						}
						else
						{
							log4cxx::helpers::LogLog::error(_T("failed to rename ")+my_fileName +_T("to") + newFileName );

							log4cxx::helpers::LogLog::error(_T("programm exit!"));
							exit(0);
						}
					}
				}

				this->setFile( scheduledFilename ,this->fileAppend,this->bufferedIO,this->bufferSize);
				remove_and_link( scheduledFilename ,my_fileName);

			}
			catch(log4cxx::helpers::Exception &e)
			{
				errorHandler->error(_T("unable to open file") +  scheduledFilename  , e,log4cxx::spi::ErrorCode::FILE_OPEN_FAILURE);
			}
		}
		else
		{
			log4cxx::helpers::LogLog::warn(_T("file option not set for appender") + name);
			log4cxx::helpers::LogLog::warn(_T("are you using fileappender instead of consoleappender"));
		}
	}else
	{
		log4cxx::helpers::LogLog::error(_T("data pattern options are not set for appender") + name);
	}
}
#endif


//控制台开关
static bool s_bFirst = true;
/** 
 * @brief 构造函数
 * 
 * @param name 日志文件头
 */
aLogger::aLogger(const std::string & name)
{
	bzero(message,sizeof(message));
	logger = log4cxx::Logger::getLogger(name);
	log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getDebug());
	if(s_bFirst)
	{
		s_bFirst = false;
		addConsoleLog();
	}
}
/** 
 * @brief 析构函数
 */


aLogger::~aLogger()
{

}
/** 
 * @brief 设置日志文件名
 * 
 * @param setName 文件名
 */


const std::string  aLogger::getName()
{
	std::string nametmp;
	nametmp = logger->getName();
	return nametmp;
}
/** 
 * @brief 设置日志文件名
 * 
 * @param setName 文件名
 */

void aLogger::setName(const std::string & setName)
{
	logger = log4cxx::Logger::getLogger(setName);
}

const char CONSOLE[] = "console";

/** 
 * @brief 添加控制台日志
 * 
 * @return 成功 true 失败 false
 */


bool aLogger::addConsoleLog()
{
	log4cxx::LoggerPtr root = log4cxx::Logger::getRootLogger();
	if(root->getAppender(CONSOLE)) return true;
	
	log4cxx::PatternLayoutPtr cpl = new log4cxx::PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");


	log4cxx::ConsoleAppenderPtr  carp = new log4cxx::ConsoleAppender(cpl);

	carp->setName(CONSOLE);
	root->addAppender(carp);
	
	return true;
}
/** 
 * @brief 添加本地日志文件
 * 
 * @param file 文件
 * 
 * @return 成功 true 失败 false
 */


void aLogger::removeConsoleLog()
{
	log4cxx::Logger::getRootLogger()->removeAppender(CONSOLE);
}
/** 
 * @brief 添加本地日志文件
 * 
 * @param file 文件
 * 
 * @return 成功 true 失败 false
 */

bool aLogger::addLocalFileLog(const std::string & file)
{
	log4cxx::PatternLayoutPtr fpl = new log4cxx::PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");

	log4cxx::DailyRollingFileAppender * farp = new log4cxx::DailyRollingFileAppender(fpl,file,"'.'yyMMdd-HH");
	farp->setName("localfile:"+file);
	
	log4cxx::Logger::getRootLogger()->addAppender(farp);

	return true;
}
/** 
* @brief 添加日期日志文件
* 
* @param file 文件
* 
* @return 成功 true 失败 false
*/
bool aLogger::addDailyLocalFileLog( const std::string & file )
{
	log4cxx::PatternLayoutPtr fpl = new log4cxx::PatternLayout("\%d{\%y\%m\%d-\%H:\%M:\%S }%c %5p: %m%n");

	log4cxx::DailyRollingFileAppender * farp = new log4cxx::DailyRollingFileAppender(fpl,file,"'.'yyMMdd");
	//log4cxx::DailyRollingFileAppender * farp = new log4cxx::DailyRollingFileAppender(fpl,file,"'.'yyyy-MM-dd-HH-mm");
	farp->setName("localfile:" + file);
	
	log4cxx::Logger::getRootLogger()->addAppender(farp);
	return true;
}

void aLogger::removeLocalFileLog(const std::string & file)
{
	log4cxx::Logger::getRootLogger()->getAppender("localfile:" + file);

}

#define  getMessage(msg,msglen,pattern) \
	do \
{ \
	va_list ap; \
	bzero(msg,msglen);\
	va_start(ap,pattern);\
	vsnprintf(msg , msglen - 1, pattern ,ap);\
	va_end(ap);\
}while(false)
/** 
 * @brief 设置日志等级
 * 
 * @param aLevelPtr 日志等级
 */


void aLogger::setLevel(const aLevel * aLevelPtr)
{
	logger->setLevel(aLevelPtr->alevel);
}
/** 
 * @brief 设置日志等级
 * 
 * @param level 日志等级
 */


void aLogger::setLevel(const std::string & level)
{
	if ("off" == level)
		setLevel(aLevel::OFF);
	else if ("fatal" == level)
		setLevel(aLevel::FATAL);
	else if ("alarm"==level)
		setLevel(aLevel::ALARM);
	else if ("error" == level)
		setLevel(aLevel::ERROR);
	else if ("iffy" == level)
		setLevel(aLevel::IFFY);
	else if ("warn" == level)
		setLevel(aLevel::WARN);
	else if ("trace" == level)
		setLevel(aLevel::TRACE);
	else if ("info" == level)
		setLevel(aLevel::INFO);
	else if ("gbug" == level)
		setLevel(aLevel::GBUG);
	else if ("debug" == level)
		setLevel(aLevel::DEBUG);
	else if ("all" == level)
		setLevel(aLevel::ALL);
}
/** 
 * @brief 写log
 * 
 * @param aLevelPtr 日志等级
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false
 */


bool aLogger::log( const aLevel * aLevelPtr,const char * pattern, ... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->log(aLevelPtr->alevel,message);
	_mutex.unlock();
	return true;
}
/** 
 * @brief 写log
 * 
 * @param aLevelPtr 日志等级
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false
 */


bool aLogger::forceLog( const aLevel * aLevelPtr,const char * pattern, ...)
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->log(aLevelPtr->alevel,message);
	_mutex.unlock();
	return true;
}
/** 
 * @brief fatal 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::fatal(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->fatal(message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief error 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::error(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->error(message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief warn 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::warn(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->warn(message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief info 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::info(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->info(message);
	_mutex.unlock();
	return true;

}/** 
  * @brief debug 级别日志
  * 
  * @param pattern 参数
  * @param ... 参数列表
  * 
  * @return 成功 true 失败 false 
  */

bool aLogger::debug(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->debug(message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief alarm 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::alarm(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->log(aLevel::ALARM->alevel,message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief iffy 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::iffy(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->log(aLevel::IFFY->alevel,message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief tarce 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::trace(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->log(aLevel::TRACE->alevel,message);
	_mutex.unlock();
	return true;

}
/** 
 * @brief gbug 级别日志
 * 
 * @param pattern 参数
 * @param ... 参数列表
 * 
 * @return 成功 true 失败 false 
 */

bool aLogger::gbug(const char * pattern,... )
{
	_mutex.lock();
	getMessage(message,_MSGBUF_MAX,pattern);
	logger->log(aLevel::GBUG->alevel,message);
	_mutex.unlock();
	return true;

}
