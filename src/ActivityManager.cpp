#include "ActivityManagerImpl.h"
#include "ActivityImpl.h"

Fwk::Ptr<ActivityManager> activityManagerInstance(){
    return NULL;
}

namespace Shipping {
    Activity::Ptr ActivityManagerImpl::activityNew(const string& name){
        // just compile please
        // TODO: store/manage
        return ActivityImpl::activityNew(name);
    }

}