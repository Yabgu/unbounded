// Copyright 2016 Abdurrahim Cakar
/**
 * @file NodeHandlerLibxml2.h
 * @date Oct 10, 2012
 * @author Abdurrahim Cakar <abdurrahimcakar@gmail.com>
 * @brief Xml node handler class using libxml2
 */

#pragma once

#include <Xml/Dom/Node.h>
#include <cstring>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <list>
#include <memory>
#include <string>

namespace un
{
namespace Xml
{
namespace Dom
{

// TODO: Give option to either throw exception or give ability to create new node
static Node empty_node;

class Document;

class Node::Handler
{
public:
  class iterator;
  friend class Node::Handler::iterator;

private:
  friend class ::un::Xml::Dom::Node;
  friend class ::un::Xml::Dom::Document;
  xmlNodePtr handler;
  bool is_owner;

  std::list<Node> nodes;

private:
  static xmlNodePtr get_child_rec(xmlNodePtr p, const char *const key)
  {
    const char *inner_key = std::strchr(key, '/');

    if (inner_key)
    {
      for (xmlNodePtr i = p->children; i != NULL; i = i->next)
      {
        if (xmlNodeIsText(i))
        {
          continue;
        }

        if (xmlStrncmp(i->name, BAD_CAST key, static_cast<int>(inner_key - key)) == 0)
        {
          return get_child_rec(i, inner_key + 1);
        }
      }
    }
    else
    {
      for (xmlNodePtr i = p->children; i != NULL; i = i->next)
      {
        if (xmlNodeIsText(i))
        {
          continue;
        }

        if (xmlStrcmp(i->name, BAD_CAST key) == 0)
        {
          return i;
        }
      }
    }

    return NULL;
  }

  static xmlNodePtr get_child(xmlNodePtr node, int index)
  {
    if (index >= 0)
    {
      int ind = 0;
      for (xmlNodePtr i = node->children; i != NULL; i = i->next)
      {
        if (xmlNodeIsText(i))
        {
          continue;
        }

        if (ind == index)
        {
          return i;
        }
        ++ind;
      }
    }
    return NULL;
  }

  Node &_get_child(xmlNodePtr node)
  {
    if (this->handler->children == NULL)
    {
      throw std::runtime_error("Node has no children");
    }

    bool weHave = false;
    for (xmlNodePtr i = handler->children; i != NULL; i = i->next)
    {
      if (node == i)
      {
        weHave = true;
        break;
      }
    }

    if (!weHave)
    {
      throw std::runtime_error("Node is not child of this node.");
    }

    if (!nodes.empty())
    {
      for (Node &n : nodes)
      {
        if (n.handler->handler == node)
        {
          return n;
        }
      }
    }

    Node result(std::shared_ptr<Node::Handler>(new Node::Handler(node, false)));

    nodes.push_back(result);

    return nodes.back();
  }

public:
  Handler()
    : handler(NULL), is_owner(false) {}

  Handler(xmlNodePtr node, bool is_owner)
    : handler(node), is_owner(is_owner) {}

  explicit Handler(const char *name) : handler(NULL), is_owner(false)
  {
    if (name == NULL)
    {
      throw std::runtime_error("Cannot create nameless node");
    }

    handler = xmlNewNode(NULL, BAD_CAST name);

    if (handler == NULL)
    {
      throw std::runtime_error("xmlNewNode failed");
    }

    this->is_owner = true;
  }

  Handler(const char *name, const char *content)
      : handler(NULL), is_owner(false)
  {
    handler = xmlNewNode(NULL, BAD_CAST name);

    if (handler == NULL)
    {
      throw std::runtime_error("xmlNewNode failed");
    }

    xmlNodeSetContent(handler, BAD_CAST content);

    this->is_owner = true;
  }

  ~Handler()
  {
    if (this->is_owner && handler != NULL)
    {
      xmlFreeNode(handler);
    }
  }

  const std::string get_content() const
  {
    char *_cont = reinterpret_cast<char *>(xmlNodeGetContent(handler));
    if (_cont == NULL)
    {
      return std::string();
    }

    std::string result(_cont);
    xmlFree(_cont);

    return result;
  }

