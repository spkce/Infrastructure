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

*/



//#define trace(fmt,...) printf(fmt,## _VA_ARGS_)


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

void print_log(const char* fmt, int fontColor = Font_white, int backgroundColor = background_black, ...);


#endif //__LOG_H__
