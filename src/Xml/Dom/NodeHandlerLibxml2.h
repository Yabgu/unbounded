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

class Node::_Handler
{
public:
  class iterator;
  friend class Node::_Handler::iterator;

private:
  friend class ::un::Xml::Dom::Node;
  friend class ::un::Xml::Dom::Document;
  xmlNodePtr _handler;
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
    if (this->_handler->children == NULL)
    {
      throw std::runtime_error("Node has no children");
    }

    bool weHave = false;
    for (xmlNodePtr i = _handler->children; i != NULL; i = i->next)
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
        if (n._handler->_handler == node)
        {
          return n;
        }
      }
    }

    Node result(
        std::shared_ptr<Node::_Handler>(new Node::_Handler(node, false)));

    nodes.push_back(result);

    return nodes.back();
  }

public:
  _Handler() : _handler(NULL), is_owner(false) {}

  _Handler(xmlNodePtr node, bool is_owner)
      : _handler(node), is_owner(is_owner) {}

  explicit _Handler(const char *name) : _handler(NULL), is_owner(false)
  {
    if (name == NULL)
    {
      throw std::runtime_error("Cannot create nameless node");
    }

    _handler = xmlNewNode(NULL, BAD_CAST name);

    if (_handler == NULL)
    {
      throw std::runtime_error("xmlNewNode failed");
    }

    this->is_owner = true;
  }

  _Handler(const char *name, const char *content)
      : _handler(NULL), is_owner(false)
  {
    _handler = xmlNewNode(NULL, BAD_CAST name);

    if (_handler == NULL)
    {
      throw std::runtime_error("xmlNewNode failed");
    }

    xmlNodeSetContent(_handler, BAD_CAST content);

    this->is_owner = true;
  }

  ~_Handler()
  {
    if (this->is_owner && _handler != NULL)
    {
      // After owner mechanism we don´t need unlinking
      /*for (xmlNodePtr c = _handler->children; c != NULL; c = c->next)
       xmlUnlinkNode(c);*/

      xmlFreeNode(_handler);
    }
  }

  const std::string get_content() const
  {
    char *_cont = reinterpret_cast<char *>(xmlNodeGetContent(_handler));
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
    xmlNodeSetContent(_handler, BAD_CAST content.c_str());
  }

  void set_content(const char *cont)
  {
    xmlNodeSetContent(_handler, BAD_CAST cont);
  }

  void set_content(const char *cont, std::size_t size)
  {
    this->set_content(std::string(cont, size));
  }

  const std::string get_name() const
  {
    if (_handler->name != NULL)
    {
		return std::string(reinterpret_cast<const char *>(_handler->name));
    }

	return std::string();
  }

  void set_name(const std::string &name)
  {
    xmlNodeSetName(_handler, BAD_CAST name.c_str());
  }

  class iterator : public Node::iterator_base
  {
  private:
    Node::_Handler *_current_node;
    xmlNodePtr _ptr;
    xmlNodePtr _first_ptr;
    xmlNodePtr _last_ptr;
    bool _is_last;

  public:
    iterator(Node::_Handler *current_node, xmlNodePtr ptr, xmlNodePtr first_ptr,
             xmlNodePtr last_ptr, bool is_last)
        : _current_node(current_node), _ptr(ptr), _first_ptr(first_ptr),
          _last_ptr(last_ptr), _is_last(is_last)
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
    return Node::iterator(new Node::_Handler::iterator(
        this, _handler->children, _handler->children, _handler->last, false));
  }

  Node::iterator end()
  {
    return Node::iterator(new Node::_Handler::iterator(
        this, NULL, _handler->children, _handler->last, true));
  }

  Node &get_child(int index)
  {
    if (index < 0)
    {
      throw std::runtime_error("negative index");
    }
    xmlNodePtr node = get_child(this->_handler, index);
    return _get_child(node);
  }

  Node &get_child(const char *const key)
  {
    xmlNodePtr node = get_child_rec(this->_handler, key);
    if (node == NULL)
    {
      return empty_node;
    }
    return _get_child(node);
  }

  Node &get_child(const std::string &key)
  {
    xmlNodePtr node = get_child_rec(this->_handler, key.c_str());
    if (node == NULL)
    {
      return empty_node;
    }
    return _get_child(node);
  }

  inline std::size_t get_count() const
  {
    std::size_t result = 0;

    for (xmlNodePtr i = _handler->children; i != NULL; i = i->next)
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
    return this->_handler == rhs._handler->_handler;
  }

  void remove(const Node &node)
  {
    if (node._handler->is_owner)
    {
      throw std::runtime_error(
          "Node is not belong(child node of) to any document or node");
    }

    for (xmlNodePtr i = _handler->children; i != NULL; i = i->next)
    {
      if (node._handler->_handler == i)
      {
        node._handler->is_owner = true;
        xmlUnlinkNode(i);
      }
    }
  }

  void push_back(const Node &node)
  {
    if (!node._handler->is_owner)
    {
      throw std::runtime_error("Node is owned by another document or node");
    }

    if (xmlAddChild(this->_handler, node._handler->_handler) !=
        node._handler->_handler)
    {
      throw std::runtime_error("xmlAddChild failed");
    }
    else
    {
      node._handler->is_owner = false;
      if (!node._handler.unique())
      {
        nodes.push_back(node);
      }
    }
  }

  void push_front(const Node &node)
  {
    if (!node._handler->is_owner)
    {
      throw std::runtime_error("Node is owned by another document or node");
    }

    if (this->_handler->children == NULL)
    {
      this->push_back(node);
    }
    else
    {
      if (xmlAddPrevSibling(this->_handler->children,
                            node._handler->_handler) != NULL)
      {
        node._handler->is_owner = false;
        if (!node._handler.unique())
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
    Node &last = this->_get_child(this->_handler->last);
    this->remove(last);
    return last;
  }

  Node &pop_front()
  {
    Node &first = this->_get_child(this->_handler->children);
    this->remove(first);
    return first;
  }

  class Attribute : public Node::AttributeBase
  {
  private:
    xmlAttrPtr _handler;
    friend class Node::_Handler;
    bool is_owner; // true if attribute is removed from node

  public:
    explicit Attribute(xmlAttrPtr handler, bool owner = false)
        : _handler(handler), is_owner(owner) {}

    ~Attribute()
    {
      if (this->is_owner)
      {
        xmlFree(this->_handler);
      }
    }

    virtual const char *get_name() const
    {
      return (const char *)this->_handler->name;
    }

    virtual const char *get_value() const
    {
      return (const char *)this->_handler->children->content;
    }

    virtual void set_value(const char *val)
    {
      xmlNodeSetContent(this->_handler->children, BAD_CAST val);
    }

    virtual void set_value(const std::string &val)
    {
      xmlNodeSetContent(this->_handler->children, BAD_CAST val.c_str());
    }
  };

  Node::Attribute get_attribute_from_name(const char *name)
  {
    for (xmlAttrPtr attr = this->_handler->properties; attr != NULL;
         attr = attr->next)
    {
      if (xmlStrcmp(attr->name, BAD_CAST name) == 0)
      {
        return Node::Attribute(std::shared_ptr<Node::_Handler::Attribute>(
            new Node::_Handler::Attribute(attr)));
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
    xmlAttrPtr attr = this->_handler->properties;

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

    return Node::Attribute(std::shared_ptr<Node::_Handler::Attribute>(
        new Node::_Handler::Attribute(attr)));
  }

  bool is_attributes_empty() const
  {
    return this->_handler->properties == NULL;
  }

  std::size_t get_attributes_size() const
  {
    std::size_t result = 0;
    for (xmlAttrPtr attr = this->_handler->properties; attr != NULL; attr = attr->next)
    {
      ++result;
    }

    return result;
  }

  void push_back_attribute(const Node::Attribute &attr)
  {
    xmlAttrPtr newattr = xmlNewProp(
        this->_handler, (const xmlChar *)attr.name.operator const char *(),
        (const xmlChar *)attr.value.operator const char *());

    if (newattr == NULL)
    {
      throw std::runtime_error("Can not add new attribute");
    }
  }

  void push_back_attribute(const std::string &name, const std::string &value)
  {
    xmlAttrPtr newattr =
        xmlNewProp(this->_handler, (const xmlChar *)name.c_str(),
                   (const xmlChar *)value.c_str());

    if (newattr == NULL)
    {
      throw std::runtime_error("Can not add new attribute");
    }
  }

  void push_back_attribute(const char *name, std::size_t namesize,
                           const char *value, std::size_t valuesize)
  {
    std::string _name(name, namesize);
    std::string _value(value, valuesize);

    this->push_back_attribute(_name, _value);
  }

  void push_back_attribute(const char *name, const char *value)
  {
    xmlAttrPtr newattr = xmlNewProp(this->_handler, (const xmlChar *)name,
                                    (const xmlChar *)value);

    if (newattr == NULL)
    {
      throw std::runtime_error("Can not add new attribute");
    }
  }

  class attr_iterator : public Node::AttributesPropertyType::iterator_base
  {
  private:
    xmlAttrPtr attr;
    xmlAttrPtr begin;
    xmlAttrPtr end;
    bool is_end;

  public:
    attr_iterator(xmlAttrPtr attr, xmlAttrPtr begin, xmlAttrPtr end,
                  bool is_end)
        : attr(attr), begin(begin), end(end), is_end(is_end) {}

    Node::Attribute get_node_attr()
    {
      return Node::Attribute(std::shared_ptr<Node::_Handler::Attribute>(
          new Node::_Handler::Attribute(attr)));
    }

    const Node::Attribute get_node_attr() const
    {
      return Node::Attribute(std::shared_ptr<Node::_Handler::Attribute>(
          new Node::_Handler::Attribute(attr)));
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
        new Node::_Handler::attr_iterator(
            this->_handler->properties, this->_handler->properties,
            (xmlAttrPtr)this->_handler->properties->last, false));
  }

  const std::shared_ptr<Node::AttributesPropertyType::iterator_base>
  begin_attr() const
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::_Handler::attr_iterator(
            this->_handler->properties, this->_handler->properties,
            (xmlAttrPtr)this->_handler->properties->last, false));
  }

  std::shared_ptr<Node::AttributesPropertyType::iterator_base> end_attr()
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::_Handler::attr_iterator(
            NULL, this->_handler->properties,
            (xmlAttrPtr)this->_handler->properties->last, true));
  }

  const std::shared_ptr<Node::AttributesPropertyType::iterator_base>
  end_attr() const
  {
    return std::shared_ptr<Node::AttributesPropertyType::iterator_base>(
        new Node::_Handler::attr_iterator(
            NULL, this->_handler->properties,
            (xmlAttrPtr)this->_handler->properties->last, true));
  }
};

} // namespace Dom
} // namespace Xml
} // namespace un
