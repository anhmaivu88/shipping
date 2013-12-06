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
    std::vector<Notifiee::Ptr> notifiees_;
    Status status_;
    Time nextTime_;
  };

  

  void ActivityImpl::statusIs(Status s) { }

  void ActivityImpl::nextTimeIs(Time t) { nextTime_ = t; }
}
