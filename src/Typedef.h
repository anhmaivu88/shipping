#ifndef TYPEDEF_H
#define TYPEDEF_H

/* This file is designed to fix a circular dependency that exists between segment and location 
   We don't know enough C++ to fix this more elegantly.
 */
namespace Shipping {
  class Location;
  typedef Fwk::Ptr<Location> LocationPtr;
  class Segment;
  typedef Fwk::Ptr<Segment> SegmentPtr;
}

#endif
