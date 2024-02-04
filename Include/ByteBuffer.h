#ifndef __BYTE_BUFFER_H__
#define __BYTE_BUFFER_H__

#include "stdio.h"

namespace Infra
{

class CByteBuffer
{
public:
	CByteBuffer();

	CByteBuffer(unsigned int size);

	CByteBuffer(const CByteBuffer & o);

	CByteBuffer(CByteBuffer && o);

	virtual ~CByteBuffer();

	CByteBuffer& operator=(const CByteBuffer &o);

	CByteBuffer& operator=(CByteBuffer && o);

	void resize(unsigned int size);

	void append(const unsigned char *p, unsigned int len);

	void remove(int len);

	void clear();

	const unsigned char *getBuffer() const;

	unsigned int size() const;

	unsigned int capacity() const;

private:
	void adjust(unsigned int size);
	unsigned char *alloac(unsigned int size);

private:
	size_t m_begin;
	size_t m_end;
	size_t m_capacity;
	unsigned char *m_pBuffer;
};

} //Infra

#endif //__BYTE_BUFFER_H__
