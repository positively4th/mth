/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _LINKEDLIST_H_H
#define _LINKEDLIST_H_H

#include "debug.h"
#include "linkedlistpost.h"

namespace P4th
{

  template <class DTYPE>
  class LinkedList;

  //#ifndef _WIN32
  // Rquires operator < and operator > to be defined for item class
  template <class DTYPE>
  void HighLow( LinkedList<DTYPE> &list );
  template <class DTYPE>
  void LowHigh( LinkedList<DTYPE> &list );
  template <class DTYPE>
  int Index( LinkedList<DTYPE> &list , const DTYPE &m);
  //#endif


  ///
  /// \brief Generic LinkedList
  ///
  /// A straightforward implementation of a onw way linked list.
  /// Any type used in the list must provide the == and != operators.   
  ///
  template <class DTYPE>
  class LinkedList : public LinkedListPost<DTYPE>
  {
  protected:
    unsigned int size; /// Antal element i lista, obs ej samma som antalet l�nkar
    LinkedListPost<DTYPE> *last;  /// Pekare till sista l�nken
    /// Returnerar sista l�nken
    LinkedListPost<DTYPE> *FindLast() const; 
    /// Kopierar listan till source
    void Copy( const LinkedList &source );	
  public:
    LinkedList();					
    /// Skapar en list med tv� l�nkar, d v s ett element
    LinkedList( const DTYPE &item );						
    /// Skapar en kopia av source
    LinkedList( const LinkedList &source );	
    virtual ~LinkedList();
    /// Deallokerar alla l�nkar utom listhuvudet	
    void Delete();				
    /// Antal element (dvs l�nkar - 1)
    int GetSize() const { return size; };	
    int SafeExist( const unsigned int nr ) const;				// �r nr ok
    int Exist( const unsigned int nr ) const				// �r nr ok
    { return SafeExist( nr ); }
    int Exist( const DTYPE *d ) const;
    int Exist( const DTYPE &d ) const;
    /// index f�r ett objekt i
    int Index( DTYPE *i ) const;		
    /// Returnera element nr
    DTYPE Get( unsigned int nr ) const;				
    /// Referera till element nr
    DTYPE &Ref( const unsigned int nr ) const;					
    /// Ers�tt element nr med _new
    void Replace( int nr , const DTYPE &_new );
    /// L�gg till hela listan l sist
    void Add( const LinkedList<DTYPE> &l );	
    /// L�gg till elementet _new sist
    void Add( const DTYPE &_new );			
    /// Infoga _new p� i's plats
    void Insert( DTYPE *i , const DTYPE &_new );
    /// Depreciated 
    void Insert( int nr , const DTYPE &_new );
    /// Infoga _new p� plats nr
    void InsertAt( int nr , const DTYPE &_new );
    /// Ta bort element i fr�n listan
    void Remove( DTYPE *i );			 
    /// Ta bort elementet med index nr
    void Remove( int nr );					
    /// Ta bort alla element
    void RemoveAll() { Delete(); }			 
    /// Referera till element nr
    DTYPE &operator[]( int nr ) const				
    { return Ref( nr ); }
    /// Assignment operator
    LinkedList<DTYPE> operator=( const LinkedList<DTYPE> &ll );
    ///  Not eqaul operator
    int operator!=( const LinkedList<DTYPE> &ll ) const
    { return ! ( *this == ll ); }			
    // Equality operator
    int operator==( const LinkedList<DTYPE> &ll ) const;
#ifndef _WIN32
    /// Sort >,>,> (windows)
    friend void HighLow<>( LinkedList<DTYPE> &list );
    /// Sortera <,<,< (windows)
    friend void LowHigh<>( LinkedList<DTYPE> &list );
#endif
#ifdef _WIN32
    /// Sortt >,>,>
    friend void HighLow( LinkedList<DTYPE> &list );
    /// Sort <,<,<
    friend void LowHigh( LinkedList<DTYPE> &list );
#endif

  };

}

#endif

