#ifndef ACTIVITYIMPL_H
#define ACTIVITYIMPL_H

#include "Activity.h"
#include "ActivityManager.h"
#include "ActivityManagerImpl.h"
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
    friend class ActivityManagerImpl;
    /* We hold a strong reference here because we want to allow
       "fire and forget" callbacks. */
    std::vector<Notifiee::Ptr> notifiees_;
    Status status_;
    Time nextTime_;

    static Activity::Ptr activityNew(const string& name){
      Ptr ptr = new ActivityImpl(name);
      return ptr;
    }

    ActivityImpl(const string& name): Activity(name), status_(Status::waiting), nextTime_(Time(0)) {}
    // ActivityImpl(const string& name, Time time): Activity(name), status_(Status::waiting), nextTime_(time) {}
    // ActivityImpl(const string& name, Status status): Activity(name), status_(status), nextTime_(Time(0)) {}
    // ActivityImpl(const string& name, Time time, Status status): Activity(name), status_(status), nextTime_(time) {}
  };

  void ActivityImpl::statusIs(Status s) { if (status_ != s) { status_ = s; for (auto notifiee : notifiees_) { notifiee->onStatus(); } } }

  void ActivityImpl::nextTimeIs(Time t) { if (nextTime_ != t) { nextTime_ = t; for (auto notifiee : notifiees_) { notifiee->onNextTime(); } } }
}

#endif
