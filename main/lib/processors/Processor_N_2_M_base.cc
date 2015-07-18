#include "eudaq/Processor_N_2_M_base.hh"


namespace eudaq{



	Processor_N_2_M_base::Processor_N_2_M_base(Parameter_ref name) :ProcessorBase(name)
	{

	}

  Processor_rp Processor_N_2_M_base::getProcessor(ConnectionName_ref name /*= ""*/)
  {
    auto inputItt = m_interfaces.find(name);


    if (inputItt == m_interfaces.end())
    {

      
      m_interfaces[name] = CreateInterface(name, m_conf);

      auto nextProcessor = getNextProcessor(name);
      if (nextProcessor)
      {
        m_interfaces[name]->AddProcessor(nextProcessor, name);
      }

    }

    return m_interfaces[name].get();
  }



  void Processor_N_2_M_base::AddProcessor(Processor_rp next, ConnectionName_ref name /*= ""*/)
	{
	    if (next)
		{
			m_external_map[name] = next;
		}
	}


  void Processor_N_2_M_base::pushProducer(Processor_up processor)
  {
    m_interfaces[processor->getName()] = std::move(processor);
  }

  void Processor_N_2_M_base::clearProcessor()
  {
    m_interfaces.clear();
    m_internal_map.clear();
  }

  void Processor_N_2_M_base::AddProcessorInternal(Processor_rp next, ConnectionName_ref name)
	{
		if (next)
		{
			m_internal_map[name] = next;
		}
	}

	Processor_rp Processor_N_2_M_base::getNextProcessor(ConnectionName_t name /*= ""*/)
	{



		if (name.empty())
		{
			name = getName();
		}



		auto itt = m_external_map.find(name);
		if (itt!=m_external_map.end())
		{
			return itt->second;
		}

		auto ittinternal = m_internal_map.find(name);
		if (ittinternal != m_internal_map.end())
		{
			return ittinternal->second;
		}

    if (m_external_map.empty())
    {
      return nullptr;
    }
		
		auto nextProcessor = m_external_map.begin()->second->getProcessor(name);
		if (nextProcessor)
		{
			AddProcessorInternal(nextProcessor, name);
			return nextProcessor;
		}


		return nullptr;
	}

  void Processor_N_2_M_base::init()
  {
    

    for (auto &e:m_interfaces)
    {
      e.second->init();
    }

    initialize();
  }

  void Processor_N_2_M_base::end()
  {
    for (auto & e:m_interfaces)
    {
      e.second->end();
    }
    

    finish();
  }

}