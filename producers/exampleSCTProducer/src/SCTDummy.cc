#include "SCTDummy.h"
#include "eudaq\Utils.hh"



SCTDummy::SCTDummy()
{
  for (int i = 0; i < size; ++i)
  {
    data[i] = 0;
  }
}

bool SCTDummy::hasData()
{
  eudaq::mSleep(_readoutSpeed);
  return !m_data_vec.empty();
}

bool SCTDummy::readLine()
{
  m_data_vec.pop();
  auto line = m_data_vec.front();
  size_t j = 0;
  for (auto& e : line)
  {
    if (j>=size)
    {
      break;
    }
    if (e == '1')
    {
      data[j] = true;
    }
    else
    {
      data[j] = false;
    }
    ++j;
  }


  m_data_vec.pop();

  m_first = false;

  return true;
}

void SCTDummy::setReadoutSpeed(int speed)
{
  _readoutSpeed = speed;
}

void SCTDummy::openfile(std::string& name)
{
  in.open(name);

  std::string line;
  while (!in.eof())
  {
    std::getline(in, line);
    if (line.empty())
    {
      continue;
    }
    m_data_vec.push(line);
  }
  in.close();
}

SCTDummy::~SCTDummy()
{
  if (in)
  {
    in.close();
  }
}

int SCTDummy::getTimestamp()
{
  return 0;
}

