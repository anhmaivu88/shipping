#ifndef DOLLARSPERMILE_H
#define DOLLARSPERMILE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
    class DollarsPerMile : public Ordinal<DollarsPerMile, double> {
    public:

        DollarsPerMile(double num) : Ordinal<DollarsPerMile, double>(num) {

        }

      operator std::string() const { return std::to_string(value()); }
    };
}
#endif
