#include "eudaq/Processor_inspector.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Utils.hh"


namespace eudaq{
  class getConfig :public Processor_Inspector{
  public:
    getConfig(Parameter_ref conf) :Processor_Inspector(conf){ }
    virtual void initialize(Configuration_ref conf) {
      auto sectionAndTag = ProConfig::getTag(conf, getName(), "section_and_TAG", "");

      auto splittedSectionAndTags = eudaq::split(sectionAndTag, "&", true);
      for (auto& e:splittedSectionAndTags)
      {
        if (!e.empty())
        {
          push_tags(e);
        }
      }

    }

    void push_tags(const std::string& sectionAndTag){
      auto splitted = eudaq::split(sectionAndTag, "%", true);
      if (splitted.size()!=2)
      {
        EUDAQ_THROW("wrong number of elements");
      }
      m_tags.emplace_back(splitted.at(0), splitted.at(1));
    }
    virtual ReturnParam inspecktEvent(const Event&ev) {

      if (ev.IsBORE())
      {
        for (auto& e:m_tags)
        {
        Configuration conf(ev.GetTag("CONFIG", ""), e.first);
        if (e.second.empty())
        {
          conf.Save(std::cout);
          //conf.Print();
        }
        else{

          std::cout << e.second << " = " << conf.Get(e.second, "") << std::endl;
        }
        }
      }
      return ProcessorBase::sucess;
    }


    std::vector<std::pair<std::string, std::string>> m_tags;
    static  Configuration_t setSection(const std::string& section){
      return ProConfig::Tag("section", section);
    }
    static  Configuration_t setTag(const std::string& tag){
      return ProConfig::Tag("tag", tag);
    }
  };


  class select_events :public Processor_Inspector{
  public:
    select_events(Parameter_ref conf) :Processor_Inspector(conf){ }
    virtual void initialize(Configuration_ref conf) {
      m_do_bore= from_string(ProConfig::getTag(conf, getName(), "doBore", "0"),false);
      m_do_eore = from_string(ProConfig::getTag(conf, getName(), "doEore", "0"), false);
      m_eventsOfInterest = parsenumbers(ProConfig::getTag(conf, getName(), "events", ""));
    }
    virtual ReturnParam inspecktEvent(const Event&ev) {

      if (ev.IsBORE() && m_do_bore)
      {
        return ProcessorBase::sucess;
      }

      if (ev.IsEORE()&& m_do_eore)
      {
        return ProcessorBase::sucess;
      }
      if (m_eventsOfInterest.empty())
      {
        return ProcessorBase::sucess;
      }

      if (ev.GetEventNumber() > m_eventsOfInterest.back())
     {
       return ProcessorBase::stop;
     }
      

      if (std::find(m_eventsOfInterest.begin(), m_eventsOfInterest.end(),
        ev.GetEventNumber()) != m_eventsOfInterest.end())
      {
        return ProcessorBase::sucess;
      }
      return ProcessorBase::skip_event;
    }

    bool m_do_bore = false, m_do_eore=false;
    std::vector<unsigned> m_eventsOfInterest;
  };

}