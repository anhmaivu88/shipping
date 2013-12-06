#ifndef __ACTIVITYMANAGER_H__
#define __ACTIVITYMANAGER_H__

#include <string>
#include <vector>

#include "PtrInterface.h"
#include "Ptr.h"

#include "ActivityTime.h"
#include "Activity.h"
#include "Notifiee.h"

class Activity;

class ActivityManager : public Fwk::PtrInterface<ActivityManager> {
public:
    typedef Fwk::Ptr<ActivityManager> Ptr;

    virtual Fwk::Ptr<Activity> activityNew(/* const string &name */) = 0;

    // virtual Fwk::Ptr<Activity> activity(const string &name) const = 0;

    // virtual void activityDel(const string &name) = 0;

    // virtual void lastActivityIs(Activity::Ptr) = 0;

    virtual Time now() const = 0;
    virtual void nowIs(Time) = 0;
    virtual Time step() const = 0;
    virtual void stepIs(Time) = 0;
    virtual Time speed() const = 0;
    virtual void speedIs(Time) = 0;

private:

};

extern Fwk::Ptr<ActivityManager> activityManagerInstance();

#endif
