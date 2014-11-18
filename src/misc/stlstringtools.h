/*
	Copyright 2014 Klas SandÃ©n.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _STLSTRINGTOOLS_H_
#define _STLSTRINGTOOLS_H_

#include <string>
#include <vector>

using namespace std;

namespace P4th
{


///
/// \brief String manipulation class
///
/// The class provides convenient string operation.
/// Som member functions uses the static member tmpbuf,
/// any such function is not thread safe. 
///

  ostream &operator<<( ostream &dest, const string &str );
  class STLStringTools  {
    typedef vector<string> _strVec;
    typedef vector<string>::const_iterator cstrVecee;
    typedef vector<string>::iterator _strVecee;
  public:

    static string &Append( string &dest , const string &item , const string &sep , bool infront = false );
    static string AsString( double number ); 
    static string AsString( float number ); 
    static string AsString( int number ); 
    static string AsString( unsigned int number ); 
    static string AsString( double vv , int prec , char decchar );
    static string Blank( int ant )		
    { return Fill( ant , ' ' ); }
    static bool Contains( const string &data , const string &items );

    static int Count( const string &data , char item );
    static string Fill( size_t ant , char t )
    { return string( ant , t ); }	
    static size_t GetLength( const string &str  )
    { return str.size(); }	
    static string GetNth( const string &str , int nr , const string &separator );
    static string GetNth( const string &str , int nr , char separator )
    { return GetNth( str , nr , string( &separator , 1 ) ); } 
    static size_t Index( const string &str , const string &pat , size_t start = 0 )
    { return str.find( pat , start ); }

    static string LeftOfExc( const string &str , size_t index );
    static bool IsNumber( const string &str , char separator = '.' );
    static string RightOfInc( const string &str , size_t index );
    static string LeftOfExc( const string &str , const string &pat );
    static string RightOfInc( const string &str , const string &pat );


    static int String2Int( const string &str );
    static double String2Double( const string &str );
    static string String2Lower( const string &str );
    static string String2Upper( const string &str );
    static string Strip( const string &str , const string &skips , int upto = - 1 );
    static vector<string> Split( const string &str , const string &glue );
    static string Join(const _strVec &strVec, const string &glue );

    /*
    /// Temporär Bufferstorlek
    static const int _TMPBUFSIZE;
    /// Temporär teckenbuffert
    static char tmpbuf[];				
  public:
    StringTools();		
    virtual ~StringTools();
    /// Heltal till sträng
    static StrType Int2String( const long int v );	
    /// Heltal till sträng
    static StrType Int2String( const unsigned int v );	
    /// Heltal till sträng
    static StrType Int2String( const long unsigned int v );	
    /// Heltal till sträng
    static StrType Int2String( const int v );	
    /// Heltal till sträng
    static StrType UnsignedInt2String( const unsigned int v );	
    /// Convert a double to a string
    static StrType AsString( double v , int prec , char decchar = ',' );
    /// Depreciatred - use AsString
    static StrType Double2String( const double v , int prec , char decchar = ',' );
    static long int String2LongInt( const StrType &str );
    static int String2Int( const StrType &str );
    /// Sträng till heltal
    static unsigned int String2UnsignedInt( const StrType &str );
    /// Sträng till lower case
    static StrType String2Lower( const StrType &str );
    /// Sträng till upper case
    static StrType String2Upper( const StrType &str );
    /// Sträng till flyttal
    static double String2Double( const StrType &str );
    /// Skapa en ant tecken lång sträng med bara mellanslag
    static StrType Blank( int ant )		
    { return StrType( ' ' , ant ); }
    // Inehåller strängen data tecknet item
    static int Contains( const StrType &data , char item );
    // Inehåller strängen data någpt av tecknen i items
    static int Contains( const StrType &data , const StrType &items );
    static StrType Remove( const StrType &data , char item );
    /// Returnera en ant lång sträng med bara tecken av typen t 
    { return StrType( t , ant ); }	
    /// Lägg till tecken från vänster 
    static StrType PadLeft( StrType in , int tosize , char t = ' ' );
    /// Lägg till tecken från höger 
    static StrType PadRight( StrType in , int tosize , char t = ' ' );
    /// Depreciated - use PadRight
    static StrType Pad( StrType in , int tosize , char t = ' ' )
    { return PadRight( in , tosize , t ); }	
    static StrType PadNumberLeft( StrType in , int tosize , char t );
    /// Kopiera en delsträng
    static StrType Copy( const StrType &str , int at , int size ) 
    { if ( size <= 0 ) return ""; return str.Right( str.GetLength() - at ).Left( size ); }
    /// Bryt strän radvänligt
    static StrType WordWrap( StrType &in , int maxsize , char t = ' ' ); 
    /// Vänsterjusterar till fix längd
    static StrType AlignLeft( StrType in , int tosize )
    { return Pad( in , tosize ); }	
    /// Högerjusterar till fix längd tosize och fyller ut från vänster med tecknet t
    static StrType AlignRight( StrType in , int tosize , char t = ' ' );
    /// Plocakr ut ord nr wnr ur str
    static StrType GetWord( StrType str , int wnr );
    /// Fixar eventuellt decimal format fel k innehåller decimaltecknet (, eller .)
    static StrType DecFix( StrType str , char k = ',' );
    static int IndexOr( const StrType &str , const StrType &pat , int start = 0 );
    static int IndexAnd( const StrType &str , const StrType &pat , int start = 0)
    { return Index( str , pat , start  ); }
    /// Index for pattern
    static int Index( const StrType &str , const StrType &pat , int start = 0 );
    /// Index for pattern from right
    static int IndexRight( const StrType &str , const StrType &pat );
    /// Returnera all till vänster om position index
    /// int->StrType
    static StrType String( const int tal )
    { return Int2String( tal ); }		
    /// Current date
    static StrType YYYYMMDD();			
    /// Current time
    static StrType HHMMSS();			
    static StrType Duration( float seconds );
    /// Current time
    static StrType CharNr2ByteNr( const StrType &AlphaN );		
    /// Replace all occurences of skips until length of res is greater then upto
    static StrType Strip( const StrType &str , const StrType &skips , int upto = -1 );
    static StrType Replace( const StrType &str , char from , char to );
    static StrType Replace( const StrType &str , const StrType &from , 
			    const StrType &to ,  const StrType &separators );
    static StrType GetParameter( const StrType &str , const StrType &start ,
				 const StrType &end );
    static StrType &Append( StrType &dest , const StrType &item , const StrType &sep , bool infront = false );

    /// Get the nth(zero based) element, separated by separator
    static StrType GetNth( const StrType &str , int nr , const StrType &separator );
    static bool IsNumber( const StrType &str , char separator = '.' );
    static bool IsLetter( const char c );
    static bool IsNumber( const char c );

    */

  };
 
}
#endif
