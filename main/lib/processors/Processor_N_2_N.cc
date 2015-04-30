
#include "eudaq/Processor_N_2_N.hh"


namespace eudaq{
	void Processor_N_2_N::init(Configuration_ref conf)
  {
    for (auto&e:m_Processors)
    {
      e.second->AddProcessor(getNextProcessor(e.first), e.first);
      e.second->init(conf);
    }
    Initilize();
  }

  void Processor_N_2_N::end()
  {
    for (auto&e : m_Processors)
    {
      e.second->end();
    }
    multiEnd();
  }



  Processor_rp Processor_N_2_N::getProcessor(ConnectionName_ref name /*= ""*/)
  {
    auto inputItt =m_Processors.find(name);


    if (inputItt == m_Processors.end())
    {
      m_Processors[name] = CreateProcessor(name,m_conf);

	  auto nextProcessor = getNextProcessor(name);
      if (nextProcessor)
      {
        m_Processors[name]->AddProcessor(nextProcessor, name);
      }

    }

    return m_Processors[name].get();
  }


  void Processor_N_2_N::pushProducer(Processor_up processor)
  {
   m_Processors[processor->getName()]=std::move(processor);
  }

 


  Processor_N_2_N::Processor_N_2_N(Parameter_ref conf) :Processor_N_2_M_base(conf)
  {

  }

}