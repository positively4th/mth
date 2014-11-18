/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tSTLStringTools
#include <boost/test/unit_test.hpp>
#include <math.h>

#include "stlstringtools.h"

using namespace P4th;

bool IsEqual( double d1 ,double d2 , double tolerance ) 
{
  if ( tolerance > 0 )
    return sqrt( ( d1 - d2 ) * ( d1 - d2 ) ) < tolerance; 
  return sqrt( ( d1 - d2 ) * ( d1 - d2 ) ) < tolerance * sqrt( d1 * d2 ); 

}

BOOST_AUTO_TEST_CASE( Append )
{
  string res;
  
  res = "";
  BOOST_CHECK_EQUAL( "" , STLStringTools::Append( res , "" , "." , false )  ); 
  BOOST_CHECK_EQUAL( "a" , STLStringTools::Append( res , "a" , "." , false )  ); 
  BOOST_CHECK_EQUAL( "a.b" , STLStringTools::Append( res , "b" , "." , false )  ); 
  BOOST_CHECK_EQUAL( "a.b,c" , STLStringTools::Append( res , "c" , "," , false )  ); 

  res = "";
  BOOST_CHECK_EQUAL( "" , STLStringTools::Append( res , "" , "." , true )  ); 
  BOOST_CHECK_EQUAL( "a" , STLStringTools::Append( res , "a" , "." , true )  ); 
  BOOST_CHECK_EQUAL( "b.a" , STLStringTools::Append( res , "b" , "." , true )  ); 
  BOOST_CHECK_EQUAL( "c,b.a" , STLStringTools::Append( res , "c" , "," , true )  ); 
}

BOOST_AUTO_TEST_CASE( AsString )
{
  BOOST_CHECK_EQUAL( "0" , STLStringTools::AsString( (double)0.0 ) ); 
  BOOST_CHECK_EQUAL( "2" , STLStringTools::AsString( (double)2.0 ) ); 
  BOOST_CHECK_EQUAL( "-2" , STLStringTools::AsString( (double)-2.0 ) ); 
  BOOST_CHECK_EQUAL( "2.23" , STLStringTools::AsString( (double)2.23 ) ); 
  BOOST_CHECK_EQUAL( "-2.23" , STLStringTools::AsString( (double)-2.23 ) ); 

  BOOST_CHECK_EQUAL( "0" , STLStringTools::AsString( (float)0.0 ) ); 
  BOOST_CHECK_EQUAL( "2" , STLStringTools::AsString( (float)2.0 ) ); 
  BOOST_CHECK_EQUAL( "-2" , STLStringTools::AsString( (float)-2.0 ) ); 
  BOOST_CHECK_EQUAL( "2.23" , STLStringTools::AsString( (float)2.23 ) ); 
  BOOST_CHECK_EQUAL( "-2.23" , STLStringTools::AsString( (float)-2.23 ) ); 

  BOOST_CHECK_EQUAL( "0" , STLStringTools::AsString( (int)0 ) ); 
  BOOST_CHECK_EQUAL( "2" , STLStringTools::AsString( (int)2 ) ); 
  BOOST_CHECK_EQUAL( "-2" , STLStringTools::AsString( (int)-2 ) ); 

  BOOST_CHECK_EQUAL( "0" , STLStringTools::AsString( (unsigned int)0 ) ); 
  BOOST_CHECK_EQUAL( "2" , STLStringTools::AsString( (unsigned int)2 ) ); 

  BOOST_CHECK_EQUAL( "1.2" , STLStringTools::AsString( (double)1.234 , 1 , '.'  ) ); 
  BOOST_CHECK_EQUAL( "-1.2" , STLStringTools::AsString( (double)-1.234 , 1 , '.'  ) ); 
  BOOST_CHECK_EQUAL( "1.3" , STLStringTools::AsString( (double)1.264 , 1 , '.'  ) ); 
  BOOST_CHECK_EQUAL( "-1.3" , STLStringTools::AsString( (double)-1.264 , 1 , '.'  ) ); 
  BOOST_CHECK_EQUAL( "10;2345" , STLStringTools::AsString( (double)10.23451 , 4 , ';'  ) ); 
  BOOST_CHECK_EQUAL( "-10;2345" , STLStringTools::AsString( (double)-10.23451 , 4 , ';'  ) ); 
}

