#include "ActivityManagerImpl.h"
#include "ActivityImpl.h"

Fwk::Ptr<ActivityManager> activityManagerInstance(){
    static Fwk::Ptr<ActivityManager> theManager = new Shipping::ActivityManagerImpl();
    return theManager;
}

namespace Shipping {

    Fwk::Ptr<Activity> ActivityManagerImpl::activityNew(){
        Activity::Ptr activity =  ActivityImpl::activityNew("activity");
        activity->statusIs(Activity::Status::waiting);
        ActivityManagerReactor::Ptr reactor = ActivityManagerReactor::activityManagerReactorNew(this, activity.ptr());
        activity->notifieeAdd(reactor.ptr());
        return activity;
    }

    void ActivityManagerImpl::nowIs(Time t){
        if(now_ != t){
            now_ = t;
            executeActivities();
        }
    }

    void ActivityManagerImpl::executeActivities(){
        while(!activities_.empty()){
            Activity::Ptr next = activities_.top();
            if(next->status() != Activity::Status::ready) break;
            if(next->nextTime() > now_) break;
            next->statusIs(Activity::Status::executing);
            activities_.pop();
        }
    }

}