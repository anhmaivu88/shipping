#ifndef LOCATION_H
#define LOCATION_H

#include "Engine.h"
#include "Segment.h"
#include "Typedef.h"
#include <vector>

namespace Shipping {
	class Location : public Entity<Location> {
	public:
		typedef Fwk::Ptr<Location> Ptr;

		enum Type {
			customer_,
			port_,
			terminal_
		};

		Type customer(){ return customer_; } 
		Type port(){ return port_; }
		Type terminal(){ return terminal_; }

		Type type(){ return type_; }

    SegmentPtr segment(int index){ return segments_[index]; }
    virtual void segmentIs(SegmentPtr segment){ segments_.push_back(segment); }

	protected:
		std::vector<SegmentPtr> segments_;
		Type type_;

		static Location::Ptr locationNew(EntityName name, Type type){
			Ptr ptr = new Location(name, type);
      ptr->deleteRef();
      return ptr;
		}

		Location(EntityName name, Type type): Entity(name), type_(type){}
	};

	class Customer : public Location {
	public:
		static Location::Ptr customerNew(EntityName name){ return locationNew(name, customer_); }

	protected:
		// Customer(EntityName name) : Location(name, Location::customer_){}
	};


	class Port : public Location {
	public:
		static Location::Ptr portNew(EntityName name){ return locationNew(name, port_); }

	protected:
		// Port(EntityName name) : Location(name, Location::port_){}
	};


	class Terminal : public Location {
	public:
		static Location::Ptr terminalNew(EntityName name){
			Ptr ptr = new Terminal(name);
      ptr->deleteRef();
      return ptr;
		}

		void segmentIs(SegmentPtr segment){ 
			if(segments_.size() > 0 && segment->type() != segments_[0]->type()) return;
			segments_.push_back(segment);
		}

	protected:
		Terminal(EntityName name) : Location(name, terminal_){}
	};

}

#endif
