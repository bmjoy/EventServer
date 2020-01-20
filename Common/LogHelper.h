#pragma once

#include <string>
#include <memory>
#include <cstdio>
#include <cstdarg>
#include <thread>
#include <iostream>
#include <sstream>
#include <fstream>
#include "ThreadBase.h"
#include "ConcurrentQueue.hpp"
//#include "BlockQueue.hpp"
#include "FileHelper.h"
#include "SePlatForm.h"
#include "Util.h"
#include "CommDef.h"

enum LogLevel
{
	E_LOG_DEBUG = 1,
	E_LOG_INFO = 2,
	E_LOG_WARN = 3,
	E_LOG_ERR = 4,
	E_LOG_FATAL = 5,
};

enum LogRollType
{
	E_ROLL_SIZE = 1,
	E_ROLL_HOUR = 2,
	E_ROLL_DAY = 3,
};

class NullStream
{
public:
	inline NullStream& operator<<(std::ostream& (*)(std::ostream&)) {
		return *this;
	}

	template <typename T>
	inline NullStream& operator<<(const T&) {
		return *this;
	}

private:
};

class LogStream : public NullStream
{
public:
	LogStream() {}
	~LogStream() {}

	void Init(int level, const char* file, const char* func, int line);
	void Clear();
	template<typename T>
	LogStream& operator<<(const T& log)
	{
		m_oss << log;
		return *this;
	}

	LogStream& operator<<(std::ostream& (*log)(std::ostream&))
	{
		Clear();
		return *this;
	}

	std::ostringstream m_oss;

	int m_level{ 1 };
	int m_line{ 0 };
	const char* m_file{nullptr};
	const char* m_func{nullptr};

};

class LogHelper {
public:
	LogHelper(){}
	~LogHelper(){}

	bool Init(std::string servername = "");
	void Log(int level, const char* file, const char* func, int line, const char* fmt, ...);
	
	void Start();
	void Stop();
	bool LoadLogCfg(std::string& logcfg);
	bool LoadLogCfg(const char* logcfg);
	int GetLevel() { return m_level; }

	LogStream& Stream(int level, const char* file, const char* func, int line);
	ConcurrentQueue<std::string>& GetQueue() { return m_queue; }
	//Queue<std::string>& GetQueue() {return m_queue;}
private:
	void ThreadLoop();
	bool SendLog();
	void SetLogLevel(int level);
	bool CreateLog();
private:
	ConcurrentQueue<std::string> m_queue;
	//Queue<std::string> m_queue;
	std::string m_LogPath;
	std::ostringstream m_oss;
	std::thread m_thread;
	int m_level;
	LogStream m_logstream;
	bool m_stop{ false };
	
	int m_roll_type;
	int64_t m_roll_size;
	std::string m_logname;
	std::string m_servername;
	int64_t m_lasttime;
	FileC m_filec;

	TimeOut m_timeout;
};

extern std::unique_ptr<LogHelper> g_pLog;

#define LOG_DEBUG(...)   g_pLog->Log(E_LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    g_pLog->Log(E_LOG_INFO, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)    g_pLog->Log(E_LOG_WARN, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...)     g_pLog->Log(E_LOG_ERR, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...)   g_pLog->Log(E_LOG_FATAL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

// 流风格日志输出
#define CLOG_DEBUG g_pLog->Stream(E_LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_INFO g_pLog->Stream(E_LOG_INFO, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_WARN g_pLog->Stream(E_LOG_WARN, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_ERR g_pLog->Stream(E_LOG_ERR, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_FATAL g_pLog->Stream(E_LOG_FATAL, __FILE__, __FUNCTION__, __LINE__)

#define CLOG_END std::endl

#define INIT_SFLOG(a) do{\
g_pLog = std::make_unique<LogHelper>();\
g_pLog->LoadLogCfg(LOG_CFG_PATH);\
g_pLog->Init(a);\
g_pLog->Start();\
}while (0);

#define STOP_SFLOG() do{\
g_pLog->Stop();\
}while (0);