  void set_content(const std::string &content)
  {
    xmlNodeSetContent(handler, BAD_CAST content.c_str());
  }

  void set_content(const char *cont)
  {
    xmlNodeSetContent(handler, BAD_CAST cont);
  }

  void set_content(const char *cont, std::size_t size)
  {
    this->set_content(std::string(cont, size));
  }

  const std::string get_name() const
  {
    if (handler->name != NULL)
    {
		  return std::string(reinterpret_cast<const char *>(handler->name));
    }

	  return std::string();
  }

  void set_name(const std::string &name)
  {
    xmlNodeSetName(handler, BAD_CAST name.c_str());
  }

  class iterator : public Node::iterator_base
  {
  private:
    Node::Handler *_current_node;
    xmlNodePtr _ptr;
    xmlNodePtr _first_ptr;
    xmlNodePtr _last_ptr;
    bool _is_last;

  public:
    iterator(Node::Handler *current_node, xmlNodePtr ptr, xmlNodePtr first_ptr, xmlNodePtr last_ptr, bool is_last)
      : _current_node(current_node),
        _ptr(ptr),
        _first_ptr(first_ptr),
        _last_ptr(last_ptr),
        _is_last(is_last)
    {
      if (!is_last && ptr != NULL && xmlIsBlankNode(_ptr))
      {
        this->seek_to_next();
      }
    }

    void seek_to_next()
    {
      do
      {
        if (_ptr == NULL)
        {
          if (this->_is_last)
          {
            throw std::runtime_error("End of nodes reached");
          }
          else
          {
            this->_is_last = true;
          }
        }
        else
        {
          _ptr = _ptr->next;
        }
      } while (xmlIsBlankNode(_ptr));
    }

    void seek_to_previous()
    {
      do
      {
        if (_ptr == _first_ptr || _ptr == nullptr)
        {
          throw std::runtime_error("Start of nodes reached");
        }
        else
        {
          if (this->_is_last)
          {
            this->_is_last = false;
            _ptr = this->_last_ptr;
          }
          else
          {
            _ptr = _ptr->prev;
          }
        }
      } while (xmlIsBlankNode(_ptr));
    }

    bool is_beginning() const { return this->_ptr == _first_ptr; }

    bool is_end() const { return this->_is_last; }

    Node &get_node()
    {
      if (this->_is_last)
      {
        throw std::runtime_error("End of nodes reached");
      }
      return _current_node->_get_child(this->_ptr);
    }

    const void *get_address() const { return (const void *)_ptr; }

    bool is_equal(Node::iterator_base *other) const
    {
      return this->get_address() == other->get_address();
    }
  };

  Node::iterator begin()
  {
    return Node::iterator(new Node::Handler::iterator(
        this, handler->children, handler->children, handler->last, false));
  }

  Node::iterator end()
  {
    return Node::iterator(new Node::Handler::iterator(
        this, NULL, handler->children, handler->last, true));
  }

  Node &get_child(int index)
  {
    if (index < 0)
    {
      throw std::runtime_error("negative index");
    }
    xmlNodePtr node = get_child(this->handler, index);
    return _get_child(node);
  }

  Node &get_child(const char *const key)
  {
    xmlNodePtr node = get_child_rec(this->handler, key);
    if (node == NULL)
    {
      return empty_node;
    }
    return _get_child(node);
  }

  Node &get_child(const std::string &key)
  {
    xmlNodePtr node = get_child_rec(this->handler, key.c_str());
    if (node == NULL)
    {
      return empty_node;
    }
    return _get_child(node);
  }

  inline std::size_t get_count() const
  {
    std::size_t result = 0;

    for (xmlNodePtr i = handler->children; i != NULL; i = i->next)
    {
      if (!xmlNodeIsText(i))
      {
        ++result;
      }
    }
    return result;
  }

  bool is_equal(const Node &rhs) const
  {
    return this->handler == rhs.handler->handler;
  }

  void remove(const Node &node)
  {
    if (node.handler->is_owner)
    {
      throw std::runtime_error("Node is not belong(child node of) to any document or node");
    }

    for (xmlNodePtr i = handler->children; i != NULL; i = i->next)
    {
      if (node.handler->handler == i)
      {
        node.handler->is_owner = true;
        xmlUnlinkNode(i);
      }
    }
  }

