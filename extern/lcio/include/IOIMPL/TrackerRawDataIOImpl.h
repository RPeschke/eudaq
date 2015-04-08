#ifndef SIO_TPCHITIOIMPL_H
#define SIO_TPCHITIOIMPL_H 1

#include "LCPlatform.hh"
#include "IMPL/TrackerRawDataImpl.h"

namespace SIO{

  class SIOTrackerRawDataHandler ;
}

namespace IOIMPL {

  class SIOCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Sep 11, 2003
 */
   class DLLEXPORT_LCIO TrackerRawDataIOImpl : public IMPL::TrackerRawDataImpl {
    
    friend class SIO::SIOTrackerRawDataHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TPCHITIOIMPL_H */
