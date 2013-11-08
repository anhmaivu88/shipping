#include <sstream>
#include <iomanip>

#include "Connectivity.h"
#include "Query.h"
#include "Location.h"


namespace Shipping {
	using namespace std;

	void Connectivity::queryIs(Query query){
		query_ = query;
		vector<Connectivity::Path> paths;
		if(query_.type() == Query::Type::explore_){
		  Connectivity::Path empty(this, query.priority());
			generateExplorePaths(paths, query.start(), empty);
		} else if(query_.type() == Query::Type::connect_) {
		  Connectivity::Path emptyNormal(this, Segment::Priority::NORMAL);
			generateConnectPaths(paths, query.start(), emptyNormal);
		  Connectivity::Path emptyExpedited(this, Segment::Priority::EXPEDITED);
			generateConnectPaths(paths, query.start(), emptyExpedited);
		} else {
			result_ = "";
			return;
		}

		stringstream output;
		for(Connectivity::Path & p : paths){
			output << p.stringify(query.type()) << endl;
		}
		result_ = output.str();
		if(paths.size() > 0) result_.pop_back(); // remove last endl
	}

	void Connectivity::generateExplorePaths(vector<Connectivity::Path> & paths, Location::Ptr loc, Path curPath){
		if(!loc) return;
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
				if(seg->returnSegment()->source()->type() != Location::Type::CUSTOMER){
					generateExplorePaths(paths, seg->returnSegment()->source(), newPath);
				}
			}
		}
	}

	void Connectivity::generateConnectPaths(vector<Connectivity::Path> & paths, Location::Ptr loc, Path curPath){
		Location::Ptr lastLoc = NULL;
		if(curPath.size() > 0) lastLoc = curPath.peek_back()->returnSegment()->source();
		if(lastLoc == query_.end()){
			paths.push_back(curPath);
			return;
		}
		if(!loc || (lastLoc && lastLoc->type() == Location::Type::CUSTOMER)) return;
		for(int i = 0; /***/; i++){
			Segment::Ptr seg = loc->segment(i);
			if(seg == NULL) break;
			Connectivity::Path newPath(curPath);
			if(newPath.push_back(seg)){
				generateConnectPaths(paths, seg->returnSegment()->source(), newPath);
			}
		}
	}



	bool Connectivity::Path::push_back(Segment::Ptr segment){
    Segment::Ptr comp = segment->returnSegment();
    if(segment->source() == NULL || comp == NULL || comp->source() == NULL) return false;
    Location::Ptr dest = comp->source();
    for(Segment::Ptr pathSeg : path_){

    // already visited; forbid cycles
      if(pathSeg->source() == dest) return false;
    }

    Fleet::Ptr fleet = connectivity_->engine_->fleet(segment->type());
    MilesPerHour speed = fleet ? fleet->speed() : MilesPerHour(1);
    DollarsPerMile cost = fleet ? fleet->cost() : DollarsPerMile(1);
    if(priority_ == Segment::Priority::EXPEDITED){
      if(segment->priority() == Segment::Priority::EXPEDITED){
        speed *= 1.3;
        cost *= 1.5;
      } else return false;
    }

    Mile len = segment->length();
    distance_ += len;
    cost_ += len * cost;
    if(speed.value() > 0) time_ += len / speed;
    path_.push_back(segment);
    return true;
  }

	string Connectivity::Path::stringify(Query::Type type){
		stringstream ss;
		if(type == Query::Type::connect_) ss << summary() << " ";
		for(Segment::Ptr fwd : path_){
			Segment::Ptr bak = fwd->returnSegment();
			ss << fwd->source()->name() << "(" << fwd->name() << ":" << string(fwd->length()) << ":" << bak->name() << ") ";
		}
		ss << path_[path_.size() - 1]->returnSegment()->source()->name();
		return ss.str();
	}

	string Connectivity::Path::summary(){
		stringstream ss;
		ss << string(cost_) << " " << string(time_) << " ";
		if(priority_ == Segment::Priority::EXPEDITED) ss << "yes";
		else ss << "no";
		ss << ";";
		return ss.str();
	}

}