  void push_back(const Node &node)
  {
    if (!node.handler->is_owner)
    {
      throw std::runtime_error("Node is owned by another document or node");
    }

    if (xmlAddChild(this->handler, node.handler->handler) !=
        node.handler->handler)
    {
      throw std::runtime_error("xmlAddChild failed");
    }
    else
    {
      node.handler->is_owner = false;
      if (!node.handler.unique())
      {
        nodes.push_back(node);
      }
    }
  }

  void push_front(const Node &node)
  {
    if (!node.handler->is_owner)
    {
      throw std::runtime_error("Node is owned by another document or node");
    }

    if (this->handler->children == NULL)
    {
      this->push_back(node);
    }
    else
    {
      if (xmlAddPrevSibling(this->handler->children,
                            node.handler->handler) != NULL)
      {
        node.handler->is_owner = false;
        if (!node.handler.unique())
        {
          nodes.push_back(node);
        }
      }
      else
      {
        throw std::runtime_error("xmlAddPrevSibling failed");
      }
    }
  }

  Node &pop_back()
  {
    Node &last = this->_get_child(this->handler->last);
    this->remove(last);
    return last;
  }

  Node &pop_front()
  {
    Node &first = this->_get_child(this->handler->children);
    this->remove(first);
    return first;
  }

  class Attribute : public Node::AttributeBase
  {
  private:
    xmlAttrPtr handler;
    friend class Node::Handler;
    bool is_owner; // true if attribute is removed from node

  public:
    explicit Attribute(xmlAttrPtr handler, bool owner = false)
        : handler(handler), is_owner(owner) {}

    ~Attribute()
    {
      if (this->is_owner)
      {
        xmlFree(this->handler);
      }
    }

    virtual const char *get_name() const
    {
      return (const char *)this->handler->name;
    }

    virtual const char *get_value() const
    {
      return (const char *)this->handler->children->content;
    }

    virtual void set_value(const char *val)
    {
      xmlNodeSetContent(this->handler->children, BAD_CAST val);
    }

    virtual void set_value(const std::string &val)
    {
      xmlNodeSetContent(this->handler->children, BAD_CAST val.c_str());
    }
  };

  Node::Attribute get_attribute_from_name(const char *name)
  {
    for (xmlAttrPtr attr = this->handler->properties; attr != NULL;
         attr = attr->next)
    {
      if (xmlStrcmp(attr->name, BAD_CAST name) == 0)
      {
        return Node::Attribute(std::shared_ptr<Node::Handler::Attribute>(
            new Node::Handler::Attribute(attr)));
      }
    }

    // throw std::runtime_error("Attribute not found");
    return Node::Attribute(std::shared_ptr<AttributeBase>());
  }

  Node::Attribute get_attribute_from_name(const std::string &name)
  {
    return this->get_attribute_from_name(name.c_str());
  }

  Node::Attribute get_attribute_from_index(int index)
  {
    xmlAttrPtr attr = this->handler->properties;

    if (index < 0)
    {
      throw std::runtime_error("Negative range");
    }

    for (int i = 1; i <= index; ++i)
    {
      if (attr == NULL)
      {
        throw std::runtime_error("Out of range");
      }
      attr = attr->next;
    }

    return Node::Attribute(std::shared_ptr<Node::Handler::Attribute>(
        new Node::Handler::Attribute(attr)));
  }

  bool is_attributes_empty() const
  {
    return this->handler->properties == NULL;
  }

  std::size_t get_attributes_size() const
  {
    std::size_t result = 0;
    for (xmlAttrPtr attr = this->handler->properties; attr != NULL; attr = attr->next)
    {
      ++result;
    }

    return result;
  }

  void push_back_attribute(const Node::Attribute &attr)
  {
    xmlAttrPtr newattr = xmlNewProp(
        this->handler, (const xmlChar *)attr.name.operator const char *(),
        (const xmlChar *)attr.value.operator const char *());

    if (newattr == NULL)
    {
      throw std::runtime_error("Can not add new attribute");
    }
  }

