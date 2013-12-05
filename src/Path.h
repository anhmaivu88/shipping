#ifndef PATH_H
#define PATH_H

#include "PathData.h"
#include "Engine.h"

namespace Shipping {
  class Path {
  public:
    Path(const Path & other) :
      engine_(other.engine_),
      path_(other.path_),
      distance_(other.distance_),
      cost_(other.cost_),
      time_(other.time_),
      priority_(other.priority_)
    {}

    Path(Engine::Ptr engine, Segment::Priority priority) :  
      engine_(engine), 
      distance_(0), 
      cost_(0), 
      time_(0), 
      priority_(priority) 
    {}

    bool segmentIs(Segment::Ptr segment);
    Segment::Ptr segment(size_t index){ return path_.size() > index ? path_[index] : NULL; }

    PathData data(){ return PathData(path_, distance_, cost_, time_); }

    string summary(Query::Type type);
    size_t size(){ return path_.size(); }
    Mile distance(){ return distance_; }
    Dollar cost(){ return cost_; }
    Hour time(){ return time_; }

  private:
    Engine::Ptr engine_;
    std::vector<Segment::Ptr> path_;
    Mile distance_;
    Dollar cost_;
    Hour time_;
    Segment::Priority priority_;

    string connectPreamble();
  };
}

#endif