
//
// Singleton
//


#include "stdlib.h"

#ifndef _SINGLETON_
#define _SINGLETON_


template <typename T>
class SSingleton
{

public:
  static T *getInstance ()
  {
    if ( _singleton == NULL )
      {
        _singleton = new T;
      }

    return (static_cast<T*> (_singleton));
  }

  static void kill ()
  {
    if ( _singleton != NULL )
      {
        delete _singleton;
        _singleton = NULL;
      }
  }

    private:
        static T *_singleton;
};

template <typename T>
T *SSingleton<T>::_singleton =  NULL;



#endif
