#ifndef LOCATION_H
#define LOCATION_H

#include "Segment.h"
#include "Typedef.h"
#include "Entity.h"
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include "ActivityManager.h"
#include "Activity.h"
#include "TransferRate.h"
#include "ShipmentCount.h"
#include "PackageCount.h"
#include "PathData.h"
#include "Shipment.h"
#include <numeric>

namespace Shipping {
	class Connectivity;
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

    void shipmentAdd(Shipment::Ptr shipment) { 
      shipments_.push_back(shipment);
      for (auto notifiee : notifiees_) {
        notifiee->onShipmentAdd(shipment);
      }
    }

    void shipmentDel(Shipment::Ptr shipment) { 
      shipments_.erase(find(shipments_.begin(), shipments_.end(), shipment)); 
      for (auto notifiee : notifiees_) {
        notifiee->onShipmentDel(shipment);
      }
    }

    Shipment::Ptr shipment(int i) { return shipments_[i]; }
    EntityCount shipmentCount() { return shipments_.size(); }

    void routeIs(EntityName dest, PathData route){
    	std::vector<PathData> & paths = routes_[dest];
    	paths.push_back(route);
    }

    PathData route(EntityName dest){
    	auto it = routes_.find(dest);
    	if(it == routes_.end()){
        std::cerr << "No routes found" << std::endl;
        throw new InvalidAttributeException("No routes found");
      }
    	std::vector<PathData> paths = (*it).second;
      if (paths.size() == 0) {
        std::cerr << "No routes found" << std::endl;
        throw new InvalidAttributeException("No routes found");
      }
    	return paths[0]; // TODO: choose best path?
    }

    ShipmentCount shipmentsReceived() { return shipmentsReceived_; }
    Hour averageLatency() { 
      Hour average(0);
      for (Hour latency : latency_) { average += latency; }
      if (latency_.size() > 0) { average /= latency_.size(); }
      return average;
    } 
    Dollar totalCost() { return totalCost_; }

    class Notifiee : public Fwk::BaseNotifiee<Location> {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
        
      virtual void onShipmentAdd(Shipment::Ptr shipment) {}
      virtual void onShipmentDel(Shipment::Ptr shipment) {}
      virtual void onTransferRate() {}
      virtual void onShipmentSize() {}
      virtual void onDestination() {}

    protected:
        Notifiee(Location *loc) : Fwk::BaseNotifiee<Location>(loc), location_(loc) {}
        Location *location(){ return location_; }
        Location *location_;

    };

    void notifieeAdd(Notifiee::Ptr notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee::Ptr notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

    ~Location() {
      for (auto notifiee : notifiees_) {
        notifiee->notifierIs(NULL);
      }
    }

	protected:
    std::vector<Notifiee::Ptr> notifiees_;
    std::vector<SegmentPtr> segments_;
    std::map<EntityName, std::vector<PathData>> routes_; // Name of destination
    Type type_;

    ShipmentCount shipmentsReceived_;
    Dollar totalCost_;
    vector<Shipment::Ptr> shipments_;

    static Location::Ptr locationNew(EntityName name, Type type){
      Ptr ptr = new Location(name, type);
      return ptr;
    }

    Location(EntityName name, Type type): Entity(name), type_(type), shipmentsReceived_(0), totalCost_(0) { LocationShipmentMonitor::locationShipmentMonitorNew(Ptr(this)); }

  private:
    std::vector<Hour> latency_;
    class LocationShipmentMonitor : public Notifiee {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      static LocationShipmentMonitor::Ptr locationShipmentMonitorNew(Location::Ptr location) {
        return Ptr(new LocationShipmentMonitor(location.ptr()));
      }

      void onShipmentAdd(Shipment::Ptr shipment) {
        shipment->lastLocationIs(notifier());
        if (shipment->destination() == notifier()) {
          notifier()->shipmentDel(shipment);
          notifier()->recordDelivery(shipment);
        }
      }
            
    protected:
      LocationShipmentMonitor(Location *location) : Notifiee(location) { }
    };

    void shipmentsReceivedInc() { 
      std::cout << "Incrementing shipments received to " << shipmentsReceived_.value() + 1 << " on location " << name() << std::endl;
      shipmentsReceived_ += 1; 
    }
    void costInc(Dollar cost) { totalCost_ += cost; }
    void latencyAdd(Hour latency) { latency_.push_back(latency); }

    void recordDelivery(Shipment::Ptr shipment) {
      std::cout << "RECORDING DELIVERY" << std::endl;
      shipmentsReceivedInc();
      latencyAdd(shipment->transitTime());
      costInc(shipment->shippingCost());
    }
	};

	class Customer : public Location {
	public:
    typedef Fwk::Ptr<Customer> Ptr;

    class Notifiee : public Location::Notifiee {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
        
    protected:
        Notifiee(Customer *loc) : Location::Notifiee(loc), customer_(loc) {}
        Customer *customer(){ return customer_; }
        Customer *customer_;
    };

    static Customer::Ptr customerNew(EntityName name){ return new Customer(name); }

    void transferRateIs(TransferRate transferRate) { 
      transferRate_ = transferRate; 
      for (auto notifiee : notifiees_) { notifiee->onTransferRate(); }
    }
    void shipmentSizeIs(PackageCount shipmentSize) { 
      shipmentSize_ = shipmentSize; 
      for (auto notifiee : notifiees_) { notifiee->onShipmentSize(); }
    }
    void destinationIs(Location::Ptr destination){
      destination_ = destination;
      for (auto notifiee : notifiees_) { notifiee->onDestination(); }
    }

    TransferRate transferRate() { return transferRate_; }
    PackageCount shipmentSize() { return shipmentSize_; }
    Location::Ptr destination() { return destination_; }

  private:
    Customer(EntityName name): Location(name, Location::Type::CUSTOMER), transferRate_(0), shipmentSize_(0) {}
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
