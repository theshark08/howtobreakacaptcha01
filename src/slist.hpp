#include <list>

using namespace std;

template <class T>
class slist : public list<T>{
public:
  void push_sorted(T elem, bool (*comp)(T,T)){
    for(auto p = list<T>::begin(); p != list<T>::end(); p++)
      if(comp(elem, *p)){
        list<T>::insert(p, elem);
        return;
      }
    list<T>::push_back(elem);
  }
};
