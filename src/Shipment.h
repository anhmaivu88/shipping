#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "PtrInterface.h"
#include "Location.h"
#include "Segment.h"
#include "PackageCount.h"
#include "Notifiee.h"
#include "PathData.h"
#include <iostream>

namespace Shipping {
  class Shipment : public Fwk::PtrInterface<Shipment> {
  public:
    typedef Fwk::Ptr<Shipment> Ptr;

    static Shipment::Ptr shipmentNew(LocationPtr source, LocationPtr destination, PackageCount packageCount, PathData path, Hour startTime){
      Ptr ptr = new Shipment(source, destination, packageCount, path, startTime);
      std::cout << "Creating a shipment." << std::endl;
      return ptr;
    }

    class Notifiee : public Fwk::BaseNotifiee<Shipment> {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onArrive(LocationPtr arrivalLocation) {}
    };

    void notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

    void lastLocationIs(LocationPtr location) { lastLocation_ = location; for (auto notifiee : notifiees_) { notifiee->onArrive(location); } }
    LocationPtr lastLocation() { return lastLocation_; }

    LocationPtr source() { return source_; }
    LocationPtr destination() { return destination_; }
    PathData path() { return path_; }
    
    Hour transitTime() { return endTime_ - startTime_; }
    Dollar shippingCost() { return path().cost(); }
    PackageCount packageCount() { return packageCount_; }

    ~Shipment() {
      for (auto notifiee : notifiees_) {
        notifiee->notifierIs(NULL);
      }
    }

  private:
    typedef Fwk::Ptr<Location> LocationPtr;
    typedef Fwk::Ptr<Segment> SegmentPtr;

    Shipment(LocationPtr source, LocationPtr destination, PackageCount packageCount, PathData path, Hour startTime) :
      source_(source), 
      destination_(destination), 
      currentSegment_(NULL), 
      startTime_(startTime),
      endTime_(startTime),
      packageCount_(packageCount), 
      path_(path) 
      {}

    LocationPtr lastLocation_;
    LocationPtr source_;
    LocationPtr destination_;
    SegmentPtr currentSegment_;
    Hour startTime_;
    Hour endTime_;
    PackageCount packageCount_;
    std::vector<Notifiee *> notifiees_;
    PathData path_;
  };
}

#endif
