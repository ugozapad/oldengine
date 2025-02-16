#ifndef CARRAY_H
#define CARRAY_H

// Based of DXUTILS

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>

// placement new
#include <new.h>

#include "ctypes.h"

//--------------------------------------------------------------------------------------
// A growable array
//--------------------------------------------------------------------------------------
template< typename TYPE >
class CArray
{
public:
    CArray()  { m_pData = NULL; m_nSize = 0; m_nMaxSize = 0; }
    CArray( const CArray<TYPE>& a ) { for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); }
    ~CArray() { RemoveAll(); }

    const TYPE& operator[]( int nIndex ) const { return GetAt( nIndex ); }
    TYPE& operator[]( int nIndex ) { return GetAt( nIndex ); }
   
    CArray& operator=( const CArray<TYPE>& a ) { if( this == &a ) return *this; RemoveAll(); for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); return *this; }

    bool    SetSize( int nNewMaxSize );
    void    Add( const TYPE& value );
    void    Insert( int nIndex, const TYPE& value );
    void    SetAt( int nIndex, const TYPE& value );
    TYPE&   GetAt( int nIndex ) const { assert( nIndex >= 0 && nIndex < m_nSize ); return m_pData[nIndex]; }
    int     GetSize() const { return m_nSize; }
    TYPE*   GetData() { return m_pData; }
    bool    Contains( const TYPE& value ){ return ( -1 != IndexOf( value ) ); }
	bool	Empty() { return GetSize() == 0; }

    int     IndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? IndexOf( value, 0, m_nSize ) : -1; }
    int     IndexOf( const TYPE& value, int iStart ) { return IndexOf( value, iStart, m_nSize - iStart ); }
    int     IndexOf( const TYPE& value, int nIndex, int nNumElements );

    int     LastIndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? LastIndexOf( value, m_nSize-1, m_nSize ) : -1; }
    int     LastIndexOf( const TYPE& value, int nIndex ) { return LastIndexOf( value, nIndex, nIndex+1 ); }
    int     LastIndexOf( const TYPE& value, int nIndex, int nNumElements );

    void    Remove( int nIndex );
    void    RemoveAll() { SetSize(0); }
    void	Reset() { m_nSize = 0; }

protected:
    TYPE* m_pData;      // the actual array of data
    int m_nSize;        // # of elements (upperBound - 1)
    int m_nMaxSize;     // max allocated

    bool SetSizeInternal( int nNewMaxSize );  // This version doesn't call ctor or dtor.
};

//--------------------------------------------------------------------------------------
// Implementation of CArray
//--------------------------------------------------------------------------------------

