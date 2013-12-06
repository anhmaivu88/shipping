class LocationReactor : public Location::Notifiee {
public:
  typedef Fwk::Ptr<LocationReactor> Ptr;
  static LocationReactor::Ptr locationReactorNew(Engine *engine, Location *location) {
    Ptr me = new LocationReactor(engine, location);
    return me;
  }

  virtual void onShipmentAdd(Shipment::Ptr shipment) {
    if (forwardingActivity() == NULL) {
      forwardingActivity_ = engine()->activityManager()->activityNew();
      ForwardingActivityReactor::forwardingActivityReactorNew(location(), forwardingActivity().ptr());
      forwardingActivity()->nextTimeIs(0);
    }

    forwardingActivity()->statusIs(Activity::Status::ready);
  }

private:
  Activity::Ptr forwardingActivity() { return forwardingActivity_; }
  Engine *engine() { return engine_; }

  Engine *engine_;
  Activity::Ptr forwardingActivity_;
  LocationReactor(Engine *engine, Location *location) : Notifiee(location), engine_(engine), forwardingActivity_(NULL) {};
};
