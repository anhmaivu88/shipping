#ifndef ENTITY_H
#define ENTITY_H

#include "Ptr.h"
#include "PtrInterface.h"
#include <string>

namespace Shipping {
  typedef string EntityName;

  template <class T>
  class Entity: public Fwk::PtrInterface<T> {
  public: 
    virtual Shipping::EntityName name() { return name_; }


  private:
    Shipping::EntityName name_;
  };
}

#endif
