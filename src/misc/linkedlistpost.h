/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef LNKD_LIST_POST_
#define LNKD_LIST_POST_

namespace P4th
{

template <class DTYPE>
class LinkedListPost 
{
protected:
public:
/* Dessa skall vara protected men jag lyckas inte kompilera i s� fall */
	DTYPE item;									// Element
	LinkedListPost *next;						// Pekare till n�sta l�nk

	LinkedListPost();							// Skapa tom(?) post
	LinkedListPost( const DTYPE &source );		// Skapa och initiera
	LinkedListPost( const LinkedListPost<DTYPE> &source );
												// Copy Constructor - kopierar
												// SAMTLIGA f�ljande l�nkar !
	virtual ~LinkedListPost();
	void Copy( const LinkedListPost<DTYPE> &source );
												// Kopiera denna och SAMTLIGA
												// f�ljande l�nkar fr�n source
	int operator==( const DTYPE &source )		// �r elementets v�rde = source
		{ return source == item; }
	int Exist( const DTYPE *b ) const;			// Finns ett element med v�rdet
	int Exist( const DTYPE &b ) const;			// Finns ett element med v�rdet
												// b i denna eller n�gon av de
												// n�stkomamnde elementen
	DTYPE Get( DTYPE *it ) const;			// H�mta it om det finns (varf�r?)
	DTYPE &Ref( DTYPE * );				// Referera till ett element 
												// om det finns
	void Replace( const DTYPE &_new )			// Ers�tt detta elemnt med _new
		{ item = _new; };
	void Replace( const DTYPE &old , const DTYPE &_new );
												// Ers�tt det f�rsta elementet
												// i denna eller n�stkommande
												// l�nkar som har v�rdet old
												// med v�rdet _new
	LinkedListPost<DTYPE> operator=( const LinkedListPost<DTYPE> &llp );
												// �verlagra = (g�ller dennna
												// och samtliga n�stkommande
												// l�nkar)
	int operator==( const LinkedListPost<DTYPE> &ll ) const;
												// �verlagra == (g�ller denna 
												// och samtliga n�stkomamnde 
												// l�nkar)
public:

};

}
#endif 

