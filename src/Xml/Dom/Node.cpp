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

Node::Node(const std::shared_ptr<Node::Handler> &_h) : handler(_h) {}

Node::Node() {}

Node::Node(const char *name) : handler(new Node::Handler(name)) {}

Node::Node(const std::string &name)
    : handler(new Node::Handler(name.c_str())) {}

Node::Node(const std::string &name, const std::string &content)
    : handler(new Node::Handler(name.c_str(), content.c_str())) {}

Node::Node(const Node &bindFrom) : handler(bindFrom.handler) {}

Node::Node(const char *name, const char *content)
    : handler(new Node::Handler(name, content)) {}

Node::~Node() {}

Node &Node::operator=(Node &rhs)
{
  this->handler = rhs.handler;
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
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return std::string(this->get_parent()->handler->get_content());
}

void Node::ContentPropertyType::set_content(const std::string &rhs)
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->set_content(rhs);
}

void Node::ContentPropertyType::set_content(const char *rhs, std::size_t size)
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->set_content(rhs, size);
}

void Node::ContentPropertyType::set_content(const char *rhs)
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->set_content(rhs);
}

Node *Node::NamePropertyType::get_parent() const
{
  static const int offset = offsetof(Node, name);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::NamePropertyType::operator const std::string() const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->get_parent()->handler->get_name();
}

const std::string &Node::NamePropertyType::operator=(const std::string &rhs)
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  this->get_parent()->handler->set_name(rhs);
  return rhs;
}

Node *Node::CountPropertyType::get_parent() const
{
  static const int offset = offsetof(Node, count);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::CountPropertyType::operator std::size_t() const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->get_parent()->handler->get_count();
}

Node::iterator Node::begin()
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->begin();
}

Node::iterator Node::end()
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->end();
}

Node::iterator Node::begin() const
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->begin();
}

Node::iterator Node::end() const
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->end();
}

Node Node::operator[](const char *const key) const
{
  if (this->handler == nullptr)
  {
    return Node(std::shared_ptr<Node::Handler>(nullptr));
  }

  return this->handler->get_child(key);
}

Node Node::operator[](const std::string &key) const
{
  if (this->handler == nullptr)
  {
    return Node(std::shared_ptr<Node::Handler>(nullptr));
  }

  return this->handler->get_child(key);
}

Node Node::operator[](int index) const
{
  if (this->handler == nullptr)
  {
    return Node(std::shared_ptr<Node::Handler>(nullptr));
  }

  return this->handler->get_child(index);
}

bool Node::operator==(const Node &rhs) const
{
  return this->handler == rhs.handler;
}

bool Node::operator==(const void *const rhs) const
{
  if (this->handler == nullptr)
  {
    return rhs == nullptr;
  }

  return this->handler->handler == rhs;
}

void Node::remove(const Node &node)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Node not found");
  }

  this->handler->remove(node);
}

void Node::push_back(const Node &node)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->handler->push_back(node);
}

void Node::push_front(const Node &node)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->handler->push_front(node);
}

Node Node::pop_back()
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->handler->pop_back();
}

Node Node::pop_front()
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->handler->pop_front();
}

/////////////// Node attribute
bool Node::Attribute::operator==(decltype(nullptr)) const
{
  return this->handler == nullptr;
}

Node::Attribute Node::get_attribute_from_index(int index)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->get_attribute_from_index(index);
}

Node::Attribute Node::get_attribute_from_name(const char *name)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->get_attribute_from_name(name);
}

Node::Attribute Node::get_attribute_from_name(const std::string &name)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  return this->handler->get_attribute_from_name(name);
}

Node::Attribute &Node::Attribute::operator=(const char *val)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  this->handler->set_value(val);
  return *this;
}

Node::Attribute &Node::Attribute::operator=(const std::string &val)
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }
  this->handler->set_value(val);
  return *this;
}

Node::Attribute::operator std::string() const
{
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return std::string(this->handler->get_value());
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
  return std::strncmp(this->get_parent()->handler->get_name(), rhs.c_str(), rhs.length()) == 0;
}

bool Node::Attribute::NamePropertyType::operator==(const char *rhs) const
{
  return std::strcmp(this->get_parent()->handler->get_name(), rhs) == 0;
}

Node::Attribute::NamePropertyType::operator const char *() const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->handler->get_name();
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
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->set_value(val);
  return *this;
}

Node::Attribute::ValuePropertyType &Node::Attribute::ValuePropertyType::
operator=(const std::string &val)
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->set_value(val);
  return *this;
}

Node::Attribute::ValuePropertyType::operator const char *() const
{
  return this->get_parent()->handler->get_value();
}
/////// Node::Attribute::Value

/////////////// END Node attribute

std::size_t Node::AttributesPropertyType::get_count() const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->handler->get_attributes_size();
}

Node *Node::AttributesPropertyType::get_parent() const
{
  static const int offset = offsetof(Node, attributes);
  return (Node *)(((uint8_t *)this) - offset);
}

Node::AttributesPropertyType *
Node::AttributesPropertyType::CountPropertyType::get_parent() const
{
  static const int offset = offsetof(Node::AttributesPropertyType, count);
  return (Node::AttributesPropertyType *)(((uint8_t *)this) - offset);
}

Node::AttributesPropertyType::CountPropertyType::operator std::size_t() const
{
  return this->get_parent()->get_count();
}

Node::Attribute Node::AttributesPropertyType::operator[](int index) const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->get_attribute_from_index(index);
}

Node::Attribute Node::AttributesPropertyType::
operator[](const char *name) const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->get_parent()->get_attribute_from_name(name);
}

Node::Attribute Node::AttributesPropertyType::
operator[](const std::string &name) const
{
  if (this->get_parent()->handler == nullptr)
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
  if (this->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return this->handler->is_attributes_empty();
}

bool Node::AttributesPropertyType::get_is_empty() const
{
  if (this->get_parent()->handler == nullptr)
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
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->push_back_attribute(attr);
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
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->push_back_attribute(name, name_size, value, value_size);
}

void Node::AttributesPropertyType::push_back(const char *name, const char *value)
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  this->get_parent()->handler->push_back_attribute(name, value);
}

void Node::AttributesPropertyType::remove(const char * const name)
{
  this->get_parent()->handler->remove_attribute(name);
}

Node::AttributesPropertyType::iterator Node::AttributesPropertyType::begin()
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->handler->begin_attr());
}

const Node::AttributesPropertyType::iterator
Node::AttributesPropertyType::begin() const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->handler->begin_attr());
}

Node::AttributesPropertyType::iterator Node::AttributesPropertyType::end()
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->handler->end_attr());
}

const Node::AttributesPropertyType::iterator
Node::AttributesPropertyType::end() const
{
  if (this->get_parent()->handler == nullptr)
  {
    throw std::runtime_error("Null object");
  }

  return Node::AttributesPropertyType::iterator(
      this->get_parent()->handler->end_attr());
}

} // namespace Dom
} // namespace Xml
} // namespace un
