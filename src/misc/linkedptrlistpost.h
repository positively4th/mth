/*
	Copyright 2014 Klas Sandén.
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
/* Dessa skall vara protected men jag lyckas inte kompilera i s� fall */
	DTYPE item;									
	// Element (pekare)
	LinkedPtrListPost *next;
	// Pekare till n�sta l�nk
	LinkedPtrListPost();
	// Skapa tom(?) post
	LinkedPtrListPost( DTYPE const source );
	// Skapa och initiera
	LinkedPtrListPost( const LinkedPtrListPost<DTYPE> &source );
	// Copy Constructor - kopierar SAMTLIGA f�ljande l�nkar !
	virtual ~LinkedPtrListPost();
	virtual void Free();	
	// Deallokerar alla l�nkar samt pekaren utom listhuvudet
	void Copy( const LinkedPtrListPost<DTYPE> &source );
	// Kopiera denna och SAMTLIGA f�ljande l�nkar fr�n source
	int operator==( const DTYPE source )
		{ return source == item; }
	// �r elementets v�rde = source
	//	int operator==( DTYPE source )
	//		{ return source == item; }
	// �r elementets pekare = source
	//	int Exist( const DTYPE &b ) const;
	// Finns ett element med v�rdet b i denna eller n�gon av de n�stkomamnde elementen
	int Exist( const DTYPE b ) const;
	// Finns ett element med v�rdet b i denna eller n�gon av de n�stkomamnde elementen
	DTYPE Get( const DTYPE ) const;
	// Referera till ett element om det finns 
	void Replace( const DTYPE _new )
		{ item = _new; };
	// Ers�tt detta elemnt med _new
	//	void Replace( const DTYPE old , DTYPE _new );
	// Ers�tt det f�rsta elementet i denna eller n�stkommande l�nkar som har v�rdet old med v�rdet _new
	void Replace( const DTYPE old , const DTYPE _new );
	// Ers�tt det f�rsta elementet i denna eller n�stkommande l�nkar som har v�rdet old med v�rdet _new
	LinkedPtrListPost<DTYPE> operator=( const LinkedPtrListPost<DTYPE> &llp );
	// �verlagra = (g�ller dennna och samtliga n�stkommande l�nkar)
	int operator==( const LinkedPtrListPost<DTYPE> &ll ) const;
	// �verlagra == (g�ller denna och samtliga n�stkomamnde l�nkar)
public:

};

}

#endif // !defined(AFX_LINKELISTPOST_H__39DF62E7_74FF_11D3_865D_184D04C10000__INCLUDED_)