BOOST_AUTO_TEST_CASE( Blank )
{
  BOOST_CHECK_EQUAL( "" , STLStringTools::Blank( 0 ) ); 
  BOOST_CHECK_EQUAL( " " , STLStringTools::Blank( 1 ) ); 
  BOOST_CHECK_EQUAL( "  " , STLStringTools::Blank( 2 ) ); 
}

BOOST_AUTO_TEST_CASE( Contains )
{
  BOOST_CHECK_EQUAL( false , STLStringTools::Contains( "12345" , "" ) ); 
  BOOST_CHECK_EQUAL( false , STLStringTools::Contains( "12345" , "0" ) ); 
  BOOST_CHECK_EQUAL( true , STLStringTools::Contains( "12345" , "1" ) ); 
  BOOST_CHECK_EQUAL( true , STLStringTools::Contains( "12345" , "56" ) ); 
}

BOOST_AUTO_TEST_CASE( Count )
{
  BOOST_CHECK_EQUAL( 0 , STLStringTools::Count( "123123123" , '4' ) ); 
  BOOST_CHECK_EQUAL( 1 , STLStringTools::Count( "1121231234" , '4' ) ); 
  BOOST_CHECK_EQUAL( 2 , STLStringTools::Count( "1121231234" , '3' ) ); 
  BOOST_CHECK_EQUAL( 3 , STLStringTools::Count( "1121231234" , '2' ) ); 
  BOOST_CHECK_EQUAL( 0 , STLStringTools::Count( "" , '2' ) ); 
}

BOOST_AUTO_TEST_CASE( Fill )
{
  BOOST_CHECK_EQUAL( "" , STLStringTools::Fill( 0 , 'a' ) ); 
  BOOST_CHECK_EQUAL( "b" , STLStringTools::Fill( 1 , 'b' ) ); 
  BOOST_CHECK_EQUAL( "cc" , STLStringTools::Fill( 2 , 'c' ) ); 
}

BOOST_AUTO_TEST_CASE( Index)
{
  BOOST_CHECK_EQUAL( string::npos , STLStringTools::Index( "ABCDEFG" , "X" ) ); 
  BOOST_CHECK_EQUAL( 1 , STLStringTools::Index( "ABCDE" , "BC" ) ); 
  BOOST_CHECK_EQUAL( 2 , STLStringTools::Index( "ABABCD" , "AB" , 1 ) ); 
}

BOOST_AUTO_TEST_CASE( GetLength )
{
  BOOST_CHECK_EQUAL( 0 , STLStringTools::GetLength( "" ) ); 
  BOOST_CHECK_EQUAL( 3 , STLStringTools::GetLength( "ABC" ) ); 

  BOOST_CHECK_EQUAL( ((string)"ABC").length() , STLStringTools::GetLength( "ABC" ) ); 
}

