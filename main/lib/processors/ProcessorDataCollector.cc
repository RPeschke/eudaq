#include "eudaq/ProcessorBase.hh"
#include "eudaq/TransportServer.hh"
#include "eudaq/TransportFactory.hh"
#include "eudaq/Exception.hh"
#include "eudaq/Processors.hh"

#include <thread>
#include <memory>

namespace eudaq {



class processor_data_collector :public ProcessorBase {
public:
  processor_data_collector(const std::string& listAdress);
  ~processor_data_collector();
  virtual void init() { m_status = runnning; };
  virtual ReturnParam ProcessEvent(event_sp ev, ConnectionName_ref con);
  virtual void end() { m_status = waiting; };
  void DataThread();

private:

  struct Info {
    std::shared_ptr<ConnectionInfo> id;
    ConnectionName_t m_counter;
  };
  std::vector<Info> m_buffer;

  size_t GetInfo(const ConnectionInfo & id);



  void OnConnect(const ConnectionInfo & id);
  ConnectionName_t GetInfoCounter(const ConnectionInfo & id);

  void OnDisconnect(const ConnectionInfo & id);
  void OnReceive(const ConnectionInfo & id, std::shared_ptr<Event> ev);
  void DataHandler(TransportEvent & ev);

  std::unique_ptr<TransportServer> m_dataServer;
  enum status {
    done,
    waiting,
    runnning
  }m_status = waiting;
  std::unique_ptr<std::thread> m_thread;
  ConnectionName_t m_connectionID = random_connection();
};

Processors::processor_up Processors::dataReciver(const std::string& listAdrrs) {
  return std::unique_ptr<ProcessorBase>(new processor_data_collector(listAdrrs));
}

void * DataCollector_thread(void * arg) {
  processor_data_collector * dc = static_cast<processor_data_collector *>(arg);
  dc->DataThread();
  return 0;
}
processor_data_collector::processor_data_collector(const std::string& listAdress)
  : m_dataServer(
  TransportFactory::CreateServer(listAdress)
  ) {
  m_dataServer->SetCallback(TransportCallback(this, &processor_data_collector::DataHandler));

  m_thread = std::unique_ptr<std::thread>(new std::thread(DataCollector_thread, this));
}

processor_data_collector::~processor_data_collector() {
  m_status = done;
  m_thread->join();
}

ProcessorBase::ReturnParam processor_data_collector::ProcessEvent(event_sp ev, ConnectionName_ref con) {
  return processNext(std::move(ev), con);
}

void processor_data_collector::DataThread() {

  try {
    while (m_status!=done) {
      m_dataServer->Process(100000);
    }
  } catch (const std::exception & e) {
    std::cout << "Error: Uncaught exception: " << e.what() << "\n" << "DataThread is dying..." << std::endl;
  } catch (...) {
    std::cout << "Error: Uncaught unrecognised exception: \n" << "DataThread is dying..." << std::endl;
  }

}

size_t processor_data_collector::GetInfo(const ConnectionInfo & id) {
  for (size_t i = 0; i < m_buffer.size(); ++i) {
    //std::cout << "Checking " << *m_buffer[i].id << " == " << id<< std::endl;
    if (m_buffer[i].id->Matches(id))
      return i;
  }
  EUDAQ_THROW("Unrecognised connection id");
}

void processor_data_collector::OnConnect(const ConnectionInfo & id) {
  Info info;
  info.id = std::shared_ptr<ConnectionInfo>(id.Clone());
  info.m_counter = random_connection();
  m_buffer.push_back(info);
}

ProcessorBase::ConnectionName_t processor_data_collector::GetInfoCounter(const ConnectionInfo & id) {
  for (size_t i = 0; i < m_buffer.size(); ++i) {
    //std::cout << "Checking " << *m_buffer[i].id << " == " << id<< std::endl;
    if (m_buffer[i].id->Matches(id))
      return m_buffer[i].m_counter;
  }
  EUDAQ_THROW("Unrecognised connection id");
}

void processor_data_collector::OnDisconnect(const ConnectionInfo & id) {
  size_t i = GetInfo(id);
  m_buffer.erase(m_buffer.begin() + i);
}

void processor_data_collector::OnReceive(const ConnectionInfo & id, std::shared_ptr<Event> ev) {
  processNext(std::move(ev), GetInfoCounter(id));
}

void processor_data_collector::DataHandler(TransportEvent & ev) {
  //std::cout << "Event: ";
  switch (ev.etype) {
  case (TransportEvent::CONNECT) :
    //std::cout << "Connect:    " << ev.id << std::endl;
    if (m_status == waiting) {
      m_dataServer->SendPacket("OK EUDAQ DATA DataCollector", ev.id, true);
    } else {
      m_dataServer->SendPacket("ERROR EUDAQ DATA Not accepting new connections", ev.id, true);
      m_dataServer->Close(ev.id);
    }
                                 break;
  case (TransportEvent::DISCONNECT) :
    //std::cout << "Disconnect: " << ev.id << std::endl;
    OnDisconnect(ev.id);
    break;
  case (TransportEvent::RECEIVE) :
    if (ev.id.GetState() == 0) { // waiting for identification
      // check packet
      do {
        size_t i0 = 0, i1 = ev.packet.find(' ');
        if (i1 == std::string::npos)
          break;
        std::string part(ev.packet, i0, i1);
        if (part != "OK")
          break;
        i0 = i1 + 1;
        i1 = ev.packet.find(' ', i0);
        if (i1 == std::string::npos)
          break;
        part = std::string(ev.packet, i0, i1 - i0);
        if (part != "EUDAQ")
          break;
        i0 = i1 + 1;
        i1 = ev.packet.find(' ', i0);
        if (i1 == std::string::npos)
          break;
        part = std::string(ev.packet, i0, i1 - i0);
        if (part != "DATA")
          break;
        i0 = i1 + 1;
        i1 = ev.packet.find(' ', i0);
        part = std::string(ev.packet, i0, i1 - i0);
        ev.id.SetType(part);
        i0 = i1 + 1;
        i1 = ev.packet.find(' ', i0);
        part = std::string(ev.packet, i0, i1 - i0);
        ev.id.SetName(part);
      } while (false);
      //std::cout << "client replied, sending OK" << std::endl;
      m_dataServer->SendPacket("OK", ev.id, true);
      ev.id.SetState(1); // successfully identified
      OnConnect(ev.id);
    } else {

      BufferSerializer ser(ev.packet.begin(), ev.packet.end());
      std::shared_ptr<Event> event(EventFactory::Create(ser));
      OnReceive(ev.id, std::move(event));

    }
                                 break;
  default:
    std::cout << "Unknown:    " << ev.id << std::endl;
  }
}

}