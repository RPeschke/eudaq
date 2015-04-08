#ifndef LCIO_INCLUDED_Platform
#define LCIO_INCLUDED_Platform

#ifdef WIN32
  #define DLLEXPORT_LCIO  __declspec( dllexport ) 
#else
  #define DLLEXPORT_LCIO  
#endif


#endif // EUDAQ_INCLUDED_Platform
