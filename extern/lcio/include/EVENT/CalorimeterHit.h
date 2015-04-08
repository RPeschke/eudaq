// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef EVENT_CALORIMETERHIT_H
#define EVENT_CALORIMETERHIT_H 1

#include "LCPlatform.hh"
#include "EVENT/LCObject.h"
#include "empty_ignore.h"

#include <vector>

namespace EVENT {

class LCObject;


class CalorimeterHit ;
/**Vector of (pointers to) CalorimeterHits.*/
typedef std::vector<CalorimeterHit*> CalorimeterHitVec ;
/** The generic calorimeter hit - created from SimCalorimeterHit or
 *  RawCalorimeterHit. The difference to RawCalorimeterHit is that amplitude
 *  ( energy) and time are now float and an optional position (x,y,z) can be stored
 *  with the hit.
 *  In order to suppress a (32-bit) pointer tag stored with every hit set
 *  flag(LCIO::RCHBIT_NO_PTR)==1. 
 *  
 * @author gaede
 * @version $Id: CalorimeterHit.aid,v 1.16 2008-06-04 17:37:15 engels Exp $
 */

class  DLLEXPORT_LCIO CalorimeterHit : public LCObject {

public: 
    /// Destructor.
    virtual ~CalorimeterHit() { /* nop */; }


    /** Useful typedef for template programming with LCIO */
    typedef CalorimeterHit lcobject_type ;

    /** Returns the detector specific (geometrical) cell id.
     */
    virtual int getCellID0() const = 0;

    /** Returns the second detector specific (geometrical) cell id. Optional, check/set 
     *  flag(LCIO::RCHBIT_ID1)==1.
     */
    virtual int getCellID1() const = 0;

    /** Returns the energy of the hit in [GeV].
     */
    virtual float getEnergy() const = 0;

    /** Returns the error of the hit energy in [GeV]. Optional, check/set
     *  flag(LCIO::RCHBIT_ENERGY_ERROR)==1.
     */
    virtual float getEnergyError() const = 0;

    /** Returns the time of the hit in [ns]. Optional, check/set 
     *  flag(LCIO::RCHBIT_TIME)==1.
     */
    virtual float getTime() const = 0;

    /** Returns the position of the hit in world coordinates.
     *  NULL if information is not stored - check 
     *  flag(LCIO::RCHBIT_LONG)==1.
     */
    virtual const float* getPosition() const = 0;

    /** Type of hit. Mapping of integer types to type names
     * through collection parameters "CalorimeterHitTypeNames"
     * and "CalorimeterHitTypeValues".
     */
    virtual int getType() const = 0;

    /** The RawCalorimeterHit. As in the future there might be other types of 
     *  raw calorimeter hits the return type is LCObject.
     */
    virtual LCObject * getRawHit() const = 0;
}; // class
} // namespace EVENT
#endif /* ifndef EVENT_CALORIMETERHIT_H */
