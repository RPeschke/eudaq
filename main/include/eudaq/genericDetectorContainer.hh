#ifndef genericDetectorContainer_h__
#define genericDetectorContainer_h__

#include <memory>
#include <vector>


#define NOEXPECTATION 0

template <typename T>
class genericDetContainer{
  typedef T::t_id ELement_id;
  typedef std::shared_ptr<T> epointer; // element Pointer 

  genericDetContainer(size_t expected_Number_of_Elemtns = NOEXPECTATION) : m_container(expected_Number_of_Elemtns), (expected_Number_of_Elemtns){}
  void add_element(epointer newElement){m_container.push_back(newElement)}
  epointer getElement(size_t index){ return m_container.at(index); }
  epointer getElement(ELement_id id){
    auto ret=std::find(m_container.begin(), m_container.end(),
      [&id](epointer e){
        return e->getID() == id;
      }
    )
    if (ret!=m_container.end())
    {
        return *ret;
    }
      return nullptr;
  }



private:

  std::vector<epointer> m_container;
  size_t m_expected_elements;
};


#endif // genericDetectorContainer_h__
