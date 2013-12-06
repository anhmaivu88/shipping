#include "Activity.h"
#include <vector>

namespace Shipping {
  class ActivityImpl : public Activity {
    virtual void notifieeAdd(Notifiee *notifiee);
    virtual void notifieeDel(Notifiee *notifiee);
    virtual void statusIs(Status status);
    virtual Status status();
    virtual void nextTimeIs();
    virtual Time nextTime();
    
  private:
    std::vector<Notifiee::Ptr> notifiees_;
    Status status_;
    Time nextTime_;
  };

  void ActivityImpl::notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
  void ActivityImpl::notifieeDel(Notifiee *notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

  void ActivityImpl::statusIs(Status s) { }
  Status ActivityImpl::status() { return status_; }

  void ActivityImpl::nextTimeIs(Time t) { nextTime_ = t; }
  Time ActivityImpl::nextTime() { return nextTime_; }
}
