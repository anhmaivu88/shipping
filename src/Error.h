#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>

namespace Shipping {
  class ValueOutOfBoundsException : public std::runtime_error {
  public:
    ValueOutOfBoundsException(const string &message): std::runtime_error(message) {};
  };

  class MissingInstanceException : public std::runtime_error {
  public:
    MissingInstanceException(const string &message): std::runtime_error(message) {};
  };

  class InvalidAttributeException : public std::runtime_error {
  public:
    InvalidAttributeException(const string &message): std::runtime_error(message) {};
  };

  class InvalidDeletionTarget : public std::runtime_error {
  public:
    InvalidDeletionTarget(const string &message): std::runtime_error(message) {};
  };

  class InternalException : public std::runtime_error {
  public:
    InternalException(const string &message): std::runtime_error(message) {};
  };
}

#endif
