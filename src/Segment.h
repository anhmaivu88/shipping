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
    class Difficulty : public Ordinal<Difficulty, float> {
    public:
      Difficulty(float num) : Ordinal<Difficulty, float>(num) {
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

    void sourceIs(const LocationPtr source);
    void lengthIs(Mile length) { length_ = length; }
    void returnSegmentIs(const Segment::Ptr segment);
    void difficultyIs(Difficulty difficulty) { difficulty_ = difficulty; }
    void expeditableIs(bool expeditable)  { expeditable_ = expeditable; }

    LocationPtr source() { return source_; }
    Mile length() { return length_; }
    Segment::Ptr returnSegment() { return returnSegment_; }
    Difficulty difficulty() { return difficulty_; }
    bool expeditable() { return expeditable_; }
    Type type() { return type_; }

  protected:
    static Segment::Ptr segmentNew(EntityName name, Type type) {
      Ptr ptr = new Segment(name, 0, 1.0, false, type);
      ptr->deleteRef();

      return ptr;
    }
    
    Segment(Shipping::EntityName name, 
            Mile length, 
            Difficulty difficulty, 
            bool expeditable, 
            Type type) : Entity(name), 
                         length_(length), 
                         difficulty_(difficulty), 
                         expeditable_(expeditable),
                         type_(type) {};

    Mile length_;
    Difficulty difficulty_;
    bool expeditable_;
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
