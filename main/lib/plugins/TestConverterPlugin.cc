#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Event.hh"
namespace eudaq {

  // Test Events have nothing to convert
  // This class is only here to prevent a runtime errors
  class TestConverterPlugin : public DataConverterPlugin<Event> {
    TestConverterPlugin() : DataConverterPlugin("Test") {}
    static TestConverterPlugin m_instance;
  };

  TestConverterPlugin TestConverterPlugin::m_instance;

}
