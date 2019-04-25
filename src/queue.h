#ifndef _QUEUE_H_
#define	_QUEUE_H_ 1


// #include "debug.h"

#include <ext/slist>
#include <iterator>
#include <iostream>
#include <string>
#include <algorithm>


using namespace std;
using namespace __gnu_cxx;


/* LIFO
   + slice operation,

   Memory/ownerhip:
   the implementing container(list) handles pointers to objects.
   The objects are owned by the application!
   if a Reference is push()-ed, we make a Clone! But we never delete the objects.
   pop() returns the pointer!
*/
template <typename T>
class my_queue
{
private:
    const char* m_name;     // for debug string
  slist<T*> list;
  typename slist<T*>::iterator last_node;

public:
    const char* get_name()
        {
            return m_name?:"(unknown)";// .c_str();
        }
    // take ownership
    void set_name(const char* name)
        {
            m_name = name;
        }


    /* move the content of appendix to the END of this queue
     *      this      appendix    this        appendix
     *     xxxxxxx   yyyyy   ->   xxxxyyyy       (empty)
     */
    void slice (my_queue<T>& appendix);

    ~my_queue()
        {
            if (m_name)
            {
                m_name = NULL;
            }
        }
    my_queue<T>(const char* name = NULL) : m_name(name)
        {
            DB(("constructor\n"));
            last_node = list.end();
        };

    void swap (my_queue<T>& peer)
        {
            typename slist<T*>::iterator temp;
            temp = last_node;

            list.swap(peer.list);

            // iter_swap(last_node,peer.last_node);
            if (list.empty())
                last_node = list.begin();
            else {
                last_node = peer.last_node;
            }

            if (peer.list.empty())
                peer.last_node = peer.list.begin();
            else
                peer.last_node = temp;
        }
  int length() const
  {
    return list.size();
  }

  bool empty() const
  {
    return (list.empty());
  }

  const T* front () const
  {
    return list.front();
  }

  T* pop();                    // top_and_pop()

  void push(T* element);

  // fixme: move-?
  void push(const T& value)   // we clone the value!
  {
    T* clone = new T(value);
    push(clone);
  }



};




template<typename T>
void my_queue<T>::push (T* value)
{
#if DEBUG > 1
    DB(("%s: %s: now %d + 1\n", __FUNCTION__, get_name(), length()));
#endif
    if (!empty ()) {
        last_node = list.insert_after(last_node, value);
    } else {
        list.push_front(value);
        last_node = list.begin();
    }
}


template<typename T>
T* my_queue<T>::pop ()
{
    T* pointer = list.front();

    list.pop_front();
    // invalidate iterators
    if (list.empty())
        last_node = list.begin();
    return pointer;
}


template<typename T>
void my_queue<T>::slice (my_queue<T> &suffix)
{
#if DEBUG > 1
    DB(("%s: %s: appending/moving all from %s:\n", __FUNCTION__, get_name(),
        suffix.get_name()));
#endif

    if (! suffix.list.empty())
    {
        list.splice_after(last_node,
                          suffix.list);
        last_node=suffix.last_node;
    }
#if DEBUG > 1
    DB(("%s now has %d\n", get_name(), length()));
    DB(("%s now has %d\n", suffix.get_name(), suffix.length()));
#endif
}


#endif
