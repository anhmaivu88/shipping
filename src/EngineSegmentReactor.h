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
    Activity::Ptr transferActivity = engine_->activityManager()->activityNew(segment()->name() + " transfer");
    transferActivity->notifieeAdd(new TransferActivityReactor(segment(), shipment, transferActivity.ptr()));
    transferActivity->nextTimeIs(0);
    transferActivity->statusIs(Activity::Status::ready);
  }
      
  Engine *engine_;
private:
  SegmentReactor(Engine *engine, Segment *segment) : Notifiee(segment), engine_(engine) {}
};
