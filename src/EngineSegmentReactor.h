class SegmentReactor : public Segment::Notifiee {
public:
  static SegmentReactor::Ptr segmentReactorNew(Engine *engine, Segment *segment) {
    Ptr reactor = new SegmentReactor(engine, segment);
    return reactor;
  }

  void onPriority(Segment::Priority priority) {
    engine_->proxyOnPriority(segment()->name(), priority);
  }

  void onShipmentAdd(Shipment::Ptr shipment) {
    Activity::Ptr transferActivity = engine_->activityManager()->activityNew();
    transferActivity->notifieeAdd(new TransferActivityReactor(segment(), shipment, transferActivity.ptr()));
    // FIXME: this is incorrect -- it should be how far ahead in the future.
    Fleet::Ptr fleet = engine_->fleet(segment()->type());

    Capacity fleetCapacity = fleet->capacity();
    float numberOfTrips = shipment->packageCount().value() / fleetCapacity.value();
    Hour totalTime = ceil(numberOfTrips) * (segment()->length() / fleet->speed()).value();

    transferActivity->nextTimeIs(engine_->activityManager()->now() + totalTime);
    transferActivity->statusIs(Activity::Status::ready);
  }
      
  Engine *engine_;
  private:
    SegmentReactor(Engine *engine, Segment *segment) : Notifiee(segment), engine_(engine) {}
  };
