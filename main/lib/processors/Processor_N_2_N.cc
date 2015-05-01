
#include "eudaq/Processor_N_2_N.hh"


namespace eudaq{






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




 


  Processor_N_2_N::Processor_N_2_N(Parameter_ref conf) :Processor_N_2_M_base(conf)
  {

  }

}