#ifndef CSTR_H
#define CSTR_H

#include <stdlib.h>
#include <memory.h>
#include <string.h>

class CStr
{
public:
	CStr()
	{
		m_Text = NULL;
	}

	CStr( const CStr& source )
	{
		m_Text = NULL;
		if ( source.m_Text != NULL )
			Copy( source.m_Text );
	}
	
	CStr( const char* source )
	{
		m_Text = NULL;
		if ( source != NULL )
			Copy( source );
	}
	
	~CStr()
	{
		Free();
	}
	
	void Free()
	{
		if ( m_Text != NULL ) 
		{
			free( m_Text );
			m_Text = NULL;
		}
	}
	
	void Copy( const char* source )
	{
		Reserve( strlen( source ) + 1 );
		strcpy( m_Text, source );
	}
	
	void Reserve( unsigned int size )
	{
		if (m_Text)
		{
			free( m_Text );
		}	

		m_Text = (char*)malloc( size );
	}
	
	CStr& operator=( const char* source )
	{
		Free();

		if ( source != NULL )
			Copy( source );

		return *this;
	}

	int Compare( const CStr& string ) const
	{
		if ( m_Text == NULL && string.m_Text == NULL )
			return 0;

		if ( m_Text == NULL || string.m_Text == NULL )
			return -1;

		return strcmp( m_Text, string.m_Text );
	}

	int Compare( const char* string ) const
	{
		if ( m_Text == NULL && string == NULL )
			return 0;

		if ( m_Text == NULL || string == NULL )
			return -1;

		return strcmp( m_Text, string );
	}
	
	friend bool operator==( const CStr& stringA, const CStr& stringB )
	{
		return stringA.Compare( stringB ) == 0;
	}

	friend bool operator==( const CStr& stringA, const char* stringB )
	{
		return stringA.Compare( stringB ) == 0;
	}

	friend bool operator==( const char* stringA, const CStr& stringB )
	{
		return stringB.Compare( stringA ) == 0;
	}

	friend bool operator!=(const CStr& stringA, const CStr& stringB)
	{
		return stringA.Compare(stringB) == 0;
	}

	friend bool operator!=(const CStr& stringA, const char* stringB)
	{
		return stringA.Compare(stringB) != 0;
	}

	friend bool operator!=(const char* stringA, const CStr& stringB)
	{
		return stringB.Compare(stringA) != 0;
	}

	char& operator[]( const size_t idx )
	{
		return m_Text[idx];
	}

	void Clear()
	{
		Free();
	}

	bool IsEmpty()
	{
		return GetLength() == 0;
	}

	unsigned int GetLength()
	{
		if ( m_Text == NULL )
			return 0;
		
		return strlen( m_Text );
	}

	const char* GetText()
	{
		return (const char*)m_Text;
	}
	
private:
	char* m_Text;
};

#endif // !CSTR_H