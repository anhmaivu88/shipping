class LocationReactor : public Location::Notifiee {
public:
  static locationReactorNew(Engine *engine, Location *location) {
    Ptr me = new LocationReactor(engine, location);
    return me;
  }

  virtual void onShipmentAdd(Shipment::Ptr shipment) {
    if (forwardingActivity() == NULL) {
      forwardingActivity_ = engine()->activityManager()->activityNew(location->name() + " forwarding activity");
      ForwardingActivityReactor::forwardingActivityReactorNew(location(), forwardingActivity());
      forwardingActivity()->nextTimeIs(0);
    }

    forwardingActivity()->statusIs(Activity::Status::ready);
  }

private:
  Activity::Ptr forwardingActivity() { return forwardingActivity_; }
  Engine *engine() { return engine_; }

  Activity::Ptr forwardingActivity_;
  Engine *engine_;
  LocationReactor(Engine *engine, Location *location) : Notifiee(location), engine_(engine), forwardingActivity_(NULL);
};
