#ifndef MILE_H
#define MILE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Engine.h"
#include "Nominal.h"

namespace Shipping {
class Mile : public Ordinal<Mile, unsigned int> {
public:

Mile(unsigned int num) : Ordinal<Mile, unsigned int>(num) {

}
};
}
#endif