BOOST_AUTO_TEST_CASE( GetNth )
{
  BOOST_CHECK_EQUAL( "a" , STLStringTools::GetNth( "a,b,c" , 0 , "," ) ); 
  BOOST_CHECK_EQUAL( "b" , STLStringTools::GetNth( "a,b,c" , 1 , "," ) ); 
  BOOST_CHECK_EQUAL( "c" , STLStringTools::GetNth( "a,b,c" , 2 , "," ) ); 

  BOOST_CHECK_EQUAL( "a" , STLStringTools::GetNth( "a,,b,,c" , 0 , ",," ) ); 
  BOOST_CHECK_EQUAL( "b" , STLStringTools::GetNth( "a,,b,,c" , 1 , ",," ) ); 
  BOOST_CHECK_EQUAL( "c" , STLStringTools::GetNth( "a,,b,,c" , 2 , ",," ) ); 

  BOOST_CHECK_EQUAL( "aa" , STLStringTools::GetNth( "aa,,,bb,,,cc" , 0 , ",,," ) ); 
  BOOST_CHECK_EQUAL( "bb" , STLStringTools::GetNth( "aa,,,bb,,,cc" , 1 , ",,," ) ); 
  BOOST_CHECK_EQUAL( "cc" , STLStringTools::GetNth( "aa,,,bb,,,cc" , 2 , ",,," ) ); 

  BOOST_CHECK_EQUAL( "aa" , STLStringTools::GetNth( "aa|bb|cc" , 0 , '|' ) ); 
  BOOST_CHECK_EQUAL( "bb" , STLStringTools::GetNth( "aa|bb|cc" , 1 , '|' ) ); 
  BOOST_CHECK_EQUAL( "cc" , STLStringTools::GetNth( "aa|bb|cc" , 2 , '|' ) ); 
}

BOOST_AUTO_TEST_CASE( LeftOfExc )
{
  BOOST_CHECK_EQUAL( "LEFT" , STLStringTools::LeftOfExc( "LEFTCRIGHT" , 4 ) ); 
  BOOST_CHECK_EQUAL( "" , STLStringTools::LeftOfExc( "" , 2 ) ); 

  BOOST_CHECK_EQUAL( "LEFT" , STLStringTools::LeftOfExc( "LEFTCRIGHT" , "C" ) ); 
  BOOST_CHECK_EQUAL( "LEFT" , STLStringTools::LeftOfExc( "LEFTCENTERRIGHT" , "CENTER" ) ); 
  BOOST_CHECK_EQUAL( "" , STLStringTools::LeftOfExc( "LEFTCENTERRIGHT" , "XXX" ) ); 

}

BOOST_AUTO_TEST_CASE( IsNumber )
{
  BOOST_CHECK_EQUAL( true , STLStringTools::IsNumber( "1343" , '.' ) ); 
  BOOST_CHECK_EQUAL( true , STLStringTools::IsNumber( "-1343" , '.' ) ); 
  BOOST_CHECK_EQUAL( true , STLStringTools::IsNumber( "-13.43" , '.' ) ); 

  BOOST_CHECK_EQUAL( false , STLStringTools::IsNumber( "--13.43" , '.' ) ); 
  BOOST_CHECK_EQUAL( false , STLStringTools::IsNumber( "-13..43" , '.' ) ); 

  BOOST_CHECK_EQUAL( false , STLStringTools::IsNumber( "-13.43" , '|' ) ); 
}

BOOST_AUTO_TEST_CASE( String2Int )
{
  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Int( "0" ) ); 
  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Int( "-0" ) ); 

  BOOST_CHECK_EQUAL( 5 , STLStringTools::String2Int( "5" ) ); 
  BOOST_CHECK_EQUAL( -5 , STLStringTools::String2Int( "-5" ) ); 

  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Int( "0.1" ) ); 
  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Int( "0,1" ) ); 

  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Int( "-0.1" ) ); 
  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Int( "-0,1" ) ); 
}

BOOST_AUTO_TEST_CASE( String2Double )
{
  BOOST_CHECK_EQUAL( 0.0 , STLStringTools::String2Double( "0" ) ); 
  BOOST_CHECK_EQUAL( 0.0 , STLStringTools::String2Double( "-0" ) ); 

  BOOST_CHECK_EQUAL( 0.1 , STLStringTools::String2Double( "0.1" ) ); 
  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Double( "0,1" ) ); 

  BOOST_CHECK_EQUAL( -0.1 , STLStringTools::String2Double( "-0.1" ) ); 
  BOOST_CHECK_EQUAL( 0 , STLStringTools::String2Double( "-0,1" ) ); 
}

