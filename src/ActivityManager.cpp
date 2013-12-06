#include "ActivityManagerImpl.h"
#include "ActivityImpl.h"
#include <iostream>

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

        if(now_ < t){
            now_ = t;
            executeActivities();
        } else if(now_ > t){
            throw new ValueOutOfBoundsException("Time moves only forward.");
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
