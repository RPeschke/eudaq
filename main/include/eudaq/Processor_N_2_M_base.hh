#ifndef Processor_N_2_m_base_hh__
#define Processor_N_2_m_base_hh__

#include "eudaq/ProcessorBase.hh"

namespace eudaq{


	class Processor_N_2_M_base :public ProcessorBase{
		
	public:
		Processor_N_2_M_base(Parameter_ref name);
		virtual ~Processor_N_2_M_base() {};
		
		virtual void init(Configuration_ref conf);
		virtual void initialize(Configuration_ref conf) = 0;
		virtual ReturnParam ProcessorEvent(event_sp ev) = 0;
		virtual void end() = 0;



		virtual Processor_rp getProcessor(ConnectionName_ref name = "") = 0;

		virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "");

		

		virtual std::string getName() = 0;
		virtual void print(std::ostream& os) = 0;
		virtual void pushProducer(Processor_up processor) = 0;


	protected:
		void AddProcessorInternal(Processor_rp next, ConnectionName_ref name);
		Processor_rp getNextProcessor(ConnectionName_t name = "");

	private:
		using Processor_map = std::map < ConnectionName_t, Processor_rp > ;
		
		
		Processor_map m_external_map , m_internal_map;

		


	};
}


#endif // Processor_N_2_m_base.hh_h__
