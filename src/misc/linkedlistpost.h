/*
	Copyright 2014 Klas SandÃ©n.
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
/* Dessa skall vara protected men jag lyckas inte kompilera i så fall */
	DTYPE item;									// Element
	LinkedListPost *next;						// Pekare till nästa länk

	LinkedListPost();							// Skapa tom(?) post
	LinkedListPost( const DTYPE &source );		// Skapa och initiera
	LinkedListPost( const LinkedListPost<DTYPE> &source );
												// Copy Constructor - kopierar
												// SAMTLIGA följande länkar !
	virtual ~LinkedListPost();
	void Copy( const LinkedListPost<DTYPE> &source );
												// Kopiera denna och SAMTLIGA
												// följande länkar från source
	int operator==( const DTYPE &source )		// Är elementets värde = source
		{ return source == item; }
	int Exist( const DTYPE *b ) const;			// Finns ett element med värdet
	int Exist( const DTYPE &b ) const;			// Finns ett element med värdet
												// b i denna eller någon av de
												// nästkomamnde elementen
	DTYPE Get( DTYPE *it ) const;			// Hämta it om det finns (varför?)
	DTYPE &Ref( DTYPE * );				// Referera till ett element 
												// om det finns
	void Replace( const DTYPE &_new )			// Ersätt detta elemnt med _new
		{ item = _new; };
	void Replace( const DTYPE &old , const DTYPE &_new );
												// Ersätt det första elementet
												// i denna eller nästkommande
												// länkar som har värdet old
												// med värdet _new
	LinkedListPost<DTYPE> operator=( const LinkedListPost<DTYPE> &llp );
												// Överlagra = (gäller dennna
												// och samtliga nästkommande
												// länkar)
	int operator==( const LinkedListPost<DTYPE> &ll ) const;
												// Överlagra == (gäller denna 
												// och samtliga nästkomamnde 
												// länkar)
public:

};

}
#endif 

