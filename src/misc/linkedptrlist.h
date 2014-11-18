/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef LINKEDPTRLIST_H_
#define LINKEDPTRLIST_H_

#include "linkedptrlistpost.h"

namespace P4th
{

template <class DTYPE>
  class LinkedPtrList;

// Rquires operator < and operator > to be defined for item class
template <class DTYPE>
void HighLow( LinkedPtrList<DTYPE> &list );
template <class DTYPE>
void LowHigh( LinkedPtrList<DTYPE> &list );

template <class DTYPE>
void Destruct(LinkedPtrList<DTYPE> &list );				
// Call the destructor for all posts
template <class DTYPE>
void Destruct( LinkedPtrList<DTYPE> &list , int nr );
// Call the destructor for post nr


template <class DTYPE>
class LinkedPtrList : public LinkedPtrListPost<DTYPE>
{
protected:
	int size;									
	// Antal element i lista, obs ej
	LinkedPtrListPost<DTYPE> *last;
	// Pekare till sista l�nken
	LinkedPtrListPost<DTYPE> *FindLast() const;
	// Returnerar sista l�nken
	void Copy( const LinkedPtrList &source );	
	// Kopierar listan till source
public:
	LinkedPtrList();							
	LinkedPtrList( DTYPE item );				
	// Skapar en list med tv� l�nkar d v s ett element
	LinkedPtrList( const LinkedPtrList &source );	
	// Skapar en kopia av source
	virtual ~LinkedPtrList();	
	void Delete();	
	// Deallokerar alla l�nkar utom listhuvudet
	virtual void Free();	
	// Deallokerar alla l�nkar samt pekaren utom listhuvudet

	// F�ljande rutiner finns inte i basklassen
	int GetSize() const { return size; };
	// Antal element (dvs l�nkar - 1)
	int Exist( int nr ) const;				
	// finns ett objekt i
	int Index( const DTYPE i ) const;			
	// index f�r ett objekt i
	int Index( const DTYPE i0 , int (*match_f)( DTYPE a , DTYPE b ) ) const;
	// Godtycklig matcning genom funktionen match_f, -1 om ingen l�nk matchar!
	DTYPE Get( int nr , bool acceptnonexistent = false ) const;				
	// Returnera element nr
	void Replace( int nr , DTYPE _new );
	// Ers�tt element nr med _new
	void Add( const LinkedPtrList<DTYPE> &l );
	// L�gg till hela listan l sist
	void Add( const DTYPE _new );
	// L�gg till elementet _new sist
	void Insert( const DTYPE i , const DTYPE _new );
	// Infoga _new p� i's plats 
	void Insert( int nr , const DTYPE _new );
	// Infoga _new p� plats nr
	void Remove( const DTYPE i );
	// Ta bort element i fr�n listan 
	void Remove( int nr );
	// Ta bort elementet med nr nr
	void RemoveAll() { Delete(); }
	// Ta bort alla element 
	DTYPE operator[]( int nr ) 
		{ return Get( nr ); }
	// Referera till element nr
	LinkedPtrList<DTYPE> operator=( const LinkedPtrList<DTYPE> &ll );
	// �verlagra =
	int operator!=( const LinkedPtrList<DTYPE> &ll ) const
	{ return ! ( *this == ll ); }			// �verlagra !=
	int operator==( const LinkedPtrList<DTYPE> &ll ) const;
	// �verlagra ==
#ifdef _WIN32
	friend void HighLow( LinkedPtrList<DTYPE> &list );
	// Sortera >,>,>
	friend void LowHigh( LinkedPtrList<DTYPE> &list );
	// Sortera <,<,<
	friend void Destruct(LinkedPtrList<DTYPE> &list );				
	// Call the destructor for all posts
	friend void Destruct( LinkedPtrList<DTYPE> &list , int nr );
	// Call the destructor for post nr
#else
	friend void HighLow<DTYPE>( LinkedPtrList<DTYPE> &list );
//	friend void HighLow<DTYPE>( LinkedPtrList<DTYPE> &list );
	// Sortera >,>,>
	friend void LowHigh<DTYPE>( LinkedPtrList<DTYPE> &list );
	// Sortera <,<,<
	friend void Destruct<DTYPE>(LinkedPtrList<DTYPE> &list );				
	// Call the destructor for all posts
	friend void Destruct<DTYPE>( LinkedPtrList<DTYPE> &list , int nr );
	// Call the destructor for post nr
#endif

};




}

#endif // !defined(AFX_LinkedPtrList_H__39DF62E1_74FF_11D3_865D_184D04C10000__INCLUDED_)
