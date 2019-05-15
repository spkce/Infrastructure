
#include "packet.h"

void packet_test(void)
{
	char buf[4096+4];
	memset(buf, 0, 4096);
	buf[4096+1] = '1';
	buf[4096+2] = '2';
	buf[4096+3] = '3';
	buf[4096+4] = '\0';
	CPacket a;
	a.append(buf,40796+4);
}