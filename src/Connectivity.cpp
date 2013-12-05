#include <sstream>
#include <iomanip>

#include "Connectivity.h"
#include "Query.h"
#include "Location.h"
#include "PathData.h"

namespace Shipping {
	using namespace std;

	void Connectivity::queryIs(Query query){
		query_ = query;
		vector<Path> paths;
		if(query_.type() == Query::Type::explore_){
		  Path empty(engine_, query.priority());
			generateExplorePaths(paths, query.start(), empty);
		} else if(query_.type() == Query::Type::connect_) {
		  Path emptyNormal(engine_, Segment::Priority::NORMAL);
			generateConnectPaths(paths, query.start(), emptyNormal);
		  Path emptyExpedited(engine_, Segment::Priority::EXPEDITED);
			generateConnectPaths(paths, query.start(), emptyExpedited);
		} else {
			result_ = "";
			return;
		}

		stringstream output;
		for(Path & p : paths){
			output << p.summary(query.type()) << endl;
		}
		result_ = output.str();
		if(paths.size() > 0) result_.erase(result_.size() - 1); // remove last endl
	}

	void Connectivity::generateExplorePaths(vector<Path> & paths, Location::Ptr loc, Path curPath){
		if(!loc) return;
		for(int i = 0; /***/; i++){
			Segment::Ptr seg = loc->segment(i);
			if(seg == NULL) break;
			Path newPath(curPath);

			if(newPath.segmentIs(seg) && 
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

	void Connectivity::generateConnectPaths(vector<Path> & paths, Location::Ptr loc, Path curPath){
		Location::Ptr lastLoc = NULL;
		size_t size = curPath.size();
		if(size > 0) lastLoc = curPath.segment(size - 1)->returnSegment()->source();
		if(lastLoc == query_.end()){
			paths.push_back(curPath);
			return;
		}
		if(!loc || (lastLoc && lastLoc->type() == Location::Type::CUSTOMER)) return;
		for(int i = 0; /***/; i++){
			Segment::Ptr seg = loc->segment(i);
			if(seg == NULL) break;
			Path newPath(curPath);
			if(newPath.segmentIs(seg)){
				generateConnectPaths(paths, seg->returnSegment()->source(), newPath);
			}
		}
	}



	

}
