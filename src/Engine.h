#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Entity.h"
#include "Location.h"
#include "Segment.h"
#include "Fleet.h"
#include "Error.h"
#include "Query.h"
#include "Terminal.h"
#include "Shipment.h"
#include <map>
#include <vector>

namespace Shipping {
  class Engine : public Entity<Engine> {
  public:
    typedef Fwk::Ptr<Engine> Ptr;

    Segment::Ptr planeSegmentNew(EntityName name);
    Segment::Ptr boatSegmentNew(EntityName name);
    Segment::Ptr truckSegmentNew(EntityName name);
    Segment::Ptr segmentNew(EntityName name, Segment::Type type);
    void segmentDel(EntityName name);
    Segment::Ptr segment(EntityName name);

    Location::Ptr customerLocationNew(EntityName name);
    Location::Ptr portLocationNew(EntityName name);
    Terminal::Ptr terminalLocationNew(EntityName name, Segment::Type type);
    void locationDel(EntityName name);
    Location::Ptr location(EntityName name);

    Fleet::Ptr fleet(EntityName name);
    Fleet::Ptr fleet(Segment::Type type);
    Fleet::Ptr fleetNew(EntityName name);

    Query queryNew();

    static Engine::Ptr engineNew(EntityName name){
      Ptr ptr = new Engine(name);
      return ptr;
    }

    /* Notification stuff. We don't support asynchronous semantics here:
       these notifications are assumed to be correct and non-batched. */
    class Notifiee : public Fwk::PtrInterface<Notifiee> {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onFleetNew(EntityName locationName) {}
      virtual void onFleetDel(EntityName name, Fleet::Ptr fleet) {}
      virtual void onLocationNew(EntityName locationName) {}
      virtual void onLocationDel(EntityName name) {}
      virtual void onSegmentNew(EntityName segmentName) {}
      virtual void onSegmentDel(EntityName name) {}
      virtual void onSegmentPriority(EntityName segment, Segment::Priority priority) {}
      
    protected:
      Notifiee(Engine *engine) : engine_(engine) {}
      Engine *engine() { return engine_; }
      Engine *engine_;
    };

    void notifieeAdd(Notifiee::Ptr notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee::Ptr notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

  private:
    void segmentIs(EntityName name, Segment::Ptr segment);
    void locationIs(EntityName name, Location::Ptr location);
    void fleetIs(EntityName name, Fleet::Ptr fleet);
    
    std::map<EntityName, Segment::Ptr> segments_;
    std::map<EntityName, Location::Ptr> locations_;
    std::map<EntityName, Fleet::Ptr> fleets_;
    
    std::vector<Notifiee::Ptr> notifiees_;

  private:
    void proxyOnPriority(EntityName segmentName, Segment::Priority priority) {
      for (auto notifiee : notifiees_) {
        notifiee->onSegmentPriority(segmentName, priority);
      }
    }

    class SegmentReactor : public Segment::Notifiee {
    public:
      static SegmentReactor::Ptr segmentReactorNew(Engine *engine, Segment *segment) {
        Ptr reactor = new SegmentReactor(engine, segment);
        segment->notifieeAdd(reactor);
        return reactor;
      }

      void onPriority(Segment::Priority priority) {
        engine_->proxyOnPriority(segment()->name(), priority);
      }
      
      Engine *engine_;
    private:
      SegmentReactor(Engine *engine, Segment *segment) : Notifiee(segment), engine_(engine) {}
    };

    Engine(EntityName name): Entity<Engine>(name){}
  };
} /* end namespace */

#endif
