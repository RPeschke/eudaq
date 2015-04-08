#ifndef SIO_RAWCALORIMETERHITIOIMPL_H
#define SIO_RAWCALORIMETERHITIOIMPL_H 1

#include "LCPlatform.hh"
#include "IMPL/RawCalorimeterHitImpl.h"

namespace SIO{

  class SIORawCalHitHandler ;

}

namespace IOIMPL {

  class SIORawCalHitHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede
 * @version $Id: RawCalorimeterHitIOImpl.h,v 1.2 2005-04-15 08:37:40 gaede Exp $
 */
   class DLLEXPORT_LCIO RawCalorimeterHitIOImpl : public IMPL::RawCalorimeterHitImpl {
    
    friend class SIO::SIORawCalHitHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_RAWCALORIMETERIOHITIMPL_H */
