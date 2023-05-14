#include "ByteBuffer.h"
#include <string>
#include "string.h"

void ByteBuffer_test(void)
{
	Infra::CByteBuffer buffer;

	const char* s1 = "aaaa,aaaabbbbbbbb";
	size_t len1 = strlen(s1);
	buffer.append((const unsigned char*)s1, len1);

	const char* s2 = "cccccccceeeeeeee";
	size_t len2 = strlen(s2);
	buffer.append((const unsigned char*)s2, len2);

	buffer.append((const unsigned char*)"", 1);

	
	std::string str1 = (const char*)buffer.getBuffer();

	printf("\033[0;35m""str1 = %s""\033[0m\n", str1.c_str());
	printf("\033[0;35m""len1 = %ld""\033[0m\n", len1);
	printf("\033[0;35m""len2 = %ld""\033[0m\n", len2);


	buffer.remove(-10);
	std::string str2 = (const char*)buffer.getBuffer();
	printf("\033[0;35m""str2 = %s""\033[0m\n", str2.c_str());


	buffer.remove(5);
	std::string str3 = (const char*)buffer.getBuffer();
	printf("\033[0;35m""str3 = %s""\033[0m\n", str3.c_str());
	
	const char* s3 = "1111111111222222222233333333334444444444";
	buffer.append((const unsigned char*)s3, strlen(s3));
	buffer.append((const unsigned char*)"", 1);

	std::string str4 = (const char*)buffer.getBuffer();
	printf("\033[0;35m""str4 = %s""\033[0m\n", str4.c_str());


	Infra::CByteBuffer buffer2(1000);
	buffer2.append((const unsigned char*)s3, strlen(s3));
	std::string str5 = (const char*)buffer.getBuffer();
	printf("\033[0;35m""str5 = %s""\033[0m\n", str5.c_str());

}