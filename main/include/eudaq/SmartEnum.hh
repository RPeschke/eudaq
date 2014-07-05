/*
 * SmartEnum.hh
 *
 *  Created on: Jun 27, 2014
 *      Author: behrens
 */

#ifndef SMARTENUM_HH_
#define SMARTENUM_HH_

using std::map;
using std::string;

class SmartEnumBase {
};

#define DECLARE_ENUM_CLASS(ClassName, ...)\
class ClassName : public SmartEnumBase {\
  public:\
    enum Value { __VA_ARGS__ } value;\
    const string& toString() { return toString( value ); }\
    static const string& toString( Value& v ) {\
  	  return getMap()[v]; \
    }\
  private:\
    static map<Value,string>& getMap() {\
      static const string str[] = { #__VA_ARGS__ };\
      static const Value val[] = {__VA_ARGS__};\
      static map<Value,string> map; \
      if ( map.empty() )\
        for ( int i = 0; i < sizeof(val) / sizeof(Value); i++ )\
          map[val[i]] = str[i];\
    }\
};



#endif /* SMARTENUM_HH_ */
