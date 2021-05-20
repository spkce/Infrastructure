#ifndef __LOG_INTERNAL_H__
#define __LOG_INTERNAL_H__

namespace Infra
{

void infraPrint(const char* file, int line, const char* func, const char* fmt, ...) __attribute__((format(printf, 4, 5)));

#define printex(fmt, ...) infraPrint(__FILE__, __LINE__, __FUNCTION__, (fmt), ## __VA_ARGS__)

} //Infra

#endif //__LOG_INTERNAL_H__
