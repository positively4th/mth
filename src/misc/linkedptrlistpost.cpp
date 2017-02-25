/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
//////////////////////////////////////////////////////////////////////
//
// LinkeListPost.cpp: implementation of the LinkeListPost class.
//
//////////////////////////////////////////////////////////////////////

#include "linkedptrlistpost.h"
#include <string>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace P4th
{


template <class DTYPE>
LinkedPtrListPost<DTYPE>::LinkedPtrListPost( )
{
	next = 0L;
	item = 0L;
}

template <class DTYPE>
LinkedPtrListPost<DTYPE>::LinkedPtrListPost( const DTYPE source )
{
	item = source;
	next = 0L;
}

template <class DTYPE>
LinkedPtrListPost<DTYPE>::LinkedPtrListPost( const LinkedPtrListPost<DTYPE> &source )
{
	next = 0L;
	Copy( source );
}

template <class DTYPE>
void LinkedPtrListPost<DTYPE>::Copy( const LinkedPtrListPost<DTYPE> &source )
{
	item = source.item;
	if ( next != 0L )
		delete next;
	if ( source.next != 0L )
		next = new LinkedPtrListPost<DTYPE>( *source.next );
	else
		next = 0L;
}

template <class DTYPE>
LinkedPtrListPost<DTYPE>::~LinkedPtrListPost()
{
  if ( next != 0L )
    delete next;
}

template <class DTYPE>
void LinkedPtrListPost<DTYPE>::Free()
{
  if ( next != 0L )
    {
      next->Free();
    }
  if ( item != 0L )
    delete item;
}

/*
template <class DTYPE>
int LinkedPtrListPost<DTYPE>::Exist( const DTYPE &b ) const
*/
/*

	Searches this and remaining items for a match.
	Return 1 if there is a match zero else.

*/
/*
{
	if ( b == *item )
		return 1;
	if ( next != 0L )
		return next->Exist( b );
	return 0;
}
*/

template <class DTYPE>
int LinkedPtrListPost<DTYPE>::Exist( const DTYPE b ) const
/*

	Searches this and remaining items for a match.
	Return 1 if there is a match zero else.

*/
{
	if ( b == item )
		return 1;
	if ( next != 0L )
		return next->Exist( b );
	return 0;
}

template <class DTYPE>
DTYPE LinkedPtrListPost<DTYPE>::Get( const DTYPE b ) const
/*

	Searches this and remaining items for a match.
	Return item if there is a match else throws an exception.

*/
{
	if ( b == item )
		return item;
	if ( next == 0L )
		throw (std::string)"Error:Item not found";
	return next->Get( b );
}

/*
template <class DTYPE>
void LinkedPtrListPost<DTYPE>::Replace( const DTYPE old , DTYPE _new ) 
*/
/*

	Searches this and remaining items for a match and replaces.
	Return item if there is a match else throws an exception.

*/
/*
{
	if ( old == item )
	{
		item = _new;
		return;
	}
	if ( next == 0L )
		throw (std::string)"Error:Item not found";
	next->Replace( old , _new );
}
*/

template <class DTYPE>
void LinkedPtrListPost<DTYPE>::Replace( const DTYPE old , const DTYPE _new ) 
/*

	Searches this and remaining items for a match and replaces.
	Return item if there is a match else throws an exception.

*/
{
	if ( old == item )
	{
		item = _new;
		return;
	}
	if ( next == 0L )
		throw (std::string) "Error:Item not found";
	next->Replace( old , _new );
}

template <class DTYPE>
LinkedPtrListPost<DTYPE> LinkedPtrListPost<DTYPE>::operator=( const LinkedPtrListPost<DTYPE> &llp )
{
	Copy( llp );
	return *this;
}

template <class DTYPE>
int LinkedPtrListPost<DTYPE>::operator==( const LinkedPtrListPost<DTYPE> &ll ) const
{
	if ( ! ( item == ll.item ) )
		return 0;
	if ( next == 0L && ll.next == 0L )
		return 1;
	if ( next != 0L && ll.next != 0L )
		return *next == *ll.next;
	throw (std::string)"Error:opertor==";
	return 0;
}

template class P4th::LinkedPtrListPost<float *>;
template class P4th::LinkedPtrListPost<double *>;

}
