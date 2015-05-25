#include "eudaq/Processor_inspector.hh"
#include <tuple>


using namespace std;

namespace eudaq{
  using ReturnParam = ProcessorBase::ReturnParam;

  class ShowEventNR :public Processor_Inspector{
  public:
    virtual void initialize(Configuration_ref conf) {
      firstEvent = true;
    }
    virtual void Finish() {
    }
    virtual ReturnParam inspecktEvent(const Event&);

    virtual void print(std::ostream& os);
    class timeing{
    public:
      void reset(){
        freq_all_time = 0;
        freq_current = 0;
        m_last_event = 0;
        m_last_time = std::clock();
        m_start_time = std::clock();
      }

      void processEvent(const Event& ev){
        auto newClock = std::clock();

        freq_all_time = ( (double)(newClock - m_start_time) / CLOCKS_PER_SEC) / (ev.GetEventNumber());

        freq_current = ((double)(newClock - m_last_time) / CLOCKS_PER_SEC) / (ev.GetEventNumber() - m_last_event);
        m_last_event = ev.GetEventNumber();
        m_last_time = std::clock();
      }
      double freq_all_time = 0,
        freq_current=0;
    private:
      unsigned m_last_event = 0;
      clock_t m_last_time = 0,m_start_time=0;
    } m_timming;
    tuple<int, int> CalcRate(const Event& ev);
    
    ShowEventNR(Parameter_ref);
    void show(const Event& ev);
    size_t m_pos;
    bool firstEvent = true;
  };
  RegisterProcessor(ShowEventNR, ProcessorNames::show_event_nr());




  void ShowEventNR::print(std::ostream& os)
  {
    os << getName();
  }



  ShowEventNR::ShowEventNR(Parameter_ref conf) :Processor_Inspector(conf)
  {
    m_pos =4* ProConfig::getProcessorParallelPos(conf);
  }



  void ShowEventNR::show(const Event& ev)
  {
    m_timming.processEvent(ev);
    std::cout << std::string(m_pos, ' ') << getName() << ": " << ev.GetEventNumber() << " rate = " << m_timming.freq_current <<std::endl;
  }

  ReturnParam ShowEventNR::inspecktEvent(const Event& ev)
  {
    if (firstEvent )
    {
      m_timming.reset();
    }
    if (ev.GetEventNumber() <10)
    {
      show(ev);
    }
    else if (ev.GetEventNumber()% 100 ==0)
    {
      show(ev);
    }
   
    return sucess;
  }

}

