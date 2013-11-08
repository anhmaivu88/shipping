#include <sstream>
#include <iomanip>

#include "Connectivity.h"
#include "Query.h"
#include "Location.h"


namespace Shipping {
	using namespace std;

	void Connectivity::queryIs(Query query){
		stringstream output;
		vector<Connectivity::Path> paths;
		Connectivity::Path empty(this, query.priority());
		generateExplorePaths(paths, query.start(), empty);
	}

	void Connectivity::generateExplorePaths(vector<Connectivity::Path> & paths, Location::Ptr loc, Path curPath){
		//curPath.push_back()
	}

	bool Connectivity::Path::push_back(Segment::Ptr segment){
    Segment::Ptr comp = segment->returnSegment();
    if(comp == NULL) return false;

    Location::Ptr dest = comp->source();
    for(Segment::Ptr pathSeg : path_){

    // already visited; forbid cycles
      if(pathSeg->source() == dest) return false;
    }

    Fleet::Ptr fleet = connectivity_->engine_->fleet(segment->type());
    MilesPerHour speed = fleet->speed();
    DollarsPerMile cost = fleet->cost();
    if(priority_ == Segment::Priority::EXPEDITED){
      if(segment->priority() == Segment::Priority::EXPEDITED){
        speed *= 1.3;
        cost *= 1.5;
      } else return false;
    }
    Mile len = segment->length();
    Dollar costForSegment = len * cost;
    Hour timeForSegment = len / speed;

    distance_ += len;
    cost_ += costForSegment;
    time_ += timeForSegment;
    path_.push_back(segment);
    return true;
  }

	string Connectivity::Path::toString(Query::Type type){
		stringstream ss;
		ss << setprecision(2);
		if(type == Query::Type::connect_) ss << summary() << " ";
		for(Segment::Ptr fwd : path_){
			Segment::Ptr bak = fwd->returnSegment();
			ss << fwd->source()->name() << "(" << fwd->name() << ":" << fwd->length().value() << ":" << bak->name() << ") ";
		}
		ss << path_[path_.size() - 1]->returnSegment()->source()->name();
		return ss.str();
	}

	string Connectivity::Path::summary(){
		stringstream ss;
		ss << setprecision(2) << cost_.value() << " " << time_.value() << " ";
		if(priority_ == Segment::Priority::EXPEDITED) ss << "yes";
		else ss << "no";
		ss << ";";
		return ss.str();
	}

}
