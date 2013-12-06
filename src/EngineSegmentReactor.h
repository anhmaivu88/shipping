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
    std::cout << "Adding a transfer activity." << std::endl;
    Activity::Ptr transferActivity = engine_->activityManager()->activityNew();
    new TransferActivityReactor(segment(), shipment, transferActivity.ptr());
    Fleet::Ptr fleet = engine_->fleet(segment()->type());

    Capacity fleetCapacity = fleet->capacity();
    float numberOfTrips = shipment->packageCount().value() / fleetCapacity.value();
    Hour totalTime = ceil(numberOfTrips) * (segment()->length() / fleet->speed()).value();

    std::cout << "It should take " << totalTime.value() << " hours to transmit this." << std::endl;

    transferActivity->nextTimeIs(engine_->activityManager()->now() + totalTime);
    transferActivity->statusIs(Activity::Status::ready);
  }
      
  Engine *engine_;
  private:
    SegmentReactor(Engine *engine, Segment *segment) : Notifiee(segment), engine_(engine) {}
  };
