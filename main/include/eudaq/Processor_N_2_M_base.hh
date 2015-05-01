#ifndef Processor_N_2_m_base_hh__
#define Processor_N_2_m_base_hh__

#include "eudaq/ProcessorBase.hh"

namespace eudaq{


	class Processor_N_2_M_base :public ProcessorBase{
		
	public:
		Processor_N_2_M_base(Parameter_ref name);
		virtual ~Processor_N_2_M_base() {};
		
		virtual void initialize(Configuration_ref conf) = 0;
    virtual void finish() = 0;
		virtual ReturnParam ProcessorEvent(event_sp ev) = 0;




    virtual Processor_up CreateInterface(ConnectionName_ref name , Parameter_ref conf) = 0;


		

		virtual std::string getName() = 0;
		virtual void print(std::ostream& os) = 0;


    virtual void pushProducer(Processor_up processor) final;
    virtual void clearProducer() final;
    virtual void AddProcessor(Processor_rp next, ConnectionName_ref name = "") final;
		virtual Processor_rp getProcessor(ConnectionName_ref name = "") final;
    virtual void init(Configuration_ref conf) final;
		virtual void end() final;


	protected:
		void AddProcessorInternal(Processor_rp next, ConnectionName_ref name);
		Processor_rp getNextProcessor(ConnectionName_t name = "");

	private:
		using Processor_map = std::map < ConnectionName_t, Processor_rp > ;
    using Processor_up_map = std::map < ConnectionName_t, Processor_up >;
		
		Processor_map m_external_map , m_internal_map;

    Processor_up_map m_interfaces;
		


	};
}


#endif // Processor_N_2_m_base.hh_h__
