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
    Segment::Ptr segment(EntityName name) { return segments_[name]; }

    Location::Ptr customerLocationNew(EntityName name);
    Location::Ptr portLocationNew(EntityName name);
    Terminal::Ptr terminalLocationNew(EntityName name, Segment::Type type);
    Location::Ptr locationNew(EntityName name, Location::Type type);
    Location::Ptr location(EntityName name) { return locations_[name]; }

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
      virtual void onLocationNew(EntityName locationName) {}
      virtual void onLocationDel(EntityName name, Location::Ptr location) {}
      virtual void onSegmentNew(EntityName segmentName) {}
      virtual void onSegmentDel(EntityName name, Segment::Ptr segment) {}

      
    protected:
      Notifiee(Engine *engine) : engine_(engine) {}
      Engine *engine_;
    };

    void notifieeAdd(Notifiee::Ptr notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee::Ptr notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

  private:
    std::map<EntityName, Segment::Ptr> segments_;
    std::map<EntityName, Location::Ptr> locations_;
    std::map<EntityName, Fleet::Ptr> fleets_;

    std::vector<Notifiee::Ptr> notifiees_;

    Engine(EntityName name): Entity<Engine>(name){}
  };
} /* end namespace */

#endif
