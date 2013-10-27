#ifndef DOLLARSPERMILE_H
#define DOLLARSPERMILE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
class DollarsPerMile : public Ordinal<DollarsPerMile, unsigned int> {
public:

DollarsPerMile(unsigned int num) : Ordinal<DollarsPerMile, unsigned int>(num) {

}
};
}
#endif
