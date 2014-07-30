#include "eudaq/aidaMetaData.hh"
namespace eudaq{


  const uint64_t * const bit_mask()
  {
    static uint64_t* array = nullptr;
    if ( !array ) {
      array = new uint64_t[65];
      array[0] = 0;
      array[1] = 1;
      for ( int i = 2; i <= 64; i++ ) {
        array[i] = (array[i - 1] << 1) + 1;
      }
    }
    return array;
  }
}