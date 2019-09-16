// Copyright 2016 Abdurrahim Cakar
/**
* @file Node.cpp
* @date Oct 10, 2012
* @author Abdurrahim Cakar <abdurrahimcakar@gmail.com>
* @brief Xml node class
*/

#include <Xml/Dom/Node.h>
#include "NodeHandlerLibxml2.h"
#include <cstdlib>

namespace un
{
namespace Xml
{
namespace Dom
{

Node::Node(const std::shared_ptr<Node::_Handler> &_h) : _handler(_h) {}

Node::Node() {}

Node::Node(const char *name) : _handler(new Node::_Handler(name)) {}

Node::Node(const std::string &name)
    : _handler(new Node::_Handler(name.c_str())) {}

Node::Node(const std::string &name, const std::string &content)
    : _handler(new Node::_Handler(name.c_str(), content.c_str())) {}

Node::Node(const Node &bindFrom) : _handler(bindFrom._handler) {}

Node::Node(const char *name, const char *content)
    : _handler(new Node::_Handler(name, content)) {}

Node::~Node() {}

Node &Node::operator=(Node &rhs)
{
  this->_handler = rhs._handler;
  return *this;
}

// NODE CONTENT PROPERTY

Node *Node::ContentPropertyType::get_parent() const
{
  static const int offset = offsetof(Node, content);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::ContentPropertyType::operator const std::string() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return std::string(this->get_parent()->_handler->get_content());
}

void Node::ContentPropertyType::set_content(const std::string &rhs)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->set_content(rhs);
}

void Node::ContentPropertyType::set_content(const char *rhs, std::size_t size)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->set_content(rhs, size);
}

void Node::ContentPropertyType::set_content(const char *rhs)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->set_content(rhs);
}

Node *Node::NamePropertyType::get_parent() const
{
  static const int offset = offsetof(Node, name);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::NamePropertyType::operator const std::string() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->get_parent()->_handler->get_name();
}

const std::string &Node::NamePropertyType::operator=(const std::string &rhs)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  this->get_parent()->_handler->set_name(rhs);
  return rhs;
}

Node *Node::SizePropertyType::get_parent() const
{
  static const int offset = offsetof(Node, size);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::SizePropertyType::operator std::size_t() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->get_parent()->_handler->get_size();
}

Node::iterator Node::begin()
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->begin();
}

Node::iterator Node::end()
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->end();
}

Node::iterator Node::begin() const
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->begin();
}

Node::iterator Node::end() const
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->end();
}

Node Node::operator[](const char *const key) const
{
  if (this->_handler == nullptr)
  {
    return Node(std::shared_ptr<Node::_Handler>(nullptr));
  }

  return this->_handler->get_child(key);
}

Node Node::operator[](const std::string &key) const
{
  if (this->_handler == nullptr)
  {
    return Node(std::shared_ptr<Node::_Handler>(nullptr));
  }

  return this->_handler->get_child(key);
}

Node Node::operator[](int index) const
{
  if (this->_handler == nullptr)
  {
    return Node(std::shared_ptr<Node::_Handler>(nullptr));
  }

  return this->_handler->get_child(index);
}

bool Node::operator==(const Node &rhs) const
{
  return this->_handler == rhs._handler;
}

bool Node::operator==(const void *const rhs) const
{
  if (this->_handler == nullptr)
  {
    return rhs == nullptr;
  }

  return this->_handler->_handler == rhs;
}

void Node::remove(const Node &node)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Node not found");
  }

  this->_handler->remove(node);
}

void Node::push_back(const Node &node)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->_handler->push_back(node);
}

void Node::push_front(const Node &node)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->_handler->push_front(node);
}

Node Node::pop_back()
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->_handler->pop_back();
}

Node Node::pop_front()
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->_handler->pop_front();
}

/////////////// Node attribute
bool Node::Attribute::operator==(decltype(nullptr)) const
{
  return this->_handler == nullptr;
}

Node::Attribute Node::get_attribute_from_index(int index)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->get_attribute_from_index(index);
}

Node::Attribute Node::get_attribute_from_name(const char *name)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->get_attribute_from_name(name);
}

Node::Attribute Node::get_attribute_from_name(const std::string &name)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->_handler->get_attribute_from_name(name);
}

Node::Attribute &Node::Attribute::operator=(const char *val)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  this->_handler->set_value(val);
  return *this;
}

Node::Attribute &Node::Attribute::operator=(const std::string &val)
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  this->_handler->set_value(val);
  return *this;
}

