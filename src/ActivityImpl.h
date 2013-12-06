#ifndef ACTIVITYIMPL_H
#define ACTIVITYIMPL_H

#include "Activity.h"
#include "ActivityManager.h"
#include "ActivityManagerImpl.h"
#include <vector>
#include <iostream>

namespace Shipping {
  class ActivityImpl : public Activity {

    void notifieeAdd(Notifiee *notifiee) { 
      std::cout << "Adding a notifiee to an activity: " << notifiees_.size() << std::endl;
      notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { 
      std::cout << "!!!Attempting to delete a notifier" << std::endl;
      notifiees_.erase(std::find(notifiees_.begin(), notifiees_.end(), notifiee)); 
    }

    Activity::Status status() const { return status_; }
    void statusIs(Status status);
    Time nextTime() const { return nextTime_; }
    void nextTimeIs(Time t);


  private:
    friend class ActivityManagerImpl;
    /* We hold a strong reference here because we want to allow
       "fire and forget" reactors. */
    std::vector<Notifiee::Ptr> notifiees_;
    Status status_;
    Time nextTime_;

    static Activity::Ptr activityNew(const string& name){
      Ptr ptr = new ActivityImpl(name);
      return ptr;
    }

    ActivityImpl(const string& name): Activity(name), status_(Status::waiting), nextTime_(Time(0)) {}
  };

  void ActivityImpl::statusIs(Status s) { 
    std::cout << "Changing status." << std::endl;
    if(s == Status::ready) std::cout << "^ (to ready)" << endl;
    if(s == Status::waiting) std::cout << "^ (to waiting)" << endl;

    if (status_ != s) { 
      std::cout << "We have a new status." << std::endl;
      status_ = s; 
      for (auto notifiee : notifiees_) { notifiee->onStatus(); } 

      if (s == Activity::Status::deleted) {
        while (notifiees_.size() > 0) {
          std::cout << "Nulling a notifier." << std::endl;
          std::cout << "Size: " << notifiees_.size() << std::endl;
          notifiees_[0]->notifierIs(NULL);
        }
      }
    }
  }

  void ActivityImpl::nextTimeIs(Time t) { if (nextTime_ != t) { nextTime_ = t; for (auto notifiee : notifiees_) { notifiee->onNextTime(); } } }
}

#endif
