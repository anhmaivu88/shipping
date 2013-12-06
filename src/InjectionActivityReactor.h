#ifndef INJECTIONACTIVITYREACTOR_H
#define INJECTIONACTIVITYREACTOR_H

#include "Activity.h"
#include "Segment.h"
#include "Shipment.h"
#include <iostream>

namespace Shipping {
  class InjectionActivityReactor : public Activity::Notifiee {
  public: 
    typedef Fwk::Ptr<InjectionActivityReactor> Ptr;

    static InjectionActivityReactor::Ptr injectionActivityReactorNew(Customer::Ptr start, Activity *activity){
      Ptr ptr = new InjectionActivityReactor(start, activity);
      std::cout << "Injector created." << std::endl;
      return ptr;
    }

    void onStatus() {
      /* Inject new shipment */
      if (notifier()->status() == Activity::Status::executing) {
        std::cout << "Injection a shipment at " << origin()->name() << std::endl;
        std::cout << "Attempting to route to: " << origin()->destination()->name() << std::endl;
        PathData path = origin()->route(origin()->destination()->name());
        origin()->shipmentAdd(Shipment::shipmentNew(origin(), origin()->destination(), origin()->shipmentSize(), path));

        Activity::Ptr activity = notifier();
        activity->nextTimeIs(activity->nextTime() + Time(24.0 / origin()->transferRate().value()));
        activity->statusIs(Activity::Status::ready);
      }
    }

    // void onNextTime(){}

  private:

    InjectionActivityReactor(Customer::Ptr start, Activity *activity) : Notifiee(activity), origin_(start) {}

    Customer::Ptr origin() { return origin_; }
      
    Customer::Ptr origin_;
  };
}

#endif
