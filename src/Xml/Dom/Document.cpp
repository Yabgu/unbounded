// Copyright 2016 Abdurrahim Cakar
/**
 * @file Document.cpp
 * @date Oct 10, 2012
 * @author Abdurrahim Cakar <abdurrahimcakar@gmail.com>
 * @brief Xml document class
 *
 * Xml document class is designed C++ to ease up development with xml document
 * objects.
 *
 * Maybe I should create an opensource project for just xml. It is
 * getting bigger and documenting this started to took so much time.
 */

#include <Xml/Dom/Document.h>
#include "DocumentHandlerLibxml2.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>

namespace un
{
namespace Xml
{
namespace Dom
{

Document::Document(const char *version)
    : _handler(new Document::_Handler(version)) {}

Document::Document(const std::string &version)
    : _handler(new Document::_Handler(version.c_str())) {}

Document::~Document() {}

Document::RootNodePropertyType::RootNodePropertyType()
    : ::un::Xml::Dom::Node(std::shared_ptr<::un::Xml::Dom::Node::_Handler>(
          new ::un::Xml::Dom::Node::_Handler(NULL, true))) {}

void Document::parse(const char *data, std::size_t size)
{
  this->_handler->parse(data, size);
  this->root_node._handler->_handler = NULL;
  this->root_node._handler->is_owner = false;
  this->_handler->get_root_node(this->root_node);
}

void Document::parse_file(const char *path)
{
  this->_handler->parse_file(path);
  this->root_node._handler.reset();
  this->_handler->get_root_node(this->root_node);
}

Document *Document::RootNodePropertyType::get_parent() const
{
  static const int offset = offsetof(Document, root_node);
  return (Document *)(((uint8_t *)this) - offset);
}

Node &Document::RootNodePropertyType::get_node()
{
  return this->get_parent()->_handler->get_root_node(
      this->get_parent()->root_node);
}

void Document::RootNodePropertyType::set_node(const Node &node)
{
  this->get_parent()->_handler->set_root_node(this->get_parent()->root_node, node);
}

Node &Document::RootNodePropertyType::operator=(const Node &node)
{
  this->set_node(node);
  return this->get_node();
}

Document::operator std::string() const { return this->_handler->as_string(); }

std::string Document::to_string(bool pretty_print, bool skip_headers) const
{
  return this->_handler->as_string(pretty_print, skip_headers);
}

std::ostream &operator<<(std::ostream &_cout, const Document &val)
{
  val._handler->write_to(_cout);
  return _cout;
}

} // namespace Dom
} // namespace Xml
} // namespace un
