#ifndef IMPL_LCIOEXCEPTIONHANDLER_H
#define IMPL_LCIOEXCEPTIONHANDLER_H 1

#include "LCPlatform.hh"
namespace IMPL{

/** Sets the default error handling for unexpected and uncaught exceptions.
 */
  class DLLEXPORT_LCIO LCIOExceptionHandler{
    
  private:

  public:
    LCIOExceptionHandler() ;
    
    static LCIOExceptionHandler* createInstance() ;

  private:

    static LCIOExceptionHandler* _me ;

  };
} // namespace

#endif
