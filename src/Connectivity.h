#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include "Engine.h"
#include "Path.h"

namespace Shipping {

  class Connectivity : public Entity<Connectivity> {
  public:
    typedef Fwk::Ptr<Connectivity> Ptr;
    typedef string QueryResult;

    static Connectivity::Ptr connectivityNew(EntityName name, Engine::Ptr engine) {
      Ptr ptr = new Connectivity(name, engine);

      return ptr;
    }
    
    void queryIs(Query query); // IMPL: runs query on engine
    Query query(){ return query_; }

    QueryResult result(){ return result_; }

  private:
    friend class Path;
    Engine::Ptr engine_;
    Query query_;
    QueryResult result_;

    Connectivity(EntityName name, Engine::Ptr engine): Entity(name), engine_(engine){}
    
    void generateExplorePaths(std::vector<Path> & paths, Location::Ptr loc, Path curPath);
    void generateConnectPaths(std::vector<Path> & paths, Location::Ptr loc, Path curPath);
   
  };
}

#endif
