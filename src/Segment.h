#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include <algorithm>
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Mile.h"
#include "Error.h"
#include "Typedef.h"
#include "Entity.h"
#include "Location.h"
#include "ShipmentCount.h"
#include "Shipment.h"
#include "Notifiee.h"
#include <iostream>

namespace Shipping {
  class Segment : public Entity<Segment> {
  public:
    typedef Fwk::Ptr<Segment> Ptr;
    class Difficulty : public Ordinal<Difficulty, double> {
    public:
      Difficulty(double num) : Ordinal<Difficulty, double>(num) {
        if(num < 1.0 || num > 5.0) {
          std::cerr << "Difficulty must be between 1.0 and 5.0" << std::endl;
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
    void capacityIs(ShipmentCount capacity) { shipmentCapacity_ = capacity; }

    Fwk::Ptr<Location> source() { return source_; }
    Mile length() { return length_; }
    Segment::Ptr returnSegment() { return returnSegment_; }
    Difficulty difficulty() { return difficulty_; }
    Priority priority() { return priority_; }
    Type type() { return type_; }
    ShipmentCount capacity() { return shipmentCapacity_; }

    void shipmentAdd(Shipment::Ptr shipment);
    void shipmentDel(Shipment::Ptr shipment);
    Shipment::Ptr shipment(int i) { return shipments_[i]; }
    ShipmentCount shipmentCount() { return shipments_.size(); }


    ShipmentCount shipmentsReceived() { return shipmentsReceived_; }
    ShipmentCount shipmentsRefused() { return shipmentsRefused_; }

    static Segment::Ptr segmentNew(EntityName name, Type type) {
      Ptr ptr = new Segment(name, 0, 1.0, normal(), type);
      return ptr;
    }

    class Notifiee : public Fwk::BaseNotifiee<Segment> {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onPriority(Priority priority) {}
      virtual void onShipmentAdd(Shipment::Ptr shipment) {}
      virtual void onShipmentDel(Shipment::Ptr shipment) {}
    protected:
      Notifiee(Segment *segment) : BaseNotifiee(segment), segment_(segment) {}
      Segment *segment() { return segment_; }
      Segment *segment_;

      /* Cannot be deleted until the segment is deleted, so we 
         don't need to do any housekeeping. */
      ~Notifiee() {}
    };

    void notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { notifiees_.erase(std::find(notifiees_.begin(), notifiees_.end(), notifiee)); }

    ~Segment() {
      for (auto notifiee : notifiees_) {
        notifiee->notifierIs(NULL);
      }
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
                         type_(type),
                         shipmentCapacity_(10),
                         shipmentsReceived_(0),
                         shipmentsRefused_(0) {};

    Mile length_;
    Difficulty difficulty_;
    Priority priority_;
    Fwk::Ptr<Location> source_;
    Segment::Ptr returnSegment_;
    Type type_;
    /* We intentionally maintain a strong reference to notifiees because
       the engine is not interested in each of the individual segment 
       reactors. Furthermore, the engine will never be deleted before
       a segment. */
    std::vector<Notifiee::Ptr> notifiees_;
    std::vector<Shipment::Ptr> shipments_;
    ShipmentCount shipmentCapacity_;

    ShipmentCount shipmentsReceived_;
    ShipmentCount shipmentsRefused_;
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
