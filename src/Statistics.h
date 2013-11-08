#ifndef STATISTICS_H
#define STATISTICS_H

#include "Engine.h"
#include <map>
#include "Segment.h"
#include "Location.h"
#include "Percentage.h"

using namespace std;

namespace Shipping {
  class Statistics : public Entity<Statistics> {
  public:
    typedef Fwk::Ptr<Statistics> Ptr;

    EntityCount locationType(Location::Type type) { return locationStats_[type]; }
    void locationTypeIs(Location::Type type, EntityCount count) { locationStats_[type] = count; }
    void locationTypeInc(Location::Type type) { locationStats_[type] += 1; }
    void locationTypeDec(Location::Type type) { locationStats_[type] -= 1; }

    EntityCount segmentType(Segment::Type type) { return segmentStats_[type]; }
    void segmentTypeIs(Segment::Type type, EntityCount count) { segmentStats_[type] = count; }
    void segmentTypeInc(Segment::Type type) { segmentStats_[type] += 1; }
    void segmentTypeDec(Segment::Type type) { segmentStats_[type] -= 1; }

    void expeditedSegmentsInc() { expeditedSegments_++; }
    void expeditedSegmentsDec() { expeditedSegments_--; }
    EntityCount expeditedSegments() { return expeditedSegments_; }

    EntityCount segmentCount() { return segmentType(Segment::Type::BOAT) + segmentType(Segment::Type::PLANE) + segmentType(Segment::Type::TRUCK); }

    Percentage expeditedShippingPercentage() { return (float) expeditedSegments() / (float) segmentCount(); }

    static Statistics::Ptr statisticsNew(EntityName name, Engine::Ptr engine) {
      Ptr ptr = new Statistics(name, engine);
      
      return ptr;
    }

  private:
    Statistics(EntityName name, Engine::Ptr engine) : Entity(name), engine_(engine), expeditedSegments_(0){
      engineReactor_ = EngineReactor::engineReactorNew(this, engine.ptr());
      segmentStats_[Segment::Type::BOAT] = 0;
      segmentStats_[Segment::Type::TRUCK] = 0;
      segmentStats_[Segment::Type::PLANE] = 0;

      locationStats_[Location::Type::CUSTOMER] = 0;
      locationStats_[Location::Type::PORT] = 0;
      locationStats_[Location::Type::BOAT_TERMINAL] = 0;
      locationStats_[Location::Type::TRUCK_TERMINAL] = 0;
      locationStats_[Location::Type::PLANE_TERMINAL] = 0;
    }

    /* FIXME: should zero out engine reference when engine dies */
    class EngineReactor : public Engine::Notifiee {
    public:
      typedef Fwk::Ptr<EngineReactor> Ptr;
      /* engineReactorNew adds the new EngineReactor to `engine` as a notifiee */
      static EngineReactor::Ptr engineReactorNew(Statistics *stats, Engine *engine) {
        Ptr reactor = new EngineReactor(stats, engine);
        engine->notifieeAdd(reactor);
        return reactor;
      }

      void onLocationNew(EntityName locationName) {
        stats()->locationTypeInc(engine()->location(locationName)->type());
      }
      void onLocationDel(EntityName name) {
        stats()->locationTypeDec(engine()->location(name)->type());
      }
      void onSegmentNew(EntityName segmentName) {
        stats()->segmentTypeInc(engine()->segment(segmentName)->type());
      }
      void onSegmentDel(EntityName name) {
        Segment::Ptr segment = engine()->segment(name);
        stats()->segmentTypeDec(segment->type());

        if (segment->priority() == Segment::Priority::EXPEDITED) {
          stats()->expeditedSegmentsDec();
        }
      }
      void onSegmentPriority(EntityName name, Segment::Priority priority) {
        if (priority == Segment::Priority::EXPEDITED) {
          stats()->expeditedSegmentsInc();
        } else {
          stats()->expeditedSegmentsDec();
        }
      }

    private:
      EngineReactor(Statistics *stats, Engine *engine) : Engine::Notifiee(engine), stats_(stats) {}
      Statistics *stats() { return stats_; }
      Statistics *stats_;
    };

    Engine::Ptr engine_;
    EngineReactor::Ptr engineReactor_;

    map<Location::Type, int> locationStats_;
    map<Segment::Type, int> segmentStats_;

    EntityCount expeditedSegments_;
  };
}

#endif
