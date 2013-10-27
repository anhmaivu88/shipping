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
			customer_ = 1,
			port_ = 2,
			terminal_ = 3
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

		Location(EntityName name, Type type): Entity(name), type_(customer_){}
	};

	class Customer : public Location {
	public:
		static Customer customerNew(EntityName name){ return Customer(name); }

	protected:
		Customer(EntityName name) : Location(name, Location::customer_){}
	};


	class Port : public Location {
	public:
		static Port portNew(EntityName name){ return Port(name); }

	protected:
		Port(EntityName name) : Location(name, Location::port_){}
	};


	class Terminal : public Location {
	public:
		static Terminal terminalNew(EntityName name){ return Terminal(name); }
		void segmentIs(SegmentPtr segment){ 
			if(_segments.size() > 0 && segment->type() != _segments[0]->type()) return;
			_segments.push_back(segment);
		}

	protected:
		Terminal(EntityName name) : Location(name, Location::terminal_){}
	};

}

#endif
