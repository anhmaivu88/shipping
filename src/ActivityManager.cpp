#include "ActivityManagerImpl.h"
#include "ActivityImpl.h"
#include <iostream>
#include <time.h>
#include <unistd.h>
#include "Microsecond.h"

Fwk::Ptr<ActivityManager> activityManagerInstance(){
    static Fwk::Ptr<ActivityManager> theManager = new Shipping::ActivityManagerImpl();
    return theManager;
}

namespace Shipping {

    Fwk::Ptr<Activity> ActivityManagerImpl::activityNew(){
        Activity::Ptr activity =  ActivityImpl::activityNew("activity");
        activity->statusIs(Activity::Status::waiting);
        ActivityManagerReactor::Ptr reactor = ActivityManagerReactor::activityManagerReactorNew(this, activity.ptr());
        return activity;
    }

    void ActivityManagerImpl::nowIs(Time now){
        if(step_.value() == 0.0) simulateVirtual(now);
        else simulateRealtime(now);
    }

    void ActivityManagerImpl::simulateVirtual(Time t){
        if(now_ > t) std::cerr << "Attempted to move time backwards." << std::endl;
        if(now_ >= t) return;
        bool ran = false;
        while(now_ < t && !activities_.empty()){
            Activity::Ptr next = activities_.top();
            if(next->status() != Activity::Status::ready) {
              break;
            }
            now_ = min(max(now_, next->nextTime()), t);
            executeActivities();
            ran = true;
        }
        if(!ran){
            now_ = t;
            executeActivities();
        }
    }

    void ActivityManagerImpl::simulateRealtime(Time t){
        if(now_ > t) std::cerr << "Attempted to move time backwards." << std::endl;
        if(now_ >= t) return;
        while(now_ < t){
            Time step = step_;
            if(!activities_.empty()){
                Activity::Ptr next = activities_.top();
                if(next->status() == Activity::Status::ready){
                    Time nextStep(0);
                    if(next->nextTime() > now_) nextStep = next->nextTime() - now_;
                    step = min(step, nextStep);
                }
            }
            
            Time now = min(now_ + step, t);
            usleep((now - now_).value() * (Microsecond(1000000) / speed_).value());
            now_ = now;
            executeActivities();
        }
    }

    void ActivityManagerImpl::executeActivities() {
      std::cout << "The time is " << now_ << std::endl;
      std::priority_queue<Activity::Ptr, std::vector<Activity::Ptr>, ActivityComparator> queueCopy = activities_;
      while (!queueCopy.empty()) {
        Activity::Ptr activity = queueCopy.top();
        queueCopy.pop();
      }
    
        while(!activities_.empty()){
            Activity::Ptr next = activities_.top();
            if(next->status() != Activity::Status::ready) {
              break;
            }
            if(next->nextTime() > now_) break;
            activities_.pop();
            next->statusIs(Activity::Status::executing);
        }
    }


}
