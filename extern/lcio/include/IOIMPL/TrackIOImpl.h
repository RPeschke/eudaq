#ifndef SIO_TRACKIOIMPL_H
#define SIO_TRACKIOIMPL_H 1

#include "LCPlatform.hh"
#include "IMPL/TrackImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackHandler ;
}

namespace IOIMPL {

  class SIOTrackHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version Mar 15, 2004
 */
   class DLLEXPORT_LCIO TrackIOImpl : public IMPL::TrackImpl {
    
    friend class SIO::SIOTrackHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKIOIMPL_H */
