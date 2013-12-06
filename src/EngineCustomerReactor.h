class CustomerReactor : public Customer::Notifiee {
public:
  static CustomerReactor::Ptr customerReactorNew(Engine *engine, Customer *customer) {
    Ptr reactor = new CustomerReactor(engine, customer);
    return reactor;
  }

  void onTransferRate(){
    TransferRate rate = customer_->transferRate();
    isTranferRate_ = (rate.value() != 0);
    checkActivity();
  }

  void onShipmentSize(){
    PackageCount size = customer_->shipmentSize();
    isShipmentSize_ = (size.value() != 0);
    checkActivity();
  }

  void onDestination(){
    Location::Ptr dest = customer_->destination();
    isDestination_ = (dest != NULL);
    checkActivity();
  }

  Engine *engine_;
private:
  bool isTranferRate_;
  bool isShipmentSize_;
  bool isDestination_;
  bool isActivity_;
  InjectionActivityReactor::Ptr react_;
  Activity::Ptr injectionActivity_;

  void checkActivity(){
    bool isNewActivity = isTranferRate_ && isShipmentSize_ && isDestination_;
    if(isNewActivity == isActivity_) return;
    if(isNewActivity){
      // Start activity
      injectionActivity_ = engine_->activityManager()->activityNew(customer()->name() + "_INJECT");
      InjectionActivityReactor::injectionActivityReactorNew(Fwk::Ptr<Customer>(customer()), injectionActivity_.ptr());
    } else {
      // Kill activity
      injectionActivity_->statusIs(Activity::Status::deleted);
    }
    isActivity_ = isNewActivity;
  }

  CustomerReactor(Engine *engine, Customer *customer) : Notifiee(customer), engine_(engine) {}
};

