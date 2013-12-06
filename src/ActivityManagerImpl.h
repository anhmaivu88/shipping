#ifndef MANAGERIMPL_H
#define MANAGERIMPL_H

#include <queue>
#include "ActivityManager.h"
#include "ActivityImpl.h"
#include "ActivityTime.h"
#include <iostream>

namespace Shipping {
    class ActivityManagerImpl : public ActivityManager {
    public:
        typedef Fwk::Ptr<ActivityManagerImpl> Ptr;

        ActivityManagerImpl() : now_(Time(0)) {}

        Fwk::Ptr<Activity> activityNew();

        virtual void nowIs(Time t);
        Time now() const { return now_; }
    protected:
        friend class ActivityManagerReactor;

        Time now_;
        struct ActivityComparator {
            bool operator()(const Activity::Ptr left, const Activity::Ptr right){
                bool lr = left->status() == Activity::Status::ready;
                bool rr = right->status() == Activity::Status::ready;
                if(lr && !rr) return true;
                if(!lr && rr) return false;

                return left->nextTime() < right->nextTime();
            }
        };
        std::priority_queue<Activity::Ptr, std::vector<Activity::Ptr>, ActivityComparator> activities_;

        void executeActivities();


        class ActivityManagerReactor : public Activity::Notifiee {
        public:
            typedef Fwk::Ptr<ActivityManagerReactor> Ptr;
            static ActivityManagerReactor::Ptr activityManagerReactorNew(ActivityManagerImpl *manager, Activity* act){
              std::cout << "I'm one of them." << std::endl;
                Ptr ptr = new ActivityManagerReactor(manager, act);
                return ptr;
            }

            void onNextTime() {
                manager_->executeActivities();
            }

            void onStatus() {
              std::cout << "Observing status change" << std::endl;
                Activity::Ptr activity = notifier();
                if(activity->status() == Activity::Status::ready){
                    manager_->activities_.push(activity);
                }
            }

        private:
            ActivityManagerImpl *manager_;
            ActivityManagerReactor(ActivityManagerImpl *manager, Activity* act) : Notifiee(act), manager_(manager) {}

        };

    };

    class RealtimeActivityManagerImpl : public ActivityManagerImpl {
    public:
        RealtimeActivityManagerImpl(Time step) : ActivityManagerImpl(), step_(step) {}
        void nowIs(Time t);
    protected:
        friend class ActivityManagerReactor;
        Time step_;

    };
}

#endif
