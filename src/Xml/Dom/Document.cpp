// Copyright 2016 Abdurrahim Cakar
/**
 * @file Document.cpp
 * @date Oct 10, 2012
 * @author Abdurrahim Cakar <abdurrahimcakar@gmail.com>
 * @brief Xml dom parser document class
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
  : handler(new Document::Handler(version)) {}

Document::Document(const std::string &version)
  : handler(new Document::Handler(version.c_str())) {}

Document::RootNodePropertyType::RootNodePropertyType()
  : ::un::Xml::Dom::Node(std::shared_ptr<::un::Xml::Dom::Node::Handler>(
    new ::un::Xml::Dom::Node::Handler(NULL, true))) {}

void Document::parse(const char *data, std::size_t size)
{
  this->handler->parse(data, size);
  this->root_node.handler->handler = NULL;
  this->root_node.handler->is_owner = false;
  this->handler->get_root_node(this->root_node);
}

void Document::parse_file(const char *path)
{
  this->handler->parse_file(path);
  this->root_node.handler.reset();
  this->handler->get_root_node(this->root_node);
}

Document *Document::RootNodePropertyType::get_parent() const
{
  static const int offset = offsetof(Document, root_node);
  return (Document *)(((uint8_t *)this) - offset);
}

Node &Document::RootNodePropertyType::get_node()
{
  return this->get_parent()->handler->get_root_node(
      this->get_parent()->root_node);
}

void Document::RootNodePropertyType::set_node(const Node &node)
{
  this->get_parent()->handler->set_root_node(this->get_parent()->root_node, node);
}

Node &Document::RootNodePropertyType::operator=(const Node &node)
{
  this->set_node(node);
  return this->get_node();
}

Document::operator std::string() const { return this->handler->as_string(); }

std::string Document::to_string(bool pretty_print, bool skip_headers) const
{
  return this->handler->as_string(pretty_print, skip_headers);
}

std::ostream &operator<<(std::ostream &_cout, const Document &val)
{
  val.handler->write_to(_cout);
  return _cout;
}

} // namespace Dom
} // namespace Xml
} // namespace un
