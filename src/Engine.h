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
#include "Notifiee.h"
#include "ActivityManager.h"
#include "ForwardingActivityReactor.h"
#include "TransferActivityReactor.h"
#include "InjectionActivityReactor.h"
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
    std::map<EntityName, Location::Ptr> locations(){ return locations_; }

    Fleet::Ptr fleet(EntityName name);
    Fleet::Ptr fleet(Segment::Type type);
    Fleet::Ptr fleetNew(EntityName name);

    ActivityManager::Ptr activityManager() { return activityManager_; }

    Query queryNew();

    static Engine::Ptr engineNew(EntityName name){
      Ptr ptr = new Engine(name);
      return ptr;
    }

    /* Notification stuff. We don't support asynchronous semantics here:
       these notifications are assumed to be correct and non-batched. */
    class Notifiee : public Fwk::BaseNotifiee<Engine> {
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
      Notifiee(Engine *engine) : BaseNotifiee(engine), engine_(engine) {}
      Engine *engine() { return engine_; }
      Engine *engine_;
    };

    void notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

    ~Engine() {
      for (auto notifiee : notifiees_) {
        notifiee->notifierIs(NULL);
      }
    }

  private:
    void segmentIs(EntityName name, Segment::Ptr segment);
    void locationIs(EntityName name, Location::Ptr location);
    void fleetIs(EntityName name, Fleet::Ptr fleet);
    
    std::map<EntityName, Segment::Ptr> segments_;
    std::map<EntityName, Location::Ptr> locations_;
    std::map<EntityName, Fleet::Ptr> fleets_;
    
    std::vector<Notifiee *> notifiees_;
    ActivityManager::Ptr activityManager_;

    void proxyOnPriority(EntityName segmentName, Segment::Priority priority) {
      for (auto notifiee : notifiees_) {
        notifiee->onSegmentPriority(segmentName, priority);
      }
    }
    
#include "EngineSegmentReactor.h"
#include "EngineLocationReactor.h"
#include "EngineCustomerReactor.h"
    Engine(EntityName name): Entity<Engine>(name), activityManager_(activityManagerInstance()) {
      fleetNew(Fleet::boat());
      fleetNew(Fleet::plane());
      fleetNew(Fleet::truck());
    }
  };
      
} /* end namespace */

#endif
