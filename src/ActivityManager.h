#ifndef __ACTIVITYMANAGER_H__
#define __ACTIVITYMANAGER_H__

#include <string>

#include "PtrInterface.h"
#include "Ptr.h"

#include "Time.h"
#include "Notifiee.h"


class Activity::Manager : public Fwk::PtrInterface<Activity::Manager> {
public:
    typedef Fwk::Ptr<Activity::Manager> Ptr;

    virtual Fwk::Ptr<Activity> activityNew(const string &name) = 0;

    virtual Fwk::Ptr<Activity> activity(const string &name) const = 0;

    virtual void activityDel(const string &name) = 0;

    virtual void lastActivityIs(Activity::Ptr) = 0;

    virtual Time now() const = 0;
    virtual void nowIs(Time) = 0;


private:
    /* Up to you */

};

extern Fwk::Ptr<Activity::Manager> activityManagerInstance();

#endif
