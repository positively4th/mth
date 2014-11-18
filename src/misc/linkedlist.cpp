/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
// LinkedList.cpp: implementation of the LinkedList class.
//
//////////////////////////////////////////////////////////////////////

#include "linkedlist.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace P4th
{

template <class DTYPE>
LinkedList<DTYPE>::LinkedList()
{
  LinkedListPost<DTYPE>::next = 0L;
  size = 0;
  last = this;
 }

template <class DTYPE>
LinkedList<DTYPE>::LinkedList( const LinkedList &source )
{
	LinkedListPost<DTYPE>::next = 0L;
	Copy( source );
}

template <class DTYPE>
LinkedList<DTYPE>::LinkedList( const DTYPE &i)
{
	size = 1;
	LinkedListPost<DTYPE>::next = new LinkedListPost<DTYPE>( i );
	last = FindLast();
}

template <class DTYPE>
LinkedList<DTYPE>::~LinkedList()
{
	size = 0;
}

template <class DTYPE>
void LinkedList<DTYPE>::Delete()
{
	if ( LinkedListPost<DTYPE>::next != 0L  )
		delete LinkedListPost<DTYPE>::next;
	LinkedListPost<DTYPE>::next = 0L;
	size = 0;
	last = this;
}

template <class DTYPE>
void LinkedList<DTYPE>::Copy( const LinkedList &source )
{
	size = source.size;
	if ( LinkedListPost<DTYPE>::next != 0L )
		delete LinkedListPost<DTYPE>::next;
	if ( source.LinkedListPost<DTYPE>::next != 0L )
		LinkedListPost<DTYPE>::next = new LinkedListPost<DTYPE>( *source.LinkedListPost<DTYPE>::next );
	else
		LinkedListPost<DTYPE>::next = 0L;
	last = FindLast();
}
	  
template <class DTYPE>
LinkedListPost<DTYPE> *LinkedList<DTYPE>::FindLast() const
{
	if ( LinkedListPost<DTYPE>::next == 0L )
		return (LinkedListPost<DTYPE> *)this;
	LinkedListPost<DTYPE> *ptr = LinkedListPost<DTYPE>::next;
	while ( ptr->LinkedListPost<DTYPE>::next != 0L )
		ptr = ptr->LinkedListPost<DTYPE>::next;
	return ptr;
}

template <class DTYPE>
int LinkedList<DTYPE>::SafeExist( const unsigned int nr ) const
{
  if ( nr >= (unsigned int)0 && nr < size )
    return 1;
  return 0;
}

template <class DTYPE>
int LinkedList<DTYPE>::Exist( const DTYPE *d ) const
{
  if ( size > (unsigned int)0 )
    return LinkedListPost<DTYPE>::next->Exist( d );
  return 0;
}

template <class DTYPE>
int LinkedList<DTYPE>::Exist( const DTYPE &d ) const
{
	if ( size > 0 )
		return LinkedListPost<DTYPE>::next->Exist( d );
	return 0;
}

template <class DTYPE>
int LinkedList<DTYPE>::Index( DTYPE *it ) const
{
	LinkedListPost<DTYPE> *ptr = LinkedListPost<DTYPE>::next;
	for ( unsigned int i = 0 ; i < size ; i++ )
	{
		if ( LinkedListPost<DTYPE>::next == 0L && i < size - 1 )
			throw (unsigned char *) "int LinkedList<DTYPE>::Index( const DTYPE &it ) const: Internal size error\0";
		if ( it == &(ptr->item) )
			return i;
		ptr = ptr->LinkedListPost<DTYPE>::next;			// If error - check this
	}
	throw (unsigned char *)"int LinkedList<DTYPE>::Index( const DTYPE &it ) const: Item not found\0";
	return -1;	//Visual bug ?	
}

template <class DTYPE>
DTYPE LinkedList<DTYPE>::Get( const unsigned int nr ) const
{
  if( ! SafeExist( (unsigned int)nr ) )
    _DBTHROW( (unsigned char *)( "DTYPE LinkedList<DTYPE>::Get( int nr ) const: Index out of bounds" ) );

  LinkedListPost<DTYPE> *ptr = LinkedListPost<DTYPE>::next;
  for ( unsigned int i = 0 ; i < nr ; i++ )
    {
      if ( ptr == 0L )
	_DBTHROW( (unsigned char *) "DTYPE LinkedList<DTYPE>::Get( int nr ) const: Internal size error" );
      ptr = ptr->LinkedListPost<DTYPE>::next;
    }
  return ptr->item;
}

template <class DTYPE>
DTYPE &LinkedList<DTYPE>::Ref( const unsigned int nr ) const
{
	if( ! SafeExist( (unsigned int)nr ) )
		_DBTHROW( (unsigned char *)"DTYPE &LinkedList<DTYPE>::Ref( int nr ): Index out of bounds" );

	LinkedListPost<DTYPE> *ptr = LinkedListPost<DTYPE>::next;
	for ( unsigned int i = 0 ; i < nr ; i++ )
	{
		if ( ptr == 0L )
			_DBTHROW( (unsigned char *) "DTYPE &LinkedList<DTYPE>::Ref( int nr ): Internal size error" );
		ptr = ptr->LinkedListPost<DTYPE>::next;
	}
	return ptr->item;
}

template <class DTYPE>
void LinkedList<DTYPE>::Replace( int nr , const DTYPE &_new) 
{
	if ( ! SafeExist( (unsigned int)nr ) )
		_DBTHROW( (unsigned char *) "void LinkedList<DTYPE>::Replace( int nr , const DTYPE &_new): Index out of bounds" );
	LinkedListPost<DTYPE> *ptr = LinkedListPost<DTYPE>::next;
	for ( int i = 0 ; i < nr ; i++ )
	{
		ptr = ptr->LinkedListPost<DTYPE>::next;
		if ( ptr == 0L )
		  _DBTHROW( (unsigned char *)"void LinkedList<DTYPE>::Replace( int nr , const DTYPE &_new): Internal size error" );
	}
	ptr->Replace( _new );
	last = FindLast();
}

template <class DTYPE>
void LinkedList<DTYPE>::Add( const LinkedList<DTYPE> &l )
{
	if ( l.size == 0 )
		return;
	size += l.size;	
	last->LinkedListPost<DTYPE>::next = new LinkedListPost<DTYPE>( *l.LinkedListPost<DTYPE>::next );
	last = FindLast();
}

template <class DTYPE>
void LinkedList<DTYPE>::Add( const DTYPE &_new) 
{
	LinkedListPost<DTYPE> *tmp = new LinkedListPost<DTYPE>( _new );
	last->LinkedListPost<DTYPE>::next = tmp;
	last = tmp;
	size++;
	last = FindLast();
}

template <class DTYPE>
void LinkedList<DTYPE>::InsertAt( int index , const DTYPE &_new) 
{
  if ( ! SafeExist( (unsigned int)index ) )
    _DBTHROW( (unsigned char *)"void LinkedList<DTYPE>::Insert( int index , const DTYPE &_new): Index out of bounds" );
  
  LinkedListPost<DTYPE> *ptr = this;
  for ( int i = 0 ; i < index ; i++ )
    {
      if ( LinkedListPost<DTYPE>::next == 0L )
	_DBTHROW( (unsigned char *) "void LinkedList<DTYPE>::Insert( int index , const DTYPE &_new): Internal size error" );
      ptr = ptr->LinkedListPost<DTYPE>::next;
    }
  LinkedListPost<DTYPE> *tptr = new LinkedListPost<DTYPE>( _new );
  tptr->LinkedListPost<DTYPE>::next = ptr->LinkedListPost<DTYPE>::next;
  ptr->LinkedListPost<DTYPE>::next = tptr;
  size++;
  last = FindLast();
}
  
template <class DTYPE>
void LinkedList<DTYPE>::Insert( int index , const DTYPE &_new) 
{
  _DB( std::cerr << "Deprecieated use LinkedList::InsertAt( ... ) instead." << std::endl; );
  if ( ! SafeExist( (unsigned int)index ) )
    _DBTHROW( (unsigned char *)"void LinkedList<DTYPE>::Insert( int index , const DTYPE &_new): Index out of bounds" );
  
  LinkedListPost<DTYPE> *ptr = this;
  for ( int i = 0 ; i < index ; i++ )
    {
      if ( LinkedListPost<DTYPE>::next == 0L )
	_DBTHROW( (unsigned char *) "void LinkedList<DTYPE>::Insert( int index , const DTYPE &_new): Internal size error" );
      ptr = ptr->LinkedListPost<DTYPE>::next;
    }
  LinkedListPost<DTYPE> *tptr = new LinkedListPost<DTYPE>( _new );
  tptr->LinkedListPost<DTYPE>::next = ptr->LinkedListPost<DTYPE>::next;
  ptr->LinkedListPost<DTYPE>::next = tptr;
  size++;
  last = FindLast();
}

template <class DTYPE>
void LinkedList<DTYPE>::Insert( DTYPE *i , const DTYPE &_new) 
{
	int index = Index( i );
	Insert( index , _new );
}

template <class DTYPE>
void LinkedList<DTYPE>::Remove( int index ) 
{
  if ( ! SafeExist( (unsigned int)index ) )
    _DBTHROW( (unsigned char *)"void LinkedList<DTYPE>::Remove( int index ): Index out of bounds" );

	LinkedListPost<DTYPE> *ptr = this;

//	for ( int i = 0 ; i < index - 1 ; i++ )
	for ( int i = 0 ; i < index ; i++ )
	{
		if ( LinkedListPost<DTYPE>::next == 0L )
		  _DBTHROW( (unsigned char *)"void LinkedList<DTYPE>::Remove( int index ): Internal size error" );
		ptr = ptr->LinkedListPost<DTYPE>::next;
	}
	LinkedListPost<DTYPE> *tptr = ptr->LinkedListPost<DTYPE>::next;
	ptr->LinkedListPost<DTYPE>::next = ptr->LinkedListPost<DTYPE>::next->LinkedListPost<DTYPE>::next;
	
	tptr->LinkedListPost<DTYPE>::next = 0L;
	delete tptr;
	size--;
	last = FindLast();
}

template <class DTYPE>
void LinkedList<DTYPE>::Remove( DTYPE *i ) 
{
	int index = Index( i );
	Remove( index );
}

template <class DTYPE>
LinkedList<DTYPE> LinkedList<DTYPE>::operator=( const LinkedList<DTYPE> &ll )
{
	Copy( ll );
	return *this;
}

template <class DTYPE>
int LinkedList<DTYPE>::operator==( const LinkedList<DTYPE> &ll ) const
{
	if ( size != ll.size )
		return 0;
	if ( LinkedListPost<DTYPE>::next != 0L )
		return LinkedListPost<DTYPE>::next->operator==( *ll.LinkedListPost<DTYPE>::next );
	else
		return 1;
}

template <class DTYPE>
void HighLow( LinkedList<DTYPE> &list)
{
	LinkedListPost<DTYPE> *previous;
	LinkedListPost<DTYPE> *current;
	LinkedListPost<DTYPE> *coming;
	LinkedListPost<DTYPE> *tmp;

	for ( int last = list.size - 2 ; last >= 0 ; last-- )
	{
		previous = &list;
		current = list.LinkedListPost<DTYPE>::next;
		coming = current->LinkedListPost<DTYPE>::next;
		for ( int i = 0 ; i <= last ; i++ )
		{
			if ( (DTYPE)(current->item) < (DTYPE)(coming->item) )
			{
				previous->LinkedListPost<DTYPE>::next = coming;
				current->LinkedListPost<DTYPE>::next = coming->LinkedListPost<DTYPE>::next;
				coming->LinkedListPost<DTYPE>::next = current;
				tmp = current;
				current = coming;
				coming = coming->LinkedListPost<DTYPE>::next;
			}
			previous = current;
			current = coming;
			coming = coming->LinkedListPost<DTYPE>::next;
		}
	}
	list.last = list.FindLast();
}

template <class DTYPE>
void LowHigh( LinkedList<DTYPE> &list)
{

}

// Added 010423
template <class DTYPE>
int Index( LinkedList<DTYPE> &list , const DTYPE &m)
{
	int i;
	for ( i = 0; i < list.GetSize() ; i++ )
		if ( list.Ref( i ).Match( m ) )
			return i;
	return -1;
}

// End 010423
}

#include <string>
template class P4th::LinkedList<std::string>;
template class P4th::LinkedList<int>;
template class P4th::LinkedList<float>;
template class P4th::LinkedList<double>;
