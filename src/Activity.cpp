#include "Activity.h"
#include <vector>

namespace Shipping {
  class ActivityImpl : public Activity {
    void notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

    Activity::Status status() const { return status_; }
    void statusIs(Status status);
    Time nextTime() const { return nextTime_; }
    void nextTimeIs(Time t);


  private:
    /* We hold a strong reference here because we want to allow
       "fire and forget" callbacks. */
    std::vector<Notifiee::Ptr> notifiees_;
    Status status_;
    Time nextTime_;
  };

  void ActivityImpl::statusIs(Status s) { status_ = s; for (auto notifiee : notifiees_) { notifiee->onStatus(); } }

  void ActivityImpl::nextTimeIs(Time t) { nextTime_ = t; for (auto notifiee : notifiees_) { notifiee->onNextTime(); } }
}
