#ifndef eventsOfIntrest_h__
#define eventsOfIntrest_h__


template <typename containerT>
class eventsOfIntresst{
public:
  
  typedef std::pair<containerT,size_t> elementOfIntresst;
   

  eventsOfIntresst(containerT Container, size_t Element=0) :m_container(Container, Element){

  }
  void addElement(elementOfIntresst ev){
    if (!IsElement(ev)){
      m_container.push_back(ev);
    }
 }
  void addElement(containerT Container, size_t Element=0){
    addElement(elementOfIntresst(Container, Element));
  }

  bool IsElement(const elementOfIntresst& ev){
   return std::find(m_container.begin(), m_container.end(), ev) != m_container.end()
  }
private:

  
  std::vector<elementOfIntresst> m_container;

};


#endif // eventsOfIntrest_h__
