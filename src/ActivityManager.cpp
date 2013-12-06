#include "ActivityManagerImpl.h"
#include "ActivityImpl.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

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

    void ActivityManagerImpl::nowIs(Time t){
        if(now_ > t) throw new ValueOutOfBoundsException("Time moves only forward.");
        if(now_ == t) return;
        bool ran = false;
        while(now_ < t && !activities_.empty()){
            Activity::Ptr next = activities_.top();
            if(next->status() != Activity::Status::ready) break;
            now_ = min(max(now_, next->nextTime()), t);
            executeActivities();
            ran = true;
        }
        if(!ran){
            now_ = t;
            executeActivities();
        }
    }

    void RealtimeActivityManagerImpl::nowIs(Time t){
            // usleep(step_.value() * 100000); // sleep 100ms per hour
            // now_ = min(now_ + step_, t);
        if(now_ > t) throw new ValueOutOfBoundsException("Time moves only forward.");
        if(now_ == t) return;
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
            usleep((now - now_).value() * 100000);
            now_ = now;
            executeActivities();
        }
    }

    void ActivityManagerImpl::executeActivities(){
      std::cout << "Attempting to execute activites." << std::endl;
        while(!activities_.empty()){
          std::cout << "Found an activity." << std::endl;
            Activity::Ptr next = activities_.top();
            if(next->status() != Activity::Status::ready) break;
            if(next->nextTime() > now_) break;
            activities_.pop();
            next->statusIs(Activity::Status::executing);
            std::cout << "Executing activity." << std::endl;
        }
    }


}
