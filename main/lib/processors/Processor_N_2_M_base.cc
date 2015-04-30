#include "eudaq/Processor_N_2_m_base.hh"


namespace eudaq{



	Processor_N_2_M_base::Processor_N_2_M_base(Parameter_ref name) :ProcessorBase(name)
	{

	}

	void Processor_N_2_M_base::AddProcessor(Processor_rp next, ConnectionName_ref name /*= ""*/)
	{
	    if (next)
		{
			m_external_map[name] = next;
		}
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

		
		auto nextProcessor = m_external_map.begin()->second->getProcessor(name);
		if (nextProcessor)
		{
			AddProcessorInternal(nextProcessor, name);
			return nextProcessor;
		}


		return nullptr;
	}
}