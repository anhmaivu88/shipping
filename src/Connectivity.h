#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include "Engine.h"

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
  	Engine::Ptr engine_;
  	Query query_;
  	QueryResult result_;

    Connectivity(EntityName name, Engine::Ptr engine): Entity(name), engine_(engine){}
    
    class Path;
    void generateExplorePaths(std::vector<Path> & paths, Location::Ptr loc, Path curPath);
    void generateConnectPaths(std::vector<Path> & paths, Location::Ptr loc, Path curPath);

    class Path {
    public:
      Path(const Path & other) :
        connectivity_(other.connectivity_),
        path_(other.path_),
        distance_(other.distance_),
        cost_(other.cost_),
        time_(other.time_),
        priority_(other.priority_)
      {}

      Path(Connectivity::Ptr connectivity, Segment::Priority priority) :  
        connectivity_(connectivity), 
        distance_(0), 
        cost_(0), 
        time_(0), 
        priority_(priority) 
      {}

      size_t size(){ return path_.size(); }
      bool push_back(Segment::Ptr segment);
      Segment::Ptr peek_back(){ return path_.size() > 0 ? path_[path_.size() - 1] : NULL; }

      string stringify(Query::Type type);

      Mile distance(){ return distance_; }
      Dollar cost(){ return cost_; }
      Hour time(){ return time_; }

    private:
      Connectivity::Ptr connectivity_;
      std::vector<Segment::Ptr> path_;
      Mile distance_;
      Dollar cost_;
      Hour time_;
      Segment::Priority priority_;

      string summary();
    };

   
  };
}

#endif
