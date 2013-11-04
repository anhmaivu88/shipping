#include "Engine.h"

namespace Shipping {
  Terminal::Ptr Engine::terminalLocationNew(EntityName name, Segment::Type type) {
    Terminal::Ptr terminal = Terminal::terminalNew(name, type);
    return terminal;
  }
}
