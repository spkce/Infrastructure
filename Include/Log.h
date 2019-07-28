#ifndef __LOG_H__
#define __LOG_H__

void print_backtrace();
/*
字背景颜色范围: 40--49                   字颜色: 30—39             
 40: 黑                           30: 黑                 
 41: 红                           31: 红                
 42: 绿                           32: 绿                
 43: 黄                           33: 黄                
 44: 蓝                           34: 蓝               
 45: 紫                           35: 紫                 
 46: 深绿                       36: 深绿                 
 47: 白色                       37: 白色
ANSI控制码:
 QUOTE: 
   \033[0m   关闭所有属性      
   \033[1m   设置高亮度      
   \033[4m   下划线      
   \033[5m   闪烁      
   \033[7m   反显      
  \033[8m   消隐    
  \033[30m   --   \033[37m   设置前景色     
  \033[40m   --   \033[47m   设置背景色      
  \033[nA   光标上移n行     
  \03[nB   光标下移n行      
  \033[nC   光标右移n行     
  \033[nD   光标左移n行
*/

class CLog
{
public:
	enum emFontColor
	{
		Font_black = 30,
		Font_red,
		Font_green,
		Font_yellow,
		Font_blue,
		Font_violet,
		Font_darkGreen,
		Font_white
	};
	enum emBackgroundColor
	{
		background_black = 40,
		background_red,
		background_green,
		background_yellow,
		background_blue,
		background_violet,
		background_darkGreen,
		background_white
	};
	enum emLogLevel
	{
		logLevel_0, //关闭所有打印
		logLevel_1,
		logLevel_2,
		logLevel_3,
		logLevel_4,
		logLevel_5
	};
	enum emLogType
	{
		type_onlyLog = 0,
		type_fileMsg,
		type_modMsg
	};
public:
	CLog(int logType, std::string name, std::string ver);
	~CLog();
	int setLogLevel(int lv);
	int getLogLevel();
	int getLogType();
	std::string getName();
	std::string getVer();

	//lv5
	void info(const char* file, int line, const char* func, const char* fmt, ...) __attribute__((format(printf, 5, 6)));
	//lv4
	void debug(const char* file, int line, const char* func, const char* fmt, ...) __attribute__((format(printf, 5, 6)));
	//lv3
	void trace(const char* file, int line, const char* func, const char* fmt, ...) __attribute__((format(printf, 5, 6)));
	//lv2
	void warning(const char* file, int line, const char* func, const char* fmt, ...) __attribute__((format(printf, 5, 6)));
	//lv1
	void error(const char* file, int line, const char* func, const char* fmt, ...) __attribute__((format(printf, 5, 6)));
	
private:
	int m_logType;
	int m_printLogLevel;
	std::string m_name;
	std::string m_ver;
};


#endif //__LOG_H__
