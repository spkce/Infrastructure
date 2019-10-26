
#include "StrParser.h"
#include <algorithm>
#include <string.h>


char* StrParser::memMem(char* pBuf, int len, char* pSub, int subLen)
{
	if (pBuf == NULL || pSub == NULL || len == 0 ||subLen == 0)
	{
		return NULL;
	}

	char* pos == std::search(pBuf, pBuf + len, pSub, pSub + subLen);
	if (pos == pBuf + len)
	{
		return NULL;
	}
	else
	{
		return pos;
	}
}
char* StrParser::skipCut(char** pBuf, const char *delimiters)
{
	char* bengin_word = *pBuf;
	char* end_word = bengin_word + strcspn(bengin_word, delimiters);
	char*end_delimiters = end_word + strspn(end_word,delimiters);

	for(char* p = end_word; p < end_delimiters; p++)
	{
		*p = '\0';
	}
	*pBuf = end_delimiters;
	return bengin_word;
}
