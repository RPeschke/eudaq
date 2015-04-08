// -*- C++ -*-
#ifndef UTIL_ILDConf_H
#define UTIL_ILDConf_H 1

#include "LCPlatform.hh"
#include <string>
#include "CellIDEncoder.h"
#include "CellIDDecoder.h"

#include "BitSet32.h"


#ifdef WIN32
#define  encoder_string __encoder_string() 
#endif // WIN32

// Some useful definitions that are used in the ILD software framework - in particular for
// encoding and decoding cellIDs.

namespace UTIL {
  
  


  /** Helper class to define constants for the canonical encoding of the CellID0 in tracking sub detectors
   *  in ILD (or ILD like detectors).
   * 
   *  F.Gaede, DESY, 2011
   *  @version $Id:$
   */
  struct DLLEXPORT_LCIO ILDCellID0 {
    
    /** The canonical encoding string to be used for writing the CellID0 with the CellIDEncoder<T>: 
      *   <pre>"subdet:5,side:-2,layer:9,module:8:sensor:8"</pre> 
      *
      *  Use the CellIdDecoder to access the elements:
      *  <pre>
      *     CellIDDecoder<TrackerHit> idDec( trkHitCol ) ;
      *      // ...
      *      TrackerHit* hit = (TrackerHit*) trkHitCol.getElementAt( i ) ;
      *
      *      int layer  = idDec( hit )[ ILDCellID0::layer ] ;
      *  </pre>
      */

#ifndef WIN32
    static const std::string encoder_string; 
#else

    static const std::string __encoder_string(); 
#endif
    enum : unsigned
    {
      /** Index of the field subdet in encoder_string - sub detector Id as defined in DetID, e.g. DetID::VXD or DetID::FTD.
      */
      subdet = 0,

      /** Index of the field side in encoder_string - one of DetID::bwd, DetID::barrel, DetID::fwd.
      */
      side =1,

      /** Index of the field layer in encoder_string - local sub detector layer ID starting from 0 going away from the origin (IP).
      */
      layer = 2,

      /** Index of the field module in encoder_string - module ID  as defined for the given subdetetor. */
      module = 3,

      /** Index of the field sensor in encoder_string -  sensor ID  as defined for the given subdetetor module. */
      sensor = 4
    };

  }; 
  
  

  /** Special wrapper to the CellIDEncoder<T> that enforces cellID0 to use the encoder string 
   *  defined in ILDCellID0::encoder_string.
   */
  template <class T> 
  class DLLEXPORT_LCIO ILDCellIDEncoder : public CellIDEncoder<T> {
    
  public:  
    
    /** Constructor for using the canonical cellID0 as defined in ILDCellID0::encoder_string - cellID1 will not be used.
     */
    ILDCellIDEncoder( EVENT::LCCollection* col) : 
      CellIDEncoder<T>( ILDCellID0::encoder_string, col ) {}
    

    /** Constructor that prepends given ILDCellID0::encoder_string to the given encoding string.
     *  Throws an Exception if called with an empty string. 
     */
    ILDCellIDEncoder( const std::string& cellIDEncoding ,  EVENT::LCCollection* col) : 
      CellIDEncoder<T>(std::string(ILDCellID0::encoder_string +"," + cellIDEncoding), col) {
      
      if(  cellIDEncoding.size() == 0 ){
	throw EVENT::Exception(" ILDCellIDEncoder::ILDCellIDEncoder: cannot initilize with empty string ! " ) ;
      }
    }
  } ;
  




  /** Define integer constants for identifying ILD sub detectors. Typically used in CellIDs, for example together with
   *  ILD::CellID0::encoder_string.
   *  
   *  F.Gaede, DESY, 2011
   *  @version $Id:$
   */
  struct ILDDetID{
    
    enum :int {
     NOTUSED = 0,
     VXD = 1,
     SIT = 2,
     FTD = 3,
     TPC = 4,
     SET = 5,
     ETD = 6,
     ECAL = 20,
     ECAL_PLUG = 21,
     HCAL = 22,
     HCAL_RING = 23,
     LCAL = 24,
     BCAL = 25,
     LHCAL = 26,
     YOKE = 27,
     bwd = -1,
     barrel = 0,
     fwd = 1
    };
    
   
  } ;
  
  /** Define integer constants for bits of the the lcio tracker hit type  used in the context of ILD reconstruction code
   *  
   *  S.J. Aplin, DESY, 2012
   *  @version $Id:$
   */
  
  struct ILDTrkHitTypeBit{
    enum{
      COMPOSITE_SPACEPOINT = 30, // i.e. a 3D space-point composed of two independent strip measurements
      ONE_DIMENSIONAL = 29       // e.g. a strip measurment
    };
  } ;

  /** Define integer constants for bits of the the lcio tracker hit quality  used in the context of ILD reconstruction code
   *  
   *  F.Gaede, DESY, 2012
   *  @version $Id:$
   */
  enum ILDTrkHitQualityBit{
    
   USED_IN_FIT = 30,
   USED_IN_TRACK = 29,
   DOUBLE_HIT_CANDIDATE = 28,
   GOOD = 27
  } ;

  

} // namespace UTIL
#endif // ifndef UTIL_ILDConf_H
