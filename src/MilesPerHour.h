#ifndef MILESPERHOUR_H
#define MILESPERHOUR_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
class MilesPerHour : public Ordinal<MilesPerHour, unsigned int> {
public:

MilesPerHour(unsigned int num) : Ordinal<MilesPerHour, unsigned int>(num) {

}
};
}
#endif
