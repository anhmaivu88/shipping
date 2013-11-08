#include <sstream>
#include <iomanip>

#include "Connectivity.h"
#include "Query.h"
#include "Location.h"


namespace Shipping {
	using namespace std;

	void Connectivity::queryIs(Query query){
		if(query_ == query) return;
		query_ = query;
		vector<Connectivity::Path> paths;
		Connectivity::Path empty(this, query.priority());
		if(query.type() == Query::Type::explore_){
			generateExplorePaths(paths, query.start(), empty);
		} else if(query.type() == Query::Type::connect_) {
			generateConnectPaths(paths, query.start(), empty);
		} else {
			result_ = "";
			return;
		}

		stringstream output;
		for(Connectivity::Path & p : paths){
			output << p.toString(query.type()) << endl;
		}
		result_ = output.str();
	}

	void Connectivity::generateExplorePaths(vector<Connectivity::Path> & paths, Location::Ptr loc, Path curPath){
		for(int i = 0; /***/; i++){
			Segment::Ptr seg = loc->segment(i);
			if(seg == NULL) break;
			Connectivity::Path newPath(curPath);
			if(newPath.push_back(seg) && 
				 newPath.cost() <= query_.cost() &&
				 newPath.distance() <= query_.distance() &&
				 newPath.time() <= query_.time()
			) {
				paths.push_back(newPath);
				generateExplorePaths(paths, seg->returnSegment()->source(), newPath);
			}
		}
	}

	void Connectivity::generateConnectPaths(vector<Connectivity::Path> & paths, Location::Ptr loc, Path curPath){
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
