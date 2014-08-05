#ifndef EUDAQ_INCLUDED_Event
#define EUDAQ_INCLUDED_Event

#include <string>
#include <vector>
#include <map>
#include <iosfwd>
#include <iostream>

#include "eudaq/Serializable.hh"
#include "eudaq/Serializer.hh"
#include "eudaq/Exception.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Platform.hh"


#define EUDAQ_DECLARE_EVENT(type)           \
  public:                                   \
static unsigned eudaq_static_id();      \
virtual unsigned get_id() const {       \
  return eudaq_static_id();             \
}                                       \
private:                                  \
static const int EUDAQ_DUMMY_VAR_DONT_USE = 0

#define EUDAQ_DEFINE_EVENT(type, id)       \
  unsigned type::eudaq_static_id() {       \
    static const unsigned id_(id);         \
    return id_;                            \
  }                                        \
namespace _eudaq_dummy_ {                \
  static eudaq::RegisterEventType<type> eudaq_reg;	\
}                                        \
static const int EUDAQ_DUMMY_VAR_DONT_USE = 0

namespace eudaq {
  
  static const uint64_t NOTIMESTAMP = (uint64_t)-1;

  class DLLEXPORT Event : public Serializable {
    public:
      typedef unsigned mainType;
      typedef std::string subType;
      typedef std::pair<mainType, subType> t_id;
      typedef uint64_t timeStamp_t;

      enum Flags { FLAG_BORE=1, FLAG_EORE=2, FLAG_HITS=4, FLAG_FAKE=8, FLAG_SIMU=16, FLAG_ALL=(unsigned)-1 }; // Matches FLAGNAMES in .cc file
      Event(unsigned run, unsigned event, timeStamp_t timestamp = NOTIMESTAMP, unsigned flags=0)
        : m_flags(flags), m_runnumber(run), m_eventnumber(event), m_timestamp(timestamp) {}
      Event(Deserializer & ds);
      Event(){}
      virtual void Serialize(Serializer &) const;

      unsigned GetRunNumber() const { return m_runnumber; }
      unsigned GetEventNumber() const { return m_eventnumber; }
      timeStamp_t GetTimestamp() const { return m_timestamp; }

      /** Returns the type string of the event implementation.
       *  Used by the plugin mechanism to identify the event type.
       */
      virtual subType GetSubType() const { return ""; }

      t_id getID() const { return t_id(get_id(), GetSubType()); }

      virtual void Print(std::ostream & os)const;

      Event & SetTag(const std::string & name, const std::string & val);
      template <typename T>
        Event & SetTag(const std::string & name, const T & val) {
          return SetTag(name, to_string(val));
        }
      std::string GetTag(const std::string & name, const std::string & def = "") const;
      std::string GetTag(const std::string & name, const char * def) const { return GetTag(name, std::string(def)); }
      template <typename T>
        T GetTag(const std::string & name, T def) const {
          return from_string(GetTag(name), def);
        }

      bool IsBORE() const { return GetFlags(FLAG_BORE) != 0; }
      bool IsEORE() const { return GetFlags(FLAG_EORE) != 0; }
      bool HasHits() const { return GetFlags(FLAG_HITS) != 0; }
      bool IsFake() const { return GetFlags(FLAG_FAKE) != 0; }
      bool IsSimulation() const { return GetFlags(FLAG_SIMU) != 0; }

      static mainType str2id(const std::string & idstr);
      static std::string id2str(mainType id);
      unsigned GetFlags(unsigned f = FLAG_ALL) const { return m_flags & f; }
      void SetFlags(unsigned f) { m_flags |= f; }
	  void SetTimeStampToNow();
    void setTimeStamp(timeStamp_t timeStamp){ m_timestamp = timeStamp; }
      void ClearFlags(unsigned f = FLAG_ALL) { m_flags &= ~f; }
      virtual mainType get_id() const { return Event::str2id("base"); }
    protected:
      typedef std::map<std::string, std::string> map_t;

      unsigned m_flags, m_runnumber, m_eventnumber;
      timeStamp_t m_timestamp;
      map_t m_tags; ///< Metadata tags in (name=value) pairs of strings
  };
  DLLEXPORT std::string to_string(const Event::t_id EventID);
  DLLEXPORT std::string to_string(const Event& ev);
  DLLEXPORT std::ostream&  operator<< (std::ostream &, const Event & ev);

  class DLLEXPORT EventFactory {
    public:
      static Event * Create(Deserializer & ds) {
        unsigned id = 0;
        ds.read(id);
        //std::cout << "Create id = " << std::hex << id << std::dec << std::endl;
        event_creator cr = GetCreator(id);
        if (!cr) EUDAQ_THROW("Unrecognised Event type (" + Event::id2str(id) + ")");
        return cr(ds);
      }

      typedef Event * (* event_creator)(Deserializer & ds);
      static void Register(uint32_t id, event_creator func);
      static event_creator GetCreator(uint32_t id);

    private:
      typedef std::map<uint32_t, event_creator> map_t;
      static map_t & get_map();
  };

  /** A utility template class for registering an Event type.
   */
  template <typename T_Evt>
    struct RegisterEventType {
      RegisterEventType() {
        EventFactory::Register(T_Evt::eudaq_static_id(), &factory_func);
      }
      static Event * factory_func(Deserializer & ds) {
        return new T_Evt(ds);
      }
    };



}

#endif // EUDAQ_INCLUDED_Event
