#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include "Engine.h"

namespace Shipping {

  class Connectivity : public Entity<Connectivity> {
  public:
  	typedef Fwk::Ptr<Connectivity> Ptr;

  	static Connectivity::Ptr connectivityNew(EntityName name, Engine::Ptr engine) {
      Ptr ptr = new Connectivity(name);
      ptr->deleteRef();

      return ptr;
    }
    
    queryIs(Query::Ptr query);

  private:
  	Engine::Ptr engine_;

    Connectivity(EntityName name): Entity(name), engine_(engine){}
  };
}

#endif
