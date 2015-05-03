#include "eudaq/StringEvent.hh"

#include <ostream>

namespace eudaq {

  EUDAQ_DEFINE_EVENT(StringEvent, str2id("_STR"));

  StringEvent::StringEvent(Deserializer & ds) :
    Event(ds)
  {
    ds.read(m_str);
  }

  void StringEvent::Print(std::ostream & os) const {
    Print(os, 0);
  }

  void StringEvent::Print(std::ostream &os, size_t offset) const
  {
    Event::Print(os,offset);
    static const size_t max = 32;
    std::string s(m_str, 0, max);
    os << std::string(offset, ' ') << ", '" << s << (m_str.length() > max ? ("' (+" + to_string(m_str.length() - max) + ")") : "'");
  }

  void StringEvent::Serialize(Serializer & ser) const {
    Event::Serialize(ser);
    ser.write(m_str);
  }

}
