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

    /* FIXME: maintain this somewhere */
    std::vector<Shipment::Ptr> shipments_;
    
    std::vector<Notifiee *> notifiees_;
    ActivityManager::Ptr activityManager_;

    void proxyOnPriority(EntityName segmentName, Segment::Priority priority) {
      for (auto notifiee : notifiees_) {
        notifiee->onSegmentPriority(segmentName, priority);
      }
    }

    class SegmentReactor : public Segment::Notifiee {
    public:
      static SegmentReactor::Ptr segmentReactorNew(Engine *engine, Segment *segment) {
        Ptr reactor = new SegmentReactor(engine, segment);
        return reactor;
      }

      void onPriority(Segment::Priority priority) {
        engine_->proxyOnPriority(segment()->name(), priority);
      }

      void onShipmentAdd(Shipment::Ptr shipment) {
        Activity::Ptr transferActivity = engine_->activityManager()->activityNew(segment()->name() + " transfer");
        transferActivity->notifieeAdd(new TransferActivityReactor(segment(), shipment, transferActivity.ptr()));
        transferActivity->nextTimeIs(0);
        transferActivity->statusIs(Activity::Status::ready);
      }
      
      Engine *engine_;
    private:
      SegmentReactor(Engine *engine, Segment *segment) : Notifiee(segment), engine_(engine) {}
    };

    class CustomerReactor : public Customer::Notifiee {
    public:
      static CustomerReactor::Ptr customerReactorNew(Engine *engine, Customer *customer) {
        Ptr reactor = new CustomerReactor(engine, customer);
        return reactor;
      }

      void onTransferRate(){
        TransferRate rate = customer_->transferRate();
        isTranferRate_ = (rate.value() != 0);
        checkActivity();
      }

      void onShipmentSize(){
        PackageCount size = customer_->shipmentSize();
        isShipmentSize_ = (size.value() != 0);
        checkActivity();
      }

      void onDestination(){
        Location::Ptr dest = customer_->destination();
        isDestination_ = (dest != NULL);
        checkActivity();
      }

      Engine *engine_;
    private:
      bool isTranferRate_;
      bool isShipmentSize_;
      bool isDestination_;
      bool isActivity_;
      InjectionActivityReactor::Ptr react_;
      Activity::ptr injectionActivity_;

      void checkActivity(){
        bool isNewActivity = isTranferRate_ && isShipmentSize_ && isDestination_;
        if(isNewActivity == isActivity_) return;
        if(isNewActivity){
          // Start activity
          injectionActivity_ = engine_->activityManager()->activityNew(customer_->name() + "_INJECT");
          InjectionActivityReactor::injectionActivityReactorNew(customer_, injectionActivity.ptr());
        } else {
          // Kill activity
          injectionActivity->statusIs(Activity::Status::deleted);
        }
        isActivity_ = isNewActivity;
      }

      CustomerReactor(Engine *engine, Customer *customer) : Notifiee(customer), engine_(engine) {}
    };

    Engine(EntityName name): Entity<Engine>(name), activityManager_(activityManagerInstance()) {}
  };
} /* end namespace */

#endif
