/*
	Copyright 2014 Klas SandÃ©n.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
// LinkePtrListPost.h: interface for the LinkePtrListPost class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _LINKEPTRLISTPOST_H_
#define _LINKEPTRLISTPOST_H_

namespace P4th
{

template <class DTYPE>
class LinkedPtrListPost 
{
  //protected:
 public:
/* Dessa skall vara protected men jag lyckas inte kompilera i så fall */
	DTYPE item;									
	// Element (pekare)
	LinkedPtrListPost *next;
	// Pekare till nästa länk
	LinkedPtrListPost();
	// Skapa tom(?) post
	LinkedPtrListPost( DTYPE const source );
	// Skapa och initiera
	LinkedPtrListPost( const LinkedPtrListPost<DTYPE> &source );
	// Copy Constructor - kopierar SAMTLIGA följande länkar !
	virtual ~LinkedPtrListPost();
	virtual void Free();	
	// Deallokerar alla länkar samt pekaren utom listhuvudet
	void Copy( const LinkedPtrListPost<DTYPE> &source );
	// Kopiera denna och SAMTLIGA följande länkar från source
	int operator==( const DTYPE source )
		{ return source == item; }
	// Är elementets värde = source
	//	int operator==( DTYPE source )
	//		{ return source == item; }
	// Är elementets pekare = source
	//	int Exist( const DTYPE &b ) const;
	// Finns ett element med värdet b i denna eller någon av de nästkomamnde elementen
	int Exist( const DTYPE b ) const;
	// Finns ett element med värdet b i denna eller någon av de nästkomamnde elementen
	DTYPE Get( const DTYPE ) const;
	// Referera till ett element om det finns 
	void Replace( const DTYPE _new )
		{ item = _new; };
	// Ersätt detta elemnt med _new
	//	void Replace( const DTYPE old , DTYPE _new );
	// Ersätt det första elementet i denna eller nästkommande länkar som har värdet old med värdet _new
	void Replace( const DTYPE old , const DTYPE _new );
	// Ersätt det första elementet i denna eller nästkommande länkar som har värdet old med värdet _new
	LinkedPtrListPost<DTYPE> operator=( const LinkedPtrListPost<DTYPE> &llp );
	// Överlagra = (gäller dennna och samtliga nästkommande länkar)
	int operator==( const LinkedPtrListPost<DTYPE> &ll ) const;
	// Överlagra == (gäller denna och samtliga nästkomamnde länkar)
public:

};

}

#endif // !defined(AFX_LINKELISTPOST_H__39DF62E7_74FF_11D3_865D_184D04C10000__INCLUDED_)
