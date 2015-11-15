#ifndef EUDAQ_INCLUDED_Producer
#define EUDAQ_INCLUDED_Producer

#include "eudaq/CommandReceiver.hh"
#include "eudaq/Platform.hh"
#include <string>
#include <memory>
#include "eudaq/Processor_inspector.hh"


namespace eudaq {
class Processor_Inspector;
class Event;
  /**
   * The base class from which all Producers should inherit.
   * It is both a CommandReceiver, listening to commands from RunControl,
   * and a DataSender, sending data to a DataCollector.
   */
  class DLLEXPORT Producer : public CommandReceiver {
    public:
      /**
       * The constructor.
       * \param runcontrol A string containing the address of the RunControl to connect to.
       */
      Producer(const std::string & name, const std::string & runcontrol);
      virtual ~Producer() {}

      void SendEvent(const Event &);
    private:
      virtual void OnData(const std::string & param) override final;
      std::unique_ptr<Processor_Inspector> m_dataSender;
      std::string m_name;
  };

}

#endif // EUDAQ_INCLUDED_Producer
