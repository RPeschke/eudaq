#ifndef ProcessorFileWriter_h__
#define ProcessorFileWriter_h__
#include "Processor_inspector.hh"

namespace eudaq {

class FileWriter;
class DLLEXPORT  ProcessorFileWriter :public Processor_Inspector {
public:
  ProcessorFileWriter(const std::string & name, const std::string & params /*= ""*/);

  ProcessorFileWriter();
  void init() override;
  void end() override;
  virtual ReturnParam inspecktEvent(const Event&, ConnectionName_ref con);

private:
  std::unique_ptr<FileWriter> m_write;
  bool m_first = true;
  std::string m_name, m_params;
  bool m_default = false;
};
}


#endif // ProcessorFileWriter_h__