  void push_back_attribute(const std::string &name, const std::string &value)
  {
    xmlAttrPtr newattr =
        xmlNewProp(this->handler, (const xmlChar *)name.c_str(),
                   (const xmlChar *)value.c_str());

    if (newattr == NULL)
    {
      throw std::runtime_error("Can not add new attribute");
    }
  }

  void push_back_attribute(const char *name, std::size_t namesize, const char *value, std::size_t valuesize)
  {
    std::string _name(name, namesize);
    std::string _value(value, valuesize);

    this->push_back_attribute(_name, _value);
  }

  void push_back_attribute(const char *name, const char *value)
  {
    xmlAttrPtr newattr = xmlNewProp(this->handler, (const xmlChar *)name, (const xmlChar *)value);

    if (newattr == NULL)
    {
      throw std::runtime_error("Can not add new attribute");
    }
  }

  bool remove_attribute(const char * const name)
  {
    for (auto i = this->handler->properties; i; i = i->next) {
      // TODO: Test against encodings etc
      if (i->type == XML_ATTRIBUTE_NODE
        && xmlStrEqual((xmlChar const * const)name, i->name)) {
        xmlRemoveProp(i);
        break;
      }
    }
  }

  class AttributeIterator : public Node::AttributesPropertyType::iterator_base
  {
  private:
    xmlAttrPtr attr;
    xmlAttrPtr begin;
    xmlAttrPtr end;
    bool is_end;

  public:
    AttributeIterator(xmlAttrPtr attr, xmlAttrPtr begin, xmlAttrPtr end, bool is_end)
      : attr(attr), begin(begin), end(end), is_end(is_end) {}

    Node::Attribute get_node_attr()
    {
      return Node::Attribute(std::shared_ptr<Node::Handler::Attribute>(
        new Node::Handler::Attribute(attr)));
    }

    const Node::Attribute get_node_attr() const
    {
      return Node::Attribute(std::shared_ptr<Node::Handler::Attribute>(
        new Node::Handler::Attribute(attr)));
    }

    virtual void seek_to_next()
    {
      if (this->attr == NULL)
      {
        if (!this->is_end)
        {
          this->attr = this->begin;
        }
        else
        {
          throw std::runtime_error("Iterator reached end allready");
        }
      }
      else
      {
        this->attr = this->attr->next;

        if (this->attr == NULL)
        {
          this->is_end = true;
        }
      }
    }

    virtual void seek_to_previous()
    {
      if (this->is_end)
      {
        this->attr = this->end;
        this->is_end = false;
      }
      else
      {
        if (this->attr == NULL)
        {
          throw std::runtime_error("Cannot seek from first to below first!");
        }
        else if (this->attr == this->begin)
        {
          this->attr = NULL;
        }
        else
        {
          this->attr = this->attr->prev;
        }
      }
    }

    void *get_pointer() const { return attr; }

    bool
    is_equal_to(const Node::AttributesPropertyType::iterator_base *rhs) const
    {
      return this->get_pointer() == rhs->get_pointer();
    }
  };

  std::shared_ptr<Node::AttributesPropertyType::iterator_base> begin_attr()
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::Handler::AttributeIterator(
            this->handler->properties, this->handler->properties,
            (xmlAttrPtr)this->handler->properties->last, false));
  }

  const std::shared_ptr<Node::AttributesPropertyType::iterator_base>
  begin_attr() const
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::Handler::AttributeIterator(
            this->handler->properties, this->handler->properties,
            (xmlAttrPtr)this->handler->properties->last, false));
  }

  std::shared_ptr<Node::AttributesPropertyType::iterator_base> end_attr()
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::Handler::AttributeIterator(
            NULL, this->handler->properties,
            (xmlAttrPtr)this->handler->properties->last, true));
  }

  const std::shared_ptr<Node::AttributesPropertyType::iterator_base>
  end_attr() const
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::Handler::AttributeIterator(
            NULL, this->handler->properties,
            (xmlAttrPtr)this->handler->properties->last, true));
  }
};

} // namespace Dom
} // namespace Xml
} // namespace un
