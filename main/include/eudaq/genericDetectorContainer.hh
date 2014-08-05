#ifndef genericDetectorContainer_h__
#define genericDetectorContainer_h__

#include <memory>
#include <vector>


#define NOEXPECTATION 0
namespace eudaq{

  template <typename T>
  class genericDetContainer{
  public:
    typedef typename T::t_id ELement_id;
    typedef std::shared_ptr<T> epointer; // element Pointer 

    genericDetContainer(T& t){
      
    }
    genericDetContainer(size_t expected_Number_of_Elemtns = NOEXPECTATION) :
      m_container(expected_Number_of_Elemtns),
      m_expected_elements(expected_Number_of_Elemtns)
    {
     
    }
    genericDetContainer(unsigned runnumber, unsigned eventnumber, uint64_t timestamp):
      m_runnumber(runnumber),
      m_eventnumber(eventnumber),
      m_timestamp(timestamp)
    {
      
    }

    void add_element(epointer newElement){ m_container.push_back(newElement); }
    epointer getElement(size_t index){ return m_container.at(index); }
    epointer getElement(ELement_id id){
      for (auto e : m_container)
      {
        if (e->getID() == id)
        {
          return e;
        }
      }

      return nullptr;
    }

    size_t Size(){
      return m_container.size();
    }

  private:

    std::vector<epointer> m_container;
    size_t m_expected_elements;
    unsigned m_runnumber = 0;
    unsigned m_eventnumber = 0;
    uint64_t m_timestamp = 0;

  };
}

#endif // genericDetectorContainer_h__
