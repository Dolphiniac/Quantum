#include "Quantum/Core/String.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//-----------------------------------------------------------------------------------------------
QuString::QuString()
	: m_capacity(s_MIN_STRING_MEMORY_BUFFER)
	, m_length(0)
{
	m_stringPtr = new char[m_capacity];
	*m_stringPtr = '\0';
}


//-----------------------------------------------------------------------------------------------
QuString::QuString(const char* otherString)
{
	m_length = strlen(otherString);
	m_capacity = m_length + 1;
	m_stringPtr = new char[m_capacity];

	if (m_length > 0)
	{
		strcpy_s(m_stringPtr, m_capacity, otherString);
	}
}


//-----------------------------------------------------------------------------------------------
QuString::QuString(const QuString& otherString)
{
	m_length = otherString.GetLength();
	m_capacity = m_length + 1;
	m_stringPtr = new char[m_capacity];

	if (m_length > 0)
	{
		strcpy_s(m_stringPtr, m_capacity, otherString.GetRaw());
	}
}


//-----------------------------------------------------------------------------------------------
QuString::QuString(QuString&& otherString)
{
	m_length = otherString.m_length;
	m_capacity = otherString.m_capacity;
	m_stringPtr = otherString.m_stringPtr;

	otherString.m_stringPtr = nullptr;
}


//-----------------------------------------------------------------------------------------------
QuString::~QuString()
{
	SAFE_DELETE_ARRAY(m_stringPtr);
}


//-----------------------------------------------------------------------------------------------
void QuString::operator=(const QuString& otherString)
{
	m_length = otherString.GetLength();
	m_capacity = m_length + 1;
	m_stringPtr = new char[m_capacity];

	if (m_length > 0)
	{
		strcpy_s(m_stringPtr, m_capacity, otherString.GetRaw());
	}
}


//-----------------------------------------------------------------------------------------------
QuString QuString::operator+(const QuString& otherString) const
{
	QuString result = *this;
	size_t otherLen = otherString.m_length;
	result.m_length += otherLen;

	bool changedSize = false;
	while ((result.m_length + otherLen + 1) > result.m_capacity)
	{
		result.m_capacity *= 2;
		changedSize = true;
	}

	if (changedSize)
	{
		char* newBuff = new char[result.m_capacity];
		memcpy(newBuff, result.m_stringPtr, result.m_length);
		delete[] result.m_stringPtr;
		result.m_stringPtr = newBuff;
	}

	strcpy_s(result.m_stringPtr + result.m_length, otherString.m_length + 1, otherString.m_stringPtr);

	return result;
}


//-----------------------------------------------------------------------------------------------
void QuString::operator+=(const QuString& otherString)
{
	size_t otherLen = otherString.m_length;

	bool changedSize = false;
	while ((m_length + otherLen + 1) > m_capacity)
	{
		m_capacity *= 2;
		changedSize = true;
	}

	if (changedSize)
	{
		char* newBuff = new char[m_capacity];
		memcpy(newBuff, m_stringPtr, m_length);
		delete[] m_stringPtr;
		m_stringPtr = newBuff;
	}

	strcpy_s(m_stringPtr + m_length, otherString.m_length + 1, otherString.m_stringPtr);
	m_length += otherString.m_length;
}


//-----------------------------------------------------------------------------------------------
QuString& QuString::operator=(QuString&& otherString)
{
	if (this == &otherString)
	{
		delete[] m_stringPtr;

		m_stringPtr = otherString.m_stringPtr;
		m_length = otherString.m_length;
		m_capacity = otherString.m_capacity;

		otherString.m_stringPtr = nullptr;
	}

	return *this;
}


//-----------------------------------------------------------------------------------------------
STATIC QuString QuString::F(const QuString& formatString, ...)
{
#ifndef __X64
	char textLiteral[2048];
	va_list vargs;
	uint32 ptrSize = sizeof(void*);

	//Getting past the annoyance of not using reference types with vargs by manually moving the ptr myself
	__asm
	{
		lea eax, [formatString];
		add eax, [ptrSize];
		mov[vargs], eax;
	}
	vsnprintf_s(textLiteral, 2048, _TRUNCATE, formatString.GetRaw(), vargs);
	textLiteral[2047] = '\0';

	return textLiteral;
#else
	return "Cannot execute necessary assembly in x64 architecture.";
#endif
}