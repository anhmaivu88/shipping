#ifndef LOCATION_H
#define LOCATION_H

#include "Segment.h"
#include "Typedef.h"
#include "Entity.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "TransferRate.h"
#include "ShipmentCount.h"
#include "PackageCount.h"

namespace Shipping {
	class Location : public Entity<Location> {
	public:
		typedef Fwk::Ptr<Location> Ptr;

		enum class Type {
			CUSTOMER,
      PORT,
      BOAT_TERMINAL,
      TRUCK_TERMINAL,
      PLANE_TERMINAL
		};

		static Type customer(){ return Type::CUSTOMER; } 
		static Type port(){ return Type::PORT; }
		static Type boatTerminal(){ return Type::BOAT_TERMINAL; }
    static Type planeTerminal(){ return Type::PLANE_TERMINAL; }
    static Type truckTerminal(){ return Type::TRUCK_TERMINAL; }

		Type type(){ return type_; }

    SegmentPtr segment(int index){ if (index < segments_.size()) { return segments_[index]; } else { return NULL; } }
    virtual void segmentIs(int index, SegmentPtr segment){ segments_[index] = segment; }
    virtual void segmentAdd(SegmentPtr segment) { segments_.push_back(segment); }
    virtual void segmentDel(SegmentPtr segment) { segments_.erase(std::find(segments_.begin(), segments_.end(), segment)); }
    EntityCount segmentCount() { return segments_.size(); }

    ShipmentCount shipmentsReceived() { return shipmentsReceived_; }
    Hour averageLatency() { return averageLatency_; }
    Dollar totalCost() { return totalCost_; }

	protected:
		std::vector<SegmentPtr> segments_;
		Type type_;

    ShipmentCount shipmentsReceived_;
    Hour averageLatency_;
    Dollar totalCost_;

		static Location::Ptr locationNew(EntityName name, Type type){
			Ptr ptr = new Location(name, type);
		  return ptr;
		}

    Location(EntityName name, Type type): Entity(name), type_(type), shipmentsReceived_(0), averageLatency_(0), totalCost_(0) {}
	};

	class Customer : public Location {
	public:
		static Location::Ptr customerNew(EntityName name){ return locationNew(name, customer()); }
    void transferRateIs(TransferRate transferRate) { transferRate_ = transferRate; }
    void shipmentSizeIs(PackageCount shipmentSize) { shipmentSize_ = shipmentSize; }
    void destinationis(Location::Ptr);

  private:
    TransferRate transferRate_;
    PackageCount shipmentSize_;
    Location::Ptr destination_;
	};


	class Port : public Location {
	public:
		static Location::Ptr portNew(EntityName name){ return locationNew(name, port()); }
	};
}

#endif
