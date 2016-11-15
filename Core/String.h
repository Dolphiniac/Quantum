#pragma once


//-----------------------------------------------------------------------------------------------
class QuString
{
	static const int8 s_MIN_STRING_MEMORY_BUFFER = 32;

public:
	//-----------------------------------------------------------------------------------------------
	//CTORS AND DTORS
	QuString();
	QuString(const char* otherString);
	QuString(const QuString& otherString);
	QuString(QuString&& otherString);
	~QuString();
	//-----------------------------------------------------------------------------------------------

public:
	//-----------------------------------------------------------------------------------------------
	//GETTERS AND SETTERS
	size_t GetLength() const { return m_length; }
	size_t GetCapacity() const { return m_capacity; }
	const char* GetRaw() const { return m_stringPtr; }
	//-----------------------------------------------------------------------------------------------
	
public:
	//-----------------------------------------------------------------------------------------------
	//OPERATORS
	void operator=(const QuString& otherString);
	QuString operator+(const QuString& otherString) const;
	void operator+=(const QuString& otherString);
	QuString& operator=(QuString&& otherString);
	//-----------------------------------------------------------------------------------------------

public:
	//-----------------------------------------------------------------------------------------------
	//UTILITY FUNCTIONS
	static QuString F(const QuString& formatString, ...);

private:
	char* m_stringPtr;
	size_t m_capacity;
	size_t m_length;
};