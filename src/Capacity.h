#ifndef CAPACITY_H
#define CAPACITY_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
class Capacity : public Ordinal<Capacity, unsigned int> {
public:

Capacity(unsigned int num) : Ordinal<Capacity, unsigned int>(num) {

}

operator std::string() const { return std::to_string(value()); }
};
}
#endif
