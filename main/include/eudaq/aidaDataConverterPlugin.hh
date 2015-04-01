#ifndef aidaDataConverterPlugin_h__
#define aidaDataConverterPlugin_h__
#include <utility>

#include <stdint.h>
#include <string>
#include "eudaq/AidaPacket.hh"

namespace lcio{
  class LCRunHeader;
  class LCEvent;
}

namespace eudaq{

  class AidaPacket;
  class TLUEvent;
  class Configuration;
  class StandardEvent;

  class aidaDataConverterPlugin {
  public:
    typedef AidaPacket::t_id t_packetid;

    virtual void Initialize(AidaPacket const &, eudaq::Configuration const &) {}
    /** Returns the first trigger id 
    */
    virtual unsigned GetTriggerID(AidaPacket const & ai) const;
    virtual unsigned GetTriggerID(AidaPacket const& ai, unsigned const TriggerNo) const;
    virtual unsigned GetNumberOfTriggers(AidaPacket const& ai) const{ return static_cast<unsigned>(-1); }; // $$TODO this functionality needs to be implemented
    

    virtual int IsSyncWithTLU(AidaPacket const & ev,eudaq::TLUEvent const & tlu) const;

    virtual void setCurrentTLUEvent(AidaPacket & ev,eudaq::TLUEvent const & tlu);

    /** Returns the StandardEvent version of the event.
    */
    virtual bool GetStandardSubEvent(StandardEvent & /*result*/, AidaPacket const & /*source*/) const;

    /** Returns the type of event this plugin can convert to lcio as a pair of Event type id and subtype string.
    */
    virtual t_packetid const & GetEventType() const { return m_eventtype; }

    virtual void GetLCIORunHeader(lcio::LCRunHeader &, AidaPacket const &, eudaq::Configuration const &) const {}

    /** Returns the LCIO version of the event.
    */
    virtual bool GetLCIOSubEvent(lcio::LCEvent & /*result*/, AidaPacket const & /*source*/) const { return false; }

    
    /** The empty destructor. Need to add it to make it virtual.
    */
    virtual ~aidaDataConverterPlugin() {}


  protected:
    /** The string storing the event type this plugin can convert to lcio.
    *  This string has to be set in the constructor of the actual implementations
    *  of the plugin.
    */
    t_packetid m_eventtype;


    /** The protected constructor which automatically registers the plugin
    *  at the pluginManager.
    */
    aidaDataConverterPlugin(std::string subtype);
    aidaDataConverterPlugin(std::string type, std::string subtype );

  private:
    /** The private copy constructor and assignment operator. They are not used anywhere, so there is not
    *  even an implementation. Even if the childs default copy constructor is public
    *  the code will not compile if it is called, since it cannot acces this cc, which the
    *  the default cc does.
    */
    aidaDataConverterPlugin(aidaDataConverterPlugin &)=delete;
    aidaDataConverterPlugin & operator = (const aidaDataConverterPlugin &)=delete;
  };

}

#endif // aidaDataConverterPlugin_h__