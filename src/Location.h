#ifndef LOCATION_H
#define LOCATION_H

#include "Segment.h"
#include "Typedef.h"
#include "Entity.h"
#include <vector>

namespace Shipping {
	class Location : public Entity<Location> {
	public:
		typedef Fwk::Ptr<Location> Ptr;

		enum class Type {
			CUSTOMER,
      PORT,
			TERMINAL
		};

		static Type customer(){ return Type::CUSTOMER; } 
		static Type port(){ return Type::PORT; }
		static Type terminal(){ return Type::TERMINAL; }

		Type type(){ return type_; }

    SegmentPtr segment(int index){ return segments_[index]; }
    virtual void segmentIs(int index, SegmentPtr segment){ segments_[index] = segment; }
    virtual void segmentAdd(SegmentPtr segment) { segments_.push_back(segment); }
    virtual void segmentDel(SegmentPtr segment) { segments_.erase(find(segments_.begin(), segments_.end(), segment)); }

	protected:
		std::vector<SegmentPtr> segments_;
		Type type_;

		static Location::Ptr locationNew(EntityName name, Type type){
			Ptr ptr = new Location(name, type);
		  return ptr;
		}

  Location(EntityName name, Type type): Entity(name), type_(type){}
	};

	class Customer : public Location {
	public:
		static Location::Ptr customerNew(EntityName name){ return locationNew(name, customer()); }

	protected:
		// Customer(EntityName name) : Location(name, Location::customer_){}
	};


	class Port : public Location {
	public:
		static Location::Ptr portNew(EntityName name){ return locationNew(name, port()); }

	protected:
		// Port(EntityName name) : Location(name, Location::port_){}
	};
}

#endif