BOOST_AUTO_TEST_CASE( RightOfInc )
{
  BOOST_CHECK_EQUAL( "CRIGHT" , STLStringTools::RightOfInc( "LEFTCRIGHT" , 4 ) ); 
  BOOST_CHECK_EQUAL( "" , STLStringTools::RightOfInc( "" , 2 ) ); 

  BOOST_CHECK_EQUAL( "CRIGHT" , STLStringTools::RightOfInc( "LEFTCRIGHT" , "C" ) ); 
  BOOST_CHECK_EQUAL( "CENTERRIGHT" , STLStringTools::RightOfInc( "LEFTCENTERRIGHT" , "CENTER" ) ); 
  BOOST_CHECK_EQUAL( "" , STLStringTools::RightOfInc( "LEFTCENTERRIGHT" , "XXX" ) ); 
  
}

BOOST_AUTO_TEST_CASE( StringToUpper )
{
  BOOST_CHECK_EQUAL( "" , STLStringTools::String2Upper( ""  ) ); 
  BOOST_CHECK_EQUAL( "*&%#01234" , STLStringTools::String2Upper( "*&%#01234"  ) ); 
  BOOST_CHECK_EQUAL( "ABC...ÅÄÖ" , STLStringTools::String2Upper( "abc...åäö"  ) ); 
}

BOOST_AUTO_TEST_CASE( StringToLower )
{
  BOOST_CHECK_EQUAL( "" , STLStringTools::String2Lower( ""  ) ); 
  BOOST_CHECK_EQUAL( "*&%#01234" , STLStringTools::String2Lower( "*&%#01234"  ) ); 
  BOOST_CHECK_EQUAL( "abc...åäö" , STLStringTools::String2Lower( "ABC...ÅÄÖ"  ) ); 
}

BOOST_AUTO_TEST_CASE( Strip )
{
  BOOST_CHECK_EQUAL( "" , STLStringTools::Strip( "" , "abc" ) ); 
  BOOST_CHECK_EQUAL( "abcde" , STLStringTools::Strip( "abcde" , "" ) ); 

  BOOST_CHECK_EQUAL( "ab" , STLStringTools::Strip( "abcde" , "" , 2 ) ); 
  BOOST_CHECK_EQUAL( "bc" , STLStringTools::Strip( "abcdef" , "a" , 2 ) ); 
}

BOOST_AUTO_TEST_CASE( Split )
{
  vector<string> actual;
  actual = STLStringTools::Split( "" , "!#" );
  BOOST_CHECK_EQUAL( 0 , actual.size() ); 

  actual = STLStringTools::Split( "1" , "!#" );
  BOOST_CHECK_EQUAL( 1 , actual.size() ); 
  BOOST_CHECK_EQUAL( "1" , actual[0] ); 

  actual = STLStringTools::Split( "!#" , "!#" );
  BOOST_CHECK_EQUAL( 2 , actual.size() ); 
  BOOST_CHECK_EQUAL( "" , actual[0] ); 
  BOOST_CHECK_EQUAL( "" , actual[1] ); 

  actual = STLStringTools::Split( "1!#2!#3" , "!#" );
  BOOST_CHECK_EQUAL( 3 , actual.size() ); 
  BOOST_CHECK_EQUAL( "1" , actual[0] ); 
  BOOST_CHECK_EQUAL( "2" , actual[1] ); 
  BOOST_CHECK_EQUAL( "3" , actual[2] ); 


}

BOOST_AUTO_TEST_CASE( Join )
{
  string actual;
  vector<string> in;
  actual = STLStringTools::Join( in , "!#" );
  BOOST_CHECK_EQUAL( "" , actual ); 

  in.push_back("AAA");
  actual = STLStringTools::Join( in , "!#" );
  BOOST_CHECK_EQUAL( "AAA" , actual ); 

  in.push_back("BB");
  actual = STLStringTools::Join( in , "!#" );
  BOOST_CHECK_EQUAL( "AAA!#BB" , actual ); 

  in.push_back("C");
  actual = STLStringTools::Join( in , "!#" );
  BOOST_CHECK_EQUAL( "AAA!#BB!#C" , actual ); 



}

