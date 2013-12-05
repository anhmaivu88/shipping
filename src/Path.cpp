#include "Path.h"

namespace Shipping {
    using namespace std;

    bool Path::segmentIs(Segment::Ptr segment){
        Segment::Ptr comp = segment->returnSegment();
        if(segment->source() == NULL || comp == NULL || comp->source() == NULL) return false;
        Location::Ptr dest = comp->source();
        for(Segment::Ptr pathSeg : path_){
            // already visited; forbid cycles
            if(pathSeg->source() == dest) return false;
        }

        Fleet::Ptr fleet = engine_->fleet(segment->type());
        MilesPerHour speed = fleet ? fleet->speed() : MilesPerHour(1);
        DollarsPerMile cost = fleet ? fleet->cost() : DollarsPerMile(1);
        cost *= segment->difficulty().value();

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

    string Path::summary(Query::Type type){
        stringstream ss;
        if(type == Query::Type::connect_) ss << connectPreamble() << " ";
        for(Segment::Ptr fwd : path_){
            Segment::Ptr bak = fwd->returnSegment();
            ss << fwd->source()->name() << "(" << fwd->name() << ":" << string(fwd->length()) << ":" << bak->name() << ") ";
        }
        ss << path_[path_.size() - 1]->returnSegment()->source()->name();
        return ss.str();
    }

    string Path::connectPreamble(){
        stringstream ss;
        ss << string(cost_) << " " << string(time_) << " ";
        if(priority_ == Segment::Priority::EXPEDITED) ss << "yes";
        else ss << "no";
        ss << ";";
        return ss.str();
    }
}