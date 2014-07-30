#ifndef aidaMetaData_h__
#define aidaMetaData_h__
#include <stdint.h>
#include <vector>


// meta data
#define TLU_BITS   1
#define TLU_SHIFT 63

#define ENTRY_TYPE_BITS   4
#define ENTRY_TYPE_SHIFT 59

#define COUNTER_BITS  59
#define COUNTER_SHIFT  0

#define getBits(FIELD,from)	getBitsTemplate<FIELD ## _SHIFT, FIELD ## _BITS>(from)
#define setBits(FIELD,dst, val)	setBitsTemplate<FIELD ## _SHIFT, FIELD ## _BITS>(dst,val)

namespace eudaq{

  const uint64_t * const bit_mask();


  template <int shift, int bits> static uint64_t getBitsTemplate( uint64_t from ) {
    return shift > 0 ? (from >> shift) & bit_mask()[bits] : from & bit_mask()[bits];
  }

  template <int shift, int bits> static void setBitsTemplate( uint64_t& dest, uint64_t val ) {
    dest |= shift > 0 ? (val & bit_mask()[bits]) << shift : val & bit_mask()[bits];
  }


  static inline int GetType( uint64_t meta_data ) { return getBits(ENTRY_TYPE, meta_data ); };
  static inline void SetType( uint64_t& meta_data, int type ) { setBits(ENTRY_TYPE,  meta_data, type ); };
  static inline bool IsTLUBitSet( uint64_t meta_data ) { return getBits(TLU, meta_data ); };
  static inline uint64_t GetCounter( uint64_t meta_data ) { return getBits(COUNTER, meta_data ); };
  static inline void SetCounter( uint64_t& meta_data, uint64_t data ) { setBits(COUNTER, meta_data, data ); };




  class MetaData {
  public:
    std::vector<uint64_t> v;

    void add( bool tlu, int type, uint64_t data ) {
      uint64_t meta_data = 0;
      SetType( meta_data, type );
      SetCounter( meta_data, data );
      v.push_back( meta_data );
    }
  };


}

#endif // aidaMetaData_h__