// This version doesn't call ctor or dtor.
template< typename TYPE >
bool CArray<TYPE>::SetSizeInternal( int nNewMaxSize )
{
    if( nNewMaxSize < 0 || ( nNewMaxSize > INT_MAX / sizeof( TYPE ) ) )
    {
        assert( false );
        return false; //E_INVALIDARG;
    }

    if( nNewMaxSize == 0 )
    {
        // Shrink to 0 size & cleanup
        if( m_pData )
        {
            free( m_pData );
            m_pData = NULL;
        }

        m_nMaxSize = 0;
        m_nSize = 0;
    }
    else if( m_pData == NULL || nNewMaxSize > m_nMaxSize )
    {
        // Grow array
        int nGrowBy = ( m_nMaxSize == 0 ) ? 16 : m_nMaxSize;

        // Limit nGrowBy to keep m_nMaxSize less than INT_MAX
        if( (uint)m_nMaxSize + (uint)nGrowBy > (uint)INT_MAX )
            nGrowBy = INT_MAX - m_nMaxSize;

        nNewMaxSize = __max( nNewMaxSize, m_nMaxSize + nGrowBy );

        // Verify that (nNewMaxSize * sizeof(TYPE)) is not greater than UINT_MAX or the realloc will overrun
        if( sizeof(TYPE) > UINT_MAX / (uint)nNewMaxSize )
            return false; //E_INVALIDARG;

        TYPE* pDataNew = (TYPE*) realloc( m_pData, nNewMaxSize * sizeof(TYPE) );
        if( pDataNew == NULL )
            return false; //E_OUTOFMEMORY;

        m_pData = pDataNew;
        m_nMaxSize = nNewMaxSize;
    }

    return true;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
bool CArray<TYPE>::SetSize( int nNewMaxSize )
{
    int nOldSize = m_nSize;

    if( nOldSize > nNewMaxSize )
    {
        assert( m_pData );
        if( m_pData )
        {
            // Removing elements. Call dtor.

            for( int i = nNewMaxSize; i < nOldSize; ++i )
                m_pData[i].~TYPE();
        }
    }

    // Adjust buffer.  Note that there's no need to check for error
    // since if it happens, nOldSize == nNewMaxSize will be true.)
    bool hr = SetSizeInternal( nNewMaxSize );

    if( nOldSize < nNewMaxSize )
    {
        assert( m_pData );
        if( m_pData )
        {
            // Adding elements. Call ctor.

            for( int i = nOldSize; i < nNewMaxSize; ++i )
                ::new (&m_pData[i]) TYPE;
        }
    }

    return hr;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
void CArray<TYPE>::Add( const TYPE& value )
{  
    bool hr = SetSizeInternal( m_nSize + 1 );
    assert( hr );

    // Construct the new element
    ::new (&m_pData[m_nSize]) TYPE;

    // Assign
    m_pData[m_nSize] = value;
    ++m_nSize;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
void CArray<TYPE>::Insert( int nIndex, const TYPE& value )
{
    // Validate index
    if( nIndex < 0 || 
        nIndex > m_nSize )
    {
        assert( false );
    }

    // Prepare the buffer
    bool result = SetSizeInternal( m_nSize + 1 );
	assert( result );

    // Shift the array
    //MoveMemory( &m_pData[nIndex+1], &m_pData[nIndex], sizeof(TYPE) * (m_nSize - nIndex) );
	memmove( &m_pData[nIndex+1], &m_pData[nIndex], sizeof(TYPE) * (m_nSize - nIndex) );

    // Construct the new element
    ::new (&m_pData[nIndex]) TYPE;

    // Set the value and increase the size
    m_pData[nIndex] = value;
    ++m_nSize;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
void CArray<TYPE>::SetAt( int nIndex, const TYPE& value )
{
    // Validate arguments
    if( nIndex < 0 ||
        nIndex >= m_nSize )
    {
        assert( false );
    }

    m_pData[nIndex] = value;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the first occurrence
// within the section of the data array that extends from iStart and contains the 
// specified number of elements. Returns -1 if value is not found within the given 
// section.
//--------------------------------------------------------------------------------------
template< typename TYPE >
int CArray<TYPE>::IndexOf( const TYPE& value, int iStart, int nNumElements )
{
    // Validate arguments
    if( iStart < 0 || 
        iStart >= m_nSize ||
        nNumElements < 0 ||
        iStart + nNumElements > m_nSize )
    {
        assert( false );
        return -1;
    }

    // Search
    for( int i = iStart; i < (iStart + nNumElements); i++ )
    {
        if( value == m_pData[i] )
            return i;
    }

    // Not found
    return -1;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the last occurrence
// within the section of the data array that contains the specified number of elements
// and ends at iEnd. Returns -1 if value is not found within the given section.
//--------------------------------------------------------------------------------------
template< typename TYPE >
int CArray<TYPE>::LastIndexOf( const TYPE& value, int iEnd, int nNumElements )
{
    // Validate arguments
    if( iEnd < 0 || 
        iEnd >= m_nSize ||
        nNumElements < 0 ||
        iEnd - nNumElements < 0 )
    {
        assert( false );
        return -1;
    }

    // Search
    for( int i = iEnd; i > (iEnd - nNumElements); i-- )
    {
        if( value == m_pData[i] )
            return i;
    }

    // Not found
    return -1;
}



//--------------------------------------------------------------------------------------
template< typename TYPE >
void CArray<TYPE>::Remove( int nIndex )
{
    if( nIndex < 0 || 
        nIndex >= m_nSize )
    {
        assert( false );
    }

    // Destruct the element to be removed
    m_pData[nIndex].~TYPE();

    // Compact the array and decrease the size
    //MoveMemory( &m_pData[nIndex], &m_pData[nIndex+1], sizeof(TYPE) * (m_nSize - (nIndex+1)) );
    memmove( &m_pData[nIndex], &m_pData[nIndex+1], sizeof(TYPE) * (m_nSize - (nIndex+1)) );
	--m_nSize;
}

#endif