Node::Attribute::operator std::string() const
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return std::string(this->_handler->get_value());
}

/////// Node::Attribute::Name
Node::Attribute *Node::Attribute::NamePropertyType::get_parent() const
{
  static const int offset = offsetof(Node::Attribute, name);
  return (Node::Attribute *)(((uint8_t *)this) - offset);
}

bool Node::Attribute::NamePropertyType::
operator==(const std::string &rhs) const
{
  return std::strncmp(this->get_parent()->_handler->get_name(), rhs.c_str(), rhs.length()) == 0;
}

bool Node::Attribute::NamePropertyType::operator==(const char *rhs) const
{
  return std::strcmp(this->get_parent()->_handler->get_name(), rhs) == 0;
}

Node::Attribute::NamePropertyType::operator const char *() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->_handler->get_name();
}

/////// Node::Attribute::Name

/////// Node::Attribute::Value
Node::Attribute *Node::Attribute::ValuePropertyType::get_parent() const
{
  static const int offset = offsetof(Node::Attribute, value);
  return (Node::Attribute *)(((uint8_t *)this) - offset);
}

Node::Attribute::ValuePropertyType &Node::Attribute::ValuePropertyType::
operator=(const char *val)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->set_value(val);
  return *this;
}

Node::Attribute::ValuePropertyType &Node::Attribute::ValuePropertyType::
operator=(const std::string &val)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->set_value(val);
  return *this;
}

Node::Attribute::ValuePropertyType::operator const char *() const
{
  return this->get_parent()->_handler->get_value();
}
/////// Node::Attribute::Value

/////////////// END Node attribute

std::size_t Node::AttributesPropertyType::get_size() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->_handler->get_attributes_size();
}

Node *Node::AttributesPropertyType::get_parent() const
{
  static const int offset = offsetof(Node, attributes);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::AttributesPropertyType *
Node::AttributesPropertyType::SizePropertyType::get_parent() const
{
  static const int offset = offsetof(Node::AttributesPropertyType, size);
  return (Node::AttributesPropertyType *)(((uint8_t *)this) - offset);
}

Node::AttributesPropertyType::SizePropertyType::operator std::size_t() const
{
  return this->get_parent()->get_size();
}

Node::Attribute Node::AttributesPropertyType::operator[](int index) const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->get_attribute_from_index(index);
}

Node::Attribute Node::AttributesPropertyType::
operator[](const char *name) const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->get_attribute_from_name(name);
}

Node::Attribute Node::AttributesPropertyType::
operator[](const std::string &name) const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->get_attribute_from_name(name);
}

Node::AttributesPropertyType *
Node::AttributesPropertyType::IsEmptyPropertyType::get_parent() const
{
  static const int offset = offsetof(Node::AttributesPropertyType, is_empty);
  return (Node::AttributesPropertyType *)(((uint8_t *)this) - offset);
}

bool Node::is_attributes_empty() const
{
  if (this->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->_handler->is_attributes_empty();
}

bool Node::AttributesPropertyType::get_is_empty() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->is_attributes_empty();
}

Node::AttributesPropertyType::IsEmptyPropertyType::operator bool() const
{
  return this->get_parent()->get_is_empty();
}

void Node::AttributesPropertyType::push_back(const Node::Attribute &attr)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->push_back_attribute(attr);
}

void Node::AttributesPropertyType::push_back(const std::string &name, const std::string &value)
{
  this->push_back(name.c_str(), name.length(), value.c_str(), value.length());
}

void Node::AttributesPropertyType::push_back(
  const char *name,
  std::size_t name_size,
  const char *value,
  std::size_t value_size)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->push_back_attribute(name, name_size, value, value_size);
}

void Node::AttributesPropertyType::push_back(const char *name, const char *value)
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->_handler->push_back_attribute(name, value);
}

Node::AttributesPropertyType::iterator Node::AttributesPropertyType::begin()
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->_handler->begin_attr());
}

const Node::AttributesPropertyType::iterator
Node::AttributesPropertyType::begin() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->_handler->begin_attr());
}

Node::AttributesPropertyType::iterator Node::AttributesPropertyType::end()
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->_handler->end_attr());
}

const Node::AttributesPropertyType::iterator
Node::AttributesPropertyType::end() const
{
  if (this->get_parent()->_handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->_handler->end_attr());
}

} // namespace Dom
} // namespace Xml
} // namespace un
