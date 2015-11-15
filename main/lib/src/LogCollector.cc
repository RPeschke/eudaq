#include "eudaq/LogCollector.hh"
#include "eudaq/LogMessage.hh"
#include "eudaq/Logger.hh"
#include "eudaq/TransportFactory.hh"
#include "eudaq/BufferSerializer.hh"
#include <iostream>
#include <ostream>
#include <sstream>

namespace eudaq {

  namespace {

    void * LogCollector_thread(void * arg) {
      LogCollector * dc = static_cast<LogCollector *>(arg);
      dc->LogThread();
      return 0;
    }

  } // anonymous namespace

  LogCollector::LogCollector(const std::string & runcontrol,
      const std::string & listenaddress)
    : CommandReceiver("LogCollector", "", runcontrol, false),
    m_done(false),
    m_logserver(TransportFactory::CreateServer(listenaddress)),
    m_filename("../logs/" + Time::Current().Formatted("%Y-%m-%d.log")),
    m_file(m_filename.c_str(), std::ios_base::app)
  {
    if (!m_file.is_open()) EUDAQ_THROWX(FileWriteException, "Unable to open log file (" + m_filename + ") is there a logs directory?");
    m_logserver->SetCallback(TransportCallback(this, &LogCollector::LogHandler));
    //pthread_attr_init(&m_threadattr);
    //pthread_create(&m_thread, &m_threadattr, LogCollector_thread, this);
	m_thread=std::unique_ptr<std::thread>(new std::thread( LogCollector_thread, this));
    std::cout << "###### listenaddress=" << m_logserver->ConnectionString() << std::endl
      << "       logfile=" << m_filename << std::endl;
    std::ostringstream os;
    os << "\n*** LogCollector started at " << Time::Current().Formatted() << " ***" << std::endl;
    m_file.write(os.str().c_str(), os.str().length());
    CommandReceiver::StartThread();
  }

  LogCollector::~LogCollector() {
    m_file << "*** LogCollector stopped at " << Time::Current().Formatted() << " ***" << std::endl;
    m_done = true;
    ///*if (m_thread)*/ pthread_join(m_thread, 0);
	m_thread->join();
    delete m_logserver;
  }

  void LogCollector::OnServer() {
    if (!m_logserver) EUDAQ_ERROR("Oops");
    //std::cout << "OnServer: " << m_logserver->ConnectionString() << std::endl;
    m_status.SetTag("_SERVER", m_logserver->ConnectionString());
  }

  void LogCollector::DoReceive(const LogMessage & ev) {
    ev.Write(m_file);
	  m_file.flush();
    OnReceive(ev);
  }

  void handleNameAndTag1(TransportEvent& ev) {
    auto splitted_packet = eudaq::split(ev.packet, " ");
    if (splitted_packet.size() < 5) {
      return;
    }
    if (splitted_packet[0] != std::string("OK")) {
      return;
    }
    if (splitted_packet[1] != std::string("EUDAQ")) {
      return;
    }
    if (splitted_packet[2] != std::string("LOG")) {
      return;
    }
    ev.id.SetType(splitted_packet[3]);
    ev.id.SetName(splitted_packet[4]);
  }

void LogCollector::HandleUnidentified(TransportEvent & ev) {
    // check packet

    handleNameAndTag1(ev);
    m_logserver->SendPacket("OK", ev.id, true);
    ev.id.SetState(1); // successfully identified
    OnConnect(ev.id);
  }
  void LogCollector::LogHandler(TransportEvent & ev) {
    
    switch (ev.etype) {
    case (TransportEvent::CONNECT) :
       m_logserver->SendPacket("OK EUDAQ LOG LogCollector", ev.id, true);
        break;
      case (TransportEvent::DISCONNECT):
        OnDisconnect(ev.id);
        break;
      case (TransportEvent::RECEIVE):
        if (ev.id.GetState() == 0) { // waiting for identification
          HandleUnidentified(ev);
        } else {

          BufferSerializer ser(ev.packet.begin(), ev.packet.end());
          std::string src = ev.id.GetType();
          if (ev.id.GetName() != "") src += "." + ev.id.GetName();
          DoReceive(LogMessage(ser).SetSender(src));
        }
        break;
      default:
        std::cout << "Unknown:    " << ev.id << std::endl;
    }
  }



  void LogCollector::LogThread() {
    try {
      while (!m_done) {
        m_logserver->Process(100000);
      }
    } catch (const std::exception & e) {
      std::cout << "Error: Uncaught exception: " << e.what() << "\n"
        << "LogThread is dying..." << std::endl;
    } catch (...) {
      std::cout << "Error: Uncaught unrecognised exception: \n"
        << "LogThread is dying..." << std::endl;
    }
  }

}
