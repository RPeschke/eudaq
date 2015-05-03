#ifndef factoryDefinitionen_h__
#define factoryDefinitionen_h__

#include "factory.hh"

#define registerBaseClassDef(BaseClass) namespace{\
                                         void dummy_register_function_##BaseClass(){\
                                           BaseClass::MainType mType{};\
                                           BaseClass::Parameter_ref pType{};\
                                           EUDAQ_Utilities::Factory<BaseClass>::Create(mType,pType );       \
                                           EUDAQ_Utilities::Factory<BaseClass>::GetTypes();          \
                                           EUDAQ_Utilities::Factory<BaseClass>::getInstance();\
                                           }}  \
                                     int EUDAQ_DUMMY_VARIABLE_DO_NOT_USE##BaseClass=0

namespace EUDAQ_Utilities{
  template <typename baseClassType>
  typename Factory<baseClassType>::MainType_V Factory<baseClassType>::GetTypes()
  {
    std::vector<MainType> result;
    for (auto& e : getInstance()) {
      result.push_back(e.first);
    }
    return result;
  }

  template <typename baseClassType>
  typename Factory<baseClassType>::u_pointer  Factory<baseClassType>::Create(const MainType & name, Parameter_ref params /*= ""*/)
  {
    auto it = getInstance().find(name);
    if (it == getInstance().end()) {
      EUDAQ_THROW("unknown class: <"+ name+">");
      return nullptr;
    }
    return (it->second)(params);

  }

  template <typename baseClassType>
  typename Factory<baseClassType>::map_t& Factory<baseClassType>::getInstance(){
    static map_t m;
    return m;
  }


//   template <typename baseClassType>
//   void Factory<baseClassType>::do_register(const MainType & name, factoryfunc func)
//   {
//   
//   }

}
#endif // factoryDefinitionen_h__
