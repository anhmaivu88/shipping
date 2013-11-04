#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Mile.h"
#include "Error.h"
#include "Typedef.h"
#include "Location.h"

namespace Shipping {
  class Segment : public Entity<Segment> {
  public:
    typedef Fwk::Ptr<Segment> Ptr;
    class Difficulty : public Ordinal<Difficulty, double> {
    public:
    Difficulty(double num) : Ordinal<Difficulty, double>(num) {
        if(num < 1.0 || num > 5.0) {
          throw new ValueOutOfBoundsException("difficulty is not in 1-5 range");
        }
      }
    };

    enum Priority {
      normal_,
      expedited_
    };

    static Priority normal(){ return normal_; }
    static Priority expedited(){ return expedited_; }

    enum Type {
      truck_,
      boat_,
      plane_
    };
    
    static Type truck(){ return truck_; } 
    static Type boat(){ return boat_; }
    static Type plane(){ return plane_; }

    void sourceIs(const LocationPtr source);
    void lengthIs(Mile length) { length_ = length; }
    void returnSegmentIs(const Segment::Ptr segment);
    void difficultyIs(Difficulty difficulty) { difficulty_ = difficulty; }
    void priorityIs(Priority priority)  { priority_ = priority; }

    LocationPtr source() { return source_; }
    Mile length() { return length_; }
    Segment::Ptr returnSegment() { return returnSegment_; }
    Difficulty difficulty() { return difficulty_; }
    Priority priority() { return priority_; }
    Type type() { return type_; }

    static Segment::Ptr segmentNew(EntityName name, Type type) {
      Ptr ptr = new Segment(name, 0, 1.0, normal(), type);
      return ptr;
    }
    
  protected:
    
    
  Segment(Shipping::EntityName name, 
          Mile length, 
          Difficulty difficulty, 
          Priority priority, 
          Type type) : Entity(name), 
      length_(length), 
      difficulty_(difficulty), 
      priority_(priority),
      type_(type) {};

    Mile length_;
    Difficulty difficulty_;
    Priority priority_;
    LocationPtr source_;
    Segment::Ptr returnSegment_;
    Type type_;
  };

    class TruckSegment : public Segment {
      static Segment::Ptr truckSegmentNew(EntityName name) {
        return segmentNew(name, Segment::truck_);
      }
    };

    class BoatSegment : public Segment {
      static Segment::Ptr boatSegmentNew(EntityName name) {
        return segmentNew(name, Segment::boat_);
      }
    };

    class PlaneSegment : public Segment {
      static Segment::Ptr planeSegmentNew(EntityName name) {
        return segmentNew(name, Segment::plane_);
      }
    };
}

#endif
