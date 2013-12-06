#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

#include <string>

#include "PtrInterface.h"
#include "Ptr.h"

#include "ActivityTime.h"
#include "Notifiee.h"
#include "Time.h"

using std::string;

/* Define the type 'Time' */

class Activity : public Fwk::PtrInterface<Activity> {
public:
  typedef Fwk::Ptr<Activity> Ptr;
    
  /* Notifiee class for Activities */
  class Notifiee : public Fwk::BaseNotifiee<Activity> {
  public:
    typedef Fwk::Ptr<Notifiee> Ptr;

    Notifiee(Activity* act) : Fwk::BaseNotifiee<Activity>(act) {}

    virtual void onNextTime() {}
    virtual void onStatus() {}
  };

  enum Status {
    free, waiting, ready, executing, nextTimeScheduled, deleted
  };

  virtual Status status() const = 0;
  virtual void statusIs(Status s)  = 0;
    
  virtual Time nextTime() const = 0;
  virtual void nextTimeIs(Time t) = 0;

  virtual void notifieeAdd(Notifiee *notifiee) = 0;
  virtual void notifieeDel(Notifiee *notifiee) = 0;

  virtual string name() const { return name_; }

protected:
  Activity(const string &name)
    : name_(name)
  {}

private:
  string name_;

};


#endif
