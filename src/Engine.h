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
    // Segment::Ptr segmentNew(EntityName name, Segment::Type type);
    Segment::Ptr segment(EntityName name);

    Location::Ptr customerLocationNew(EntityName name);
    Location::Ptr portLocationNew(EntityName name);
    Terminal::Ptr terminalLocationNew(EntityName name, Segment::Type type);
    // Location::Ptr locationNew(EntityName name, Location::Type type);
    Location::Ptr location(EntityName name);

    Fleet::Ptr fleetNew(EntityName name);

    Query queryNew();

    static Engine::Ptr engineNew(EntityName name){
        Ptr ptr = new Engine(name);
        ptr->deleteRef();
        return ptr;
    }

    /* Notification stuff */
    class Notifiee : public Fwk::PtrInterface<Notifiee> {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      virtual void onLocationNew(Location::Ptr location) {}
      virtual void onSegmentNew(Segment::Ptr segment) {}
      Notifiee(Engine *engine) : engine_(engine) {}
      
    protected:
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
