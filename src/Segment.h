#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Mile.h"
#include "Error.h"
#include "Typedef.h"
#include "Entity.h"
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

    enum class Priority {
      NORMAL,
      EXPEDITED
    };

    static Priority normal(){ return Priority::NORMAL; }
    static Priority expedited(){ return Priority::EXPEDITED; }

    enum class Type {
      TRUCK,
      BOAT,
      PLANE
    };
    
    static Type truck(){ return Type::TRUCK; } 
    static Type boat(){ return Type::BOAT; }
    static Type plane(){ return Type::PLANE; }

    void sourceIs(const Fwk::Ptr<Location> source);
    void lengthIs(Mile length) { length_ = length; }
    void returnSegmentIs(Segment::Ptr segment);
    void difficultyIs(Difficulty difficulty) { difficulty_ = difficulty; }
    void priorityIs(Priority priority);

    Fwk::Ptr<Location> source() { return source_; }
    Mile length() { return length_; }
    Segment::Ptr returnSegment() { return returnSegment_; }
    Difficulty difficulty() { return difficulty_; }
    Priority priority() { return priority_; }
    Type type() { return type_; }

    static Segment::Ptr segmentNew(EntityName name, Type type) {
      Ptr ptr = new Segment(name, 0, 1.0, normal(), type);
      return ptr;
    }

    class Notifiee : public Fwk::PtrInterface<Notifiee> {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onPriority(Priority priority) {}
    protected:
      Notifiee(Segment *segment) : segment_(segment) {}
      Segment *segment() { return segment_; }
      Segment *segment_;
    };

    void notifieeAdd(Notifiee::Ptr notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee::Ptr notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }
    
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
    Fwk::Ptr<Location> source_;
    Segment::Ptr returnSegment_;
    Type type_;
    std::vector<Notifiee::Ptr> notifiees_;
  };

  class TruckSegment : public Segment {
    static Segment::Ptr truckSegmentNew(EntityName name) {
      return segmentNew(name, Segment::Type::TRUCK);
    }
  };

  class BoatSegment : public Segment {
    static Segment::Ptr boatSegmentNew(EntityName name) {
      return segmentNew(name, Segment::Type::BOAT);
    }
  };

  class PlaneSegment : public Segment {
    static Segment::Ptr planeSegmentNew(EntityName name) {
      return segmentNew(name, Segment::Type::PLANE);
    }
  };
}

#endif
