#ifndef SIO_TRACKERHITZCYLINDERIOIMPL_H
#define SIO_TRACKERHITZCYLINDERIOIMPL_H 1

#include "LCPlatform.hh"
#include "IMPL/TrackerHitZCylinderImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackerHitZCylinderHandler ;
}

namespace IOIMPL {

  class SIOTrackerHitZCylinderHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author engels
 * @version 2011-06
 */
   class DLLEXPORT_LCIO TrackerHitZCylinderIOImpl : public IMPL::TrackerHitZCylinderImpl {
    
    friend class SIO::SIOTrackerHitZCylinderHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKERHITZCYLINDERIOIMPL_H */
