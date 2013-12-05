#ifndef TYPEDEF_H
#define TYPEDEF_H

/* This file is designed to fix circular dependencies.
   We don't know enough C++ to fix this more elegantly.
 */
namespace Shipping {
  class Location;
  typedef Fwk::Ptr<Location> LocationPtr;
  class Segment;
  typedef Fwk::Ptr<Segment> SegmentPtr;
}

#endif
