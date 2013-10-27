#ifndef LOCATION_H
#define LOCATION_H

#include "Engine.h"
#include "Segment.h"
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

		Shipping::EntityName name(){ return name; }
		Segment segment(int index){ return segments_[index]; }

	protected:
		std::vector<Segment> segments_;
		EntityName name_;
		Type type_;

		Location(EntityName name, Type type):name_(name), type_(Type::customer_){}
	};

	class Customer : public Location {
	public:
		static Customer customerNew(EntityName name){ return Customer(name); }

	protected:
		Customer(EntityName name) : Location(name, Location::Type::customer_){}
	};


	class Port : public Location {
	public:
		static Port portNew(EntityName name){ return Port(name); }

	protected:
		Port(EntityName name) : Location(name, Location::Type::port_){}
	};


	class Terminal : public Location {
	public:
		static Terminal terminalNew(EntityName name){ return Terminal(name); }

	protected:
		Terminal(EntityName name) : Location(name, Location::Type::terminal_){}
	};

}

#endif
