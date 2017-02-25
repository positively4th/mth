/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
// LinkedPtrList.cpp: implementation of the LinkedPtrList class.
//
//////////////////////////////////////////////////////////////////////

#include "linkedptrlist.h"
#include <string>
#include <debug.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace P4th
{

template <class DTYPE>
LinkedPtrList<DTYPE>::LinkedPtrList()
{
  size = 0;
  LinkedPtrListPost<DTYPE>::next = 0L;
  last = this;
}

template <class DTYPE>
LinkedPtrList<DTYPE>::LinkedPtrList( const LinkedPtrList &source )
{
	LinkedPtrListPost<DTYPE>::next = 0L;
	Copy( source );
}

template <class DTYPE>
LinkedPtrList<DTYPE>::LinkedPtrList( DTYPE i)
{
	size = 1;
	LinkedPtrListPost<DTYPE>::next = new LinkedPtrListPost<DTYPE>( i );
	last = FindLast();
}

template <class DTYPE>
LinkedPtrList<DTYPE>::~LinkedPtrList()
{
// Change 010424
	Delete();
// End Change 010424
	size = 0;
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Delete()
{
  if ( LinkedPtrListPost<DTYPE>::next != 0L  )
    delete LinkedPtrListPost<DTYPE>::next;
  LinkedPtrListPost<DTYPE>::next = 0L;
  size = 0;
  last = this;
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Free()
{
  if ( LinkedPtrListPost<DTYPE>::next != 0L  )
    LinkedPtrListPost<DTYPE>::next->Free();

  Delete();
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Copy( const LinkedPtrList &source )
{
	size = source.size;
	if ( LinkedPtrListPost<DTYPE>::next != 0L )
		delete LinkedPtrListPost<DTYPE>::next;
	if ( source.LinkedPtrListPost<DTYPE>::next != 0L )
		LinkedPtrListPost<DTYPE>::next = new LinkedPtrListPost<DTYPE>( *source.LinkedPtrListPost<DTYPE>::next );
	else
		LinkedPtrListPost<DTYPE>::next = 0L;
	last = FindLast();
}
	  
template <class DTYPE>
LinkedPtrListPost<DTYPE> *LinkedPtrList<DTYPE>::FindLast() const
{
	if ( LinkedPtrListPost<DTYPE>::next == 0L )
		return (LinkedPtrListPost<DTYPE> *)this;
	LinkedPtrListPost<DTYPE> *ptr = LinkedPtrListPost<DTYPE>::next;
	while ( ptr->LinkedPtrListPost<DTYPE>::next != 0L )
		ptr = ptr->LinkedPtrListPost<DTYPE>::next;
	return ptr;
}

template <class DTYPE>
int LinkedPtrList<DTYPE>::Exist( int nr ) const
{
	if ( nr >= 0 && nr < size )
		return 1;
	return 0;
}

/*
template <class DTYPE>
int LinkedPtrList<DTYPE>::Exist( const DTYPE *d ) const
{
	if ( size > 0 )
		return LinkedPtrListPost<DTYPE>::next->Exist( d );
	return 0;
}
*/

template <class DTYPE>
int LinkedPtrList<DTYPE>::Index( const DTYPE it ) const
{
	LinkedPtrListPost<DTYPE> *ptr = LinkedPtrListPost<DTYPE>::next;
	for ( int i = 0 ; i < size ; i++ )
	{
		if ( LinkedPtrListPost<DTYPE>::next == 0L && i < size - 1 )
			_DBTHROW( (std::string)"Error:Internal size error" );
		if ( it == ptr->item )
			return i;
		ptr = ptr->LinkedPtrListPost<DTYPE>::next;			// If error - check this
	}
	_DBTHROW( (std::string)"Error:Item not found" );
	return -1;	//Visual bug ?	
}

template <class DTYPE>
int LinkedPtrList<DTYPE>::Index( const DTYPE i0 , int (*match_f)( DTYPE a , DTYPE b ) ) const
{
	LinkedPtrListPost<DTYPE> *ptr = LinkedPtrListPost<DTYPE>::next;
	for ( int i = 0 ; i < size ; i++ )
	{
		if ( LinkedPtrListPost<DTYPE>::next == 0L && i < size - 1 )
			_DBTHROW( (std::string)"Error:Internal size error" );
		if ( match_f( i0 , ptr->item ) )
			return i;
		ptr = ptr->LinkedPtrListPost<DTYPE>::next;			// If error - check this
	}
	//	_DBTHROW( (std::string)"Error:Item not found" );
	return -1;	//Visual bug ?	
 
}

template <class DTYPE>
DTYPE LinkedPtrList<DTYPE>::Get( int nr , bool acceptnoexistent ) const
{

	if( ! Exist( nr ) )
	  if ( acceptnoexistent )
	    return NULL;
	  else
	    _DBTHROW( (std::string)"Error:Index out of bounds" );

	LinkedPtrListPost<DTYPE> *ptr = LinkedPtrListPost<DTYPE>::next;
	for ( int i = 0 ; i < nr ; i++ )
	{
		if ( ptr == 0L )
			_DBTHROW( (std::string)"Error:Internal size error" );
		ptr = ptr->LinkedPtrListPost<DTYPE>::next;
	}
	return ptr->item;
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Replace( int nr , const DTYPE _new) 
{
  //  _DB( std::cout << "void LinkedPtrList<DTYPE>::Replace( int nr , const DTYPE _new)" << std::endl; );
  //  _DB( std::cout << "nr = " << nr  << std::endl; );

  if ( ! Exist( nr ) )
    _DBTHROW( (std::string)"Error:Index out of bounds" );
  LinkedPtrListPost<DTYPE> *ptr = LinkedPtrListPost<DTYPE>::next;
  for ( int i = 0 ; i < nr ; i++ )
    {
      ptr = ptr->LinkedPtrListPost<DTYPE>::next;
      if ( ptr == 0L )
	_DBTHROW( (std::string)"Error:Internal size error" );
    }
  ptr->Replace( _new );
  last = FindLast();
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Add( const LinkedPtrList<DTYPE> &l )
{
	if ( l.size == 0 )
		return;
	size += l.size;	
	last->LinkedPtrListPost<DTYPE>::next = new LinkedPtrListPost<DTYPE>( *l.LinkedPtrListPost<DTYPE>::next );
	last = FindLast();
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Add( const DTYPE _new) 
{
	LinkedPtrListPost<DTYPE> *tmp = new LinkedPtrListPost<DTYPE>( _new );
	last->LinkedPtrListPost<DTYPE>::next = tmp;
	last = tmp;
	size++;
	last = FindLast();
//	delete tmp;
}

/* 
   Unchecked change 040726 
*/

template <class DTYPE>
void LinkedPtrList<DTYPE>::Insert( int index , const DTYPE _new) 
{
  //  std::cout << "void LinkedPtrList<DTYPE>::Insert( int index , const DTYPE _new)" << std::endl;
	if ( ! Exist( index ) )
		_DBTHROW( (std::string)"Error:Index out of bounds" );
	// Start Change 030624
	//	LinkedPtrListPost<DTYPE> *ptr = this;
	//	LinkedPtrListPost<DTYPE> *ptr = LinkedPtrListPost<DTYPE>::next;
	// End Change 030624
	// Start Change 040726
	LinkedPtrListPost<DTYPE> *ptr = this; //point to first, nonsense link
	// End Change 040726
	for ( int i = 0 ; i < index ; i++ )
	{
	  if ( LinkedPtrListPost<DTYPE>::next == 0L )
	    _DBTHROW( (std::string)"Error:Internal size error" );
	  ptr = ptr->LinkedPtrListPost<DTYPE>::next;
	}
	LinkedPtrListPost<DTYPE> *tptr = new LinkedPtrListPost<DTYPE>( _new );
	tptr->LinkedPtrListPost<DTYPE>::next = ptr->LinkedPtrListPost<DTYPE>::next;
	ptr->LinkedPtrListPost<DTYPE>::next = tptr;
	size++;
	last = FindLast();
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Insert( const DTYPE i , const DTYPE _new) 
{
	int index = Index( i );
	Insert( index , _new );
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Remove( int index ) 
{
	if ( ! Exist( index ) )
	  _DBTHROW( (std::string)"void LinkedPtrList<DTYPE>::Remove( int index ) : ! Exist( index )" );

	LinkedPtrListPost<DTYPE> *ptr = this;


//	for ( int i = 0 ; i < index - 1 ; i++ )
	for ( int i = 0 ; i < index ; i++ )
	{
		if ( LinkedPtrListPost<DTYPE>::next == 0L )
			_DBTHROW( (std::string)"Error:Internal size error" );
		ptr = ptr->LinkedPtrListPost<DTYPE>::next;
	}
	LinkedPtrListPost<DTYPE> *tptr = ptr->LinkedPtrListPost<DTYPE>::next;
	ptr->LinkedPtrListPost<DTYPE>::next = ptr->LinkedPtrListPost<DTYPE>::next->LinkedPtrListPost<DTYPE>::next;
	
	tptr->LinkedPtrListPost<DTYPE>::next = 0L;
	delete tptr;
	size--;
	last = FindLast();
}

template <class DTYPE>
void LinkedPtrList<DTYPE>::Remove( const DTYPE i ) 
{
  int index = Index( i );
  Remove( index );
}

template <class DTYPE>
LinkedPtrList<DTYPE> LinkedPtrList<DTYPE>::operator=( const LinkedPtrList<DTYPE> &ll )
{
	Copy( ll );
	return *this;
}

template <class DTYPE>
int LinkedPtrList<DTYPE>::operator==( const LinkedPtrList<DTYPE> &ll ) const
{
	if ( size != ll.size )
		return 0;
	if ( LinkedPtrListPost<DTYPE>::next != 0L )
		return LinkedPtrListPost<DTYPE>::next->operator==( *ll.LinkedPtrListPost<DTYPE>::next );
	else
		return 1;
}

template <class DTYPE>
void HighLow( LinkedPtrList<DTYPE> &list)
{
	LinkedPtrListPost<DTYPE> *previous;
	LinkedPtrListPost<DTYPE> *current;
	LinkedPtrListPost<DTYPE> *coming;
	LinkedPtrListPost<DTYPE> *tmp;

	for ( int last = list.size - 2 ; last >= 0 ; last-- )
	{
		previous = &list;
		current = list.LinkedPtrListPost<DTYPE>::next;
		coming = current->LinkedPtrListPost<DTYPE>::next;
		for ( int i = 0 ; i <= last ; i++ )
		{
			if ( *current->item < *coming->item )
			{
				previous->LinkedPtrListPost<DTYPE>::next = coming;
				current->LinkedPtrListPost<DTYPE>::next = coming->LinkedPtrListPost<DTYPE>::next;
				coming->LinkedPtrListPost<DTYPE>::next = current;
				tmp = current;
				current = coming;
				coming = coming->LinkedPtrListPost<DTYPE>::next;
			}
			previous = current;
			current = coming;
			coming = coming->LinkedPtrListPost<DTYPE>::next;
		}
	}
	list.last = list.FindLast();
}

template <class DTYPE>
void LowHigh( LinkedPtrList<DTYPE> &list)
{

}

template <class DTYPE>
void Destruct( LinkedPtrList<DTYPE> &list )
{
	for ( int i = 0 ; i < list.size ; i++ )
		delete ( list.Get( i ) );
}

template <class DTYPE>
void Destruct( LinkedPtrList<DTYPE> &list , int nr )
{
	if ( ! list.Exist( nr ) )
		_DBTHROW( (std::string)"Error:Index out of bounds" );
	delete( list.Get( nr ) );
}

template class P4th::LinkedPtrList<float *>;
template class P4th::LinkedPtrList<double *>;

}




