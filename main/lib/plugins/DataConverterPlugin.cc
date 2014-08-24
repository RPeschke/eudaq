#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>
#define RegisterNewDataConverterType(type) void temp1(eudaq::DataConverterPlugin<type>::mainType &mT, eudaq::DataConverterPlugin<type>::subType &sT){\
                                                TemporaryFunction1<type>(mT, sT);\
                                            } \
                                            const int dummy1=0


namespace eudaq {

  template <typename containerT>
  DataConverterPlugin<containerT>::DataConverterPlugin(subType subtype)
    : m_eventtype(make_pair(Event::str2id("_RAW"), subtype))
  {
    //std::cout << "DEBUG: Registering DataConverterPlugin: " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;
    PluginManager<containerT>::GetInstance().RegisterPlugin(this);
  }
  template <typename containerT>
  DataConverterPlugin<containerT>::DataConverterPlugin(mainType type, subType subtype)
    : m_eventtype(make_pair(type, subtype))
  {
    //std::cout << "DEBUG: Registering DataConverterPlugin: " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;
    PluginManager<containerT>::GetInstance().RegisterPlugin(this);
  }



}//namespace eudaq


template <typename T>
void TemporaryFunction1(typename eudaq::DataConverterPlugin<T>::mainType &mT, typename eudaq::DataConverterPlugin<T>::subType &sT)
{
  eudaq::DataConverterPlugin<T> a(sT);
  eudaq::DataConverterPlugin<T> b(mT, sT);
}