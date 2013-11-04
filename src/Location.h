#ifndef LOCATION_H
#define LOCATION_H

#include "Engine.h"
#include "Segment.h"
#include "Typedef.h"
#include <vector>
#include "Error.h"

namespace Shipping {
	class Location : public Entity<Location> {
	public:
		typedef Fwk::Ptr<Location> Ptr;

		enum Type {
	  		customer_,
	  		port_,
	  		terminal_
	  	};

		static Type customer(){ return customer_; } 
		static Type port(){ return port_; }
		static Type terminal(){ return terminal_; }

		Type type(){ return type_; }

    SegmentPtr segment(int index){ return segments_[index]; }
    virtual void segmentIs(SegmentPtr segment){ segments_.push_back(segment); }
    void segmentDel(SegmentPtr segment){
    	segments_.erase(std::find(segments_.begin(), segments_.end(), segment));
    }
    void segmentDel(int index){
    	segments_.erase(segments_.begin() + index);
    }

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


	class Terminal : public Location {
	public:
		typedef Fwk::Ptr<Terminal> Ptr;

		static Ptr terminalNew(EntityName name, Segment::Type segmentType){
			Ptr ptr = new Terminal(name, segmentType);
      		ptr->deleteRef();
      		return ptr;
		}

		void segmentIs(SegmentPtr segment){ 
			if(segment->type() != segmentType_) return;
			segments_.push_back(segment);
		}

		Segment::Type segmentType(){ return segmentType_; }
		void segmentTypeIs(Segment::Type segmentType){
			if(segments_.size() > 0) throw new ValueOutOfBoundsException("can't change type of terminal with attached segments");
			segmentType_ = segmentType;
		}

	protected:
		Segment::Type segmentType_;

		Terminal(EntityName name, Segment::Type segmentType) : Location(name, terminal()), segmentType_(segmentType){}
	};

}

#endif
