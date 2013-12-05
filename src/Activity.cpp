#include "Activity.h"
#include <vector>

namespace Shipping {
  class ActivityImpl : public Activity {
    void notifieeAdd(Notifiee *notifiee) { notifiees_.push_back(notifiee); }
    void notifieeDel(Notifiee *notifiee) { notifiees_.erase(find(notifiees_.begin(), notifiees_.end(), notifiee)); }

  private:
    std::vector<Notifiee::Ptr> notifiees_;
  };
}
