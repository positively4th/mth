/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
// LinkeListPost.cpp: implementation of the LinkeListPost class.
//
//////////////////////////////////////////////////////////////////////


#include "linkedlistpost.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace P4th
{

	template <class DTYPE>
LinkedListPost<DTYPE>::LinkedListPost( )
{
// Removed 010423
//	item;
// End Removed 010423
	next = 0L;
}

template <class DTYPE>
 LinkedListPost<DTYPE>::LinkedListPost( const DTYPE &source ) : item( source )
{
  //	item = source;
	next = 0L;
}

template <class DTYPE>
LinkedListPost<DTYPE>::LinkedListPost( const LinkedListPost<DTYPE> &source )
{
	next = 0L;
	Copy( source );
}

template <class DTYPE>
void LinkedListPost<DTYPE>::Copy( const LinkedListPost<DTYPE> &source )
{
	item = source.item;
	if ( next != 0L )
		delete next;
	if ( source.next != 0L )
		next = new LinkedListPost<DTYPE>( *source.next );
	else
		next = 0L;
}

template <class DTYPE>
LinkedListPost<DTYPE>::~LinkedListPost()
{
	if ( next != 0L )
		delete next;
}

template <class DTYPE>
int LinkedListPost<DTYPE>::Exist( const DTYPE *b ) const
/*

	Searches this and remaining items for a match.
	Return 1 if there is a match zero else.

*/
{
	if ( b == &item )
		return 1;
	if ( next != 0L )
		return next->Exist( b );
	return 0;
}

template <class DTYPE>
int LinkedListPost<DTYPE>::Exist( const DTYPE &b ) const
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
DTYPE &LinkedListPost<DTYPE>::Ref( DTYPE *b )
/*

	Searches this and remaining items for a match.
	Return item if there is a match else throws an exception.

*/
{
	if ( b == &item )
		return item;
	if ( next == 0L )
		throw (unsigned char *) "DTYPE &LinkedListPost<DTYPE>::Ref( const DTYPE &b ): Item not found";
	return next->Ref( b );
}

template <class DTYPE>
DTYPE LinkedListPost<DTYPE>::Get( DTYPE *b ) const
/*

	Searches this and remaining items for a match.
	Return item if there is a match else throws an exception.

*/
{
	if ( b == &item )
		return item;
	if ( next == 0L )
		throw (unsigned char *) "DTYPE LinkedListPost<DTYPE>::Get( const DTYPE &b ) const: Item not found";
	return next->Get( b );
}

template <class DTYPE>
void LinkedListPost<DTYPE>::Replace( const DTYPE &old , const DTYPE &_new ) 
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
		throw (unsigned char *)"void LinkedListPost<DTYPE>::Replace( const DTYPE &old , const DTYPE &_new ): Item not found";
	next->Replace( old , _new );
}

template <class DTYPE>
LinkedListPost<DTYPE> LinkedListPost<DTYPE>::operator=( const LinkedListPost<DTYPE> &llp )
{
	Copy( llp );
	return *this;
}

template <class DTYPE>
int LinkedListPost<DTYPE>::operator==( const LinkedListPost<DTYPE> &ll ) const
{
	if ( ! ( item == ll.item ) )
		return 0;
	if ( next == 0L && ll.next == 0L )
		return 1;
	if ( next != 0L && ll.next != 0L )
		return *next == *ll.next;
	throw (unsigned char *)"int LinkedListPost<DTYPE>::operator==( const LinkedListPost<DTYPE> &ll ) const: opertor==";
	return 0;
}

}

#include <string>
template class P4th::LinkedListPost<std::string>;
template class P4th::LinkedListPost<int>;
template class P4th::LinkedListPost<double>;
template class P4th::LinkedListPost<float>;
