#ifndef MANAGERIMPL_H
#define MANAGERIMPL_H

#include "ActivityManager.h"
#include "ActivityImpl.h"

namespace Shipping {
    class ActivityManagerImpl : public ActivityManager {
    public:
        typedef Fwk::Ptr<ActivityManagerImpl> Ptr;
        Activity::Ptr activityNew(const string& name);

    private:

    };
}

#endif