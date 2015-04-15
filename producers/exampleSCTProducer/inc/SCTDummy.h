
#ifndef SCTDummy_h__
#define SCTDummy_h__

#include <fstream>
#include <string>
#include <queue>


class SCTDummy{
public
  SCTDummy();
  ~SCTDummy();

  void openfile(stdstring& name);

  void setReadoutSpeed(int speed);
  int getTimestamp();

  bool  readLine();

  bool hasData();
  static const size_t size = 256;
  bool data[size];
private
  std::ifstream in;
  int _readoutSpeed = 1, m_first = 1,m_end=0;
  bool nm_newData = false;

  std::queue < std::string > m_data_vec;
};
#endif // SCTDummy_h__
