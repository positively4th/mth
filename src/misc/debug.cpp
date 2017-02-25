/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/

#include <debug.h>
#include <cstddef>
#include <execinfo.h>
#include <stdio.h>

namespace P4th {

  void show_stackframe()
  {
    void *trace[256];
    char **messages = (char **)NULL;
    int i, trace_size = 0;
    
    trace_size = backtrace( trace , 256 );
    messages = backtrace_symbols( trace , trace_size );
    
    printf( "Execution path:\n" );
    for ( int i = 0 ; i < trace_size ; i++ )
      printf( "%s:\n" , messages[i] );
  }
  
}
