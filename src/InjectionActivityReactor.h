#ifndef INJECTIONACTIVITYREACTOR_H
#define INJECTIONACTIVITYREACTOR_H

#include "Activity.h"
#include "Segment.h"
#include "Shipment.h"

namespace Shipping {
  class InjectionActivityReactor : public Activity::Notifiee {
  public: 
    typedef Fwk::Ptr<InjectionActivityReactor> Ptr;

    static InjectionActivityReactor::Ptr injectionActivityReactorNew(Location::Ptr start, TransferRate rate, PackageCount size, Location::Ptr dest, Activity *activity){
      Ptr ptr = new InjectionActivityReactor(start, rate, size, dest, activity);
      return ptr;
    }

    void onStatus() {
      /* Inject new shipment */
      if (notifier()->status() == Activity::Status::executing) {
        PathData path = origin_->route(destination_->name());
        origin_->shipmentAdd(Shipment::shipmentNew(origin_, destination_, packageCount_, path));

        Activity::Ptr activity = notifier();
        activity->nextTimeIs(activity->nextTime() + Time(24.0 / transferRate_.value()));
        activity->statusIs(Activity::Status::waiting);
      }
    }

    // void onNextTime(){}

  private:

    InjectionActivityReactor(Location::Ptr start, TransferRate rate, PackageCount size, Location::Ptr dest, Activity *activity) : 
      Notifiee(activity), origin_(start), transferRate_(rate), packageCount_(size), destination_(dest) {}

    Location::Ptr origin_;
    TransferRate transferRate_;
    PackageCount packageCount_;
    Location::Ptr destination_;
  };
}

#endif