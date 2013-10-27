#ifndef MILE_H
#define MILE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
class Mile : public Nominal<Mile, unsigned int> {
public:

Mile(unsigned int num) : Nominal<Mile, unsigned int>(num) {

}
};
}
#endif
