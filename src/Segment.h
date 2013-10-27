#ifndef SEGMENT_H
#define SEGMENT_H

#include <string>
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Mile.h"
#include "Error.h"
#include "Location.h"

namespace Shipping {
  class Segment : public Entity<Segment> {
  public:
    class Difficulty : public Ordinal<Difficulty, float> {
    public:
      Difficulty(float num) : Ordinal<Difficulty, float>(num) {
        if(num < 1.0 || num > 5.0) {
          throw new ValueOutOfBoundsException("difficulty is not in 1-5 range");
        }
      }
    };

    void sourceIs(const Location &source);
    void lengthIs(Mile length) { length_ = length; }
    void returnIs(const Segment &segment);
    void difficultyIs(Difficulty difficulty) { difficulty_ = difficulty; }
    void expeditableIs(bool expeditable)  { expeditable_ = expeditable; }

    Location::Ptr source() { return source_; }
    Mile length() { return length_; }

    
  private:
    Mile length_;
    Difficulty difficulty_;
    bool expeditable_;
    Location::Ptr source_;
  };
}

#endif
