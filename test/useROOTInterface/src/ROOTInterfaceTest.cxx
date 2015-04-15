class TSignals;
class ROOTProducer;

class SCTDummy{
public:
  SCTDummy();
  ~SCTDummy();
  void openfile(const char* name);
  void openfile(std::string& name);

  void setReadoutSpeed(int speed);
  int getTimestamp();

  bool  readLine();

  bool hasData();
  void readOutLoop();
  static const size_t size = 256;
  bool data[size];

  TSignals *eSignals;

private:
  std::ifstream in;
  int _readoutSpeed = 1;

  std::vector < std::string > m_data_vec;
};

