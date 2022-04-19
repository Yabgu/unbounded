// Copyright 2016 Abdurrahim Cakar
/**
 * @file Node.h
 * @date Oct 10, 2012
 * @author Abdurrahim Cakar <abdurrahimcakar@gmail.com>
 * @brief Xml node class
 */

#pragma once

#include <memory>
#include <string>
#include <cstring>

namespace un::Xml::Dom
{

struct Document;

/**
 * Xml Node class.
 *
 * This class is designed as a binding holder for Node::Handler class
 */
struct Node
{
  friend struct ::un::Xml::Dom::Document;
  class Handler;
  std::shared_ptr<Node::Handler> handler;

  Node(const std::shared_ptr<Node::Handler> &_h);

  /**
   * Content property class
   * Used for operations on node content
   */
  struct ContentPropertyType
  {
    Node *get_parent() const;
    /**
     * Set content
     *
     * @param rhs content to set to
     * @param size size of content
     */
    void set_content(const char *rhs, std::size_t size);

    /**
     * Set content
     *
     * @param rhs content to set to
     */
    void set_content(const std::string &rhs);

    /**
     * Set content
     *
     * @param rhs content to set to
     */
    void set_content(const char *rhs);

    /**
     * Get content as std::string. This will create new string object from the
     * content.
     *
     * @return newly created copy of content
     */
    operator const std::string() const;

    template <std::size_t strsize>
    inline auto operator=(const char (&rhs)[strsize]) -> decltype(rhs)
    {
      this->set_content(rhs, strsize - 1);
      return rhs;
    }

    inline const char *operator=(const char *rhs)
    {
      this->set_content(rhs);
      return rhs;
    }

    inline const std::string &operator=(const std::string &rhs)
    {
      this->set_content(rhs);
      return rhs;
    }

    template <class T>
    inline bool operator==(const T &rhs) const
    {
      return this->operator const std::string() == rhs;
    }

    template <class T>
    inline bool operator!=(const T &rhs) const
    {
      return !this->operator==(rhs);
    }
  };

  friend struct ContentPropertyType;

  /// Content property object. Just an interface to node class
  ContentPropertyType content;

  /**
   * Name property class
   */
  struct NamePropertyType
  {
    Node *get_parent() const;

    /**
     * Getter of content property. This will create new string object from the
     * content.
     *
     * @return newly created copy of content
     */
    operator const std::string() const;

    /**
     * Setter of content property.
     *
     * @return Returns rhs object as is
     */
    const std::string &operator=(const std::string &rhs);

    template <class T>
    inline bool operator==(const T &rhs) const
    {
      return this->operator const std::string() == rhs;
    }

    template <class T>
    inline bool operator!=(const T &rhs) const
    {
      return !this->operator==(this->get_parent()->name == rhs);
    }
  };

  friend struct NamePropertyType;
  /// Name property object. Just an interface to node class
  NamePropertyType name;

  /**
   * Count property for number of first child nodes
   */
  struct CountPropertyType
  {
    Node *get_parent() const;

    /**
     * Getter of count property
     *
     * @return Returns number of first child nodes
     */
    operator std::size_t() const;
  };

  friend struct SizePropertyType;
  /// Count property object. Just an interface to node class
  CountPropertyType count;

  /**
   * Iterator interface class of Xml::Node
   */
  class iterator_base
  {
  public:
    virtual ~iterator_base() {}

    virtual void seek_to_next() = 0;
    virtual void seek_to_previous() = 0;

    virtual bool is_beginning() const = 0;
    virtual bool is_end() const = 0;

    virtual Node &get_node() = 0;

    // To help finding equality
    virtual const void *get_address() const = 0;

    virtual bool is_equal(Node::iterator_base *other) const
    {
      return this->get_address() == other->get_address();
    }
  };

  /**
   * Iterator class of Xml::Node
   *
   * Handler will give iterator instance. This is just a holder class for the
   * iterator instance.
   */
  class iterator : private std::shared_ptr<iterator_base>
  {
  public:
    explicit iterator(iterator_base *rhs)
        : std::shared_ptr<iterator_base>(rhs) {}

    inline void operator++() { this->get()->seek_to_next(); }

    inline void operator--() { this->get()->seek_to_previous(); }

    inline Node &operator*() { return this->get()->get_node(); }

    inline Node *operator->() { return &this->get()->get_node(); }

    inline bool operator==(const iterator &rhs)
    {
      return this->get()->is_equal(rhs.get());
    }

    inline bool operator!=(const iterator &rhs)
    {
      return !this->get()->is_equal(rhs.get());
    }
  };

  /// Get node by name
  Node operator[](const char *const key) const;

  /// Get node by name
  Node operator[](const std::string &key) const;

  /// Get node by index
  Node operator[](int index) const;

  /**
   * Remove node from childs list. This will just unbind from this node and will
   * make it free.
   */
  void remove(const Node &node);

  /**
   * Appends (Binds) node to end. Will make node child to current node
   */
  void push_back(const Node &node);

  /**
   * Appends (Binds) node to begining. Will make node child to current node
   */
  void push_front(const Node &node);

  /// Unbind last element
  Node pop_back();

  /// Unbind first element
  Node pop_front();

  /**
   * Xml Attribute interface class. Just a string key-value pair.
   */
  class AttributeBase
  {
  public:
    virtual ~AttributeBase() {}

    virtual const char *get_name() const = 0;
    virtual const char *get_value() const = 0;

    virtual void set_value(const char *val) = 0;
    virtual void set_value(const std::string &val) = 0;
  };

  /**
   * Xml attribute class. Just a binding host and interface for xml attribute
   * instance.
   */
  struct Attribute
  {
    std::shared_ptr<AttributeBase> handler;

    explicit Attribute(const std::shared_ptr<AttributeBase> &handler)
      : handler(handler) {}

    Attribute &operator=(const char *val);
    Attribute &operator=(const std::string &val);
    operator std::string() const;

    /**
     * Node::Attribute name property class
     */
    struct NamePropertyType
    {
      Attribute *get_parent() const;

      bool operator==(const std::string &rhs) const;
      bool operator==(const char *rhs) const;
      operator const char *() const;
      inline operator std::string() const
      {
        return std::string(operator const char *());
      }
    };

    friend struct Attribute::NamePropertyType;
    /// Name property object. Just an interface to node class
    NamePropertyType name;

    /**
     * Node::Attribute value property class
     */
    struct ValuePropertyType
    {
      Attribute *get_parent() const;

      Attribute::ValuePropertyType &operator=(const char *val);
      Attribute::ValuePropertyType &operator=(const std::string &val);
      operator const char *() const;

      inline bool operator==(const char *const rhs) const
      {
        if (this->get_parent()->operator==(nullptr))
        {
          return rhs == nullptr;
        }
        return std::strcmp(this->operator const char *(), rhs) == 0;
      }

      inline bool operator!=(const char *const rhs) const
      {
        return !this->operator==(rhs);
      }
    };

    friend struct Attribute::ValuePropertyType;
    /// Value property object. Just an interface to node class
    ValuePropertyType value;

    bool operator==(decltype(nullptr)) const;

    inline bool operator!=(decltype(nullptr)) const
    {
      return !this->operator==(nullptr);
    }

    inline bool operator==(const char *const rhs) const
    {
      return this->value.operator==(rhs);
    }

    inline bool operator!=(const char *const rhs) const
    {
      return !this->value.operator==(rhs);
    }
  };

  Node::Attribute get_attribute_from_index(int index);
  Node::Attribute get_attribute_from_name(const char *name);
  Node::Attribute get_attribute_from_name(const std::string &name);
  bool is_attributes_empty() const;

  /// Node::Attributes property class
  struct AttributesPropertyType
  {
    Node *get_parent() const;
    std::size_t get_count() const;
    bool get_is_empty() const;

    /// Node::Attributes::Count property class
    struct CountPropertyType
    {
      Node::AttributesPropertyType *get_parent() const;

      operator std::size_t() const;
    };

    friend struct SizePropertyType;
    /// Count property object. Just an interface to node class
    CountPropertyType count;

    /// Get attribute by index
    Node::Attribute operator[](int index) const;

    /// Get attribute from name
    Node::Attribute operator[](const char *name) const;

    /// Get attribute from name
    Node::Attribute operator[](const std::string &name) const;

    /// Node::Attributes::IsEmpty property class
    struct IsEmptyPropertyType
    {
      Node::AttributesPropertyType *get_parent() const;

      /**
       * Returns true if there is no attribute for current node
       */
      operator bool() const;
    };

    friend struct IsEmptyPropertyType;
    /// IsEmpty property object. Just an interface to node class
    IsEmptyPropertyType is_empty;

    /**
     * Add new attribute to current attributes list
     */
    void push_back(const Node::Attribute &);
    void push_back(const std::string &name, const std::string &value);
    void push_back(const char *name, std::size_t name_size, const char *value, std::size_t value_size);
    void push_back(const char *name, const char *value);

    template <std::size_t namesize, std::size_t valuesize>
    void push_back(const char (&name)[namesize], const char (&value)[valuesize])
    {
      static_assert(namesize > 0, "Name cannot be empty");
      this->push_back(name, namesize, value, valuesize);
    }

    void remove(const char * const name);

    /**
     * Attributes iterator interface class
     */
    struct iterator_base
    {
      virtual ~iterator_base() {}

      virtual Node::Attribute get_node_attr() = 0;
      virtual const Node::Attribute get_node_attr() const = 0;

      virtual void seek_to_next() = 0;
      virtual void seek_to_previous() = 0;

      virtual void *get_pointer() const = 0;

      virtual bool is_equal_to(const iterator_base *) const = 0;
    };

    struct iterator
    {
      std::shared_ptr<iterator_base> handler;

      explicit iterator(const std::shared_ptr<iterator_base> &iterator_handler)
          : handler(iterator_handler) {}

      void operator++() { this->handler->seek_to_next(); }

      void operator--() { this->handler->seek_to_previous(); }

      Node::Attribute operator*() { return this->handler->get_node_attr(); }

      const Node::Attribute operator*() const
      {
        return this->handler->get_node_attr();
      }

      bool operator==(const iterator &rhs) const
      {
        return this->handler->is_equal_to(rhs.handler.get());
      }

      bool operator!=(const iterator &rhs) const
      {
        return !this->handler->is_equal_to(rhs.handler.get());
      }
    };

    iterator begin();
    const iterator begin() const;

    iterator end();
    const iterator end() const;
  };

  friend struct AttributesPropertyType;
  AttributesPropertyType attributes;

  iterator begin();
  iterator end();

  iterator begin() const;
  iterator end() const;

  Node &operator=(Node &rhs);

  bool operator==(const void *const rhs) const;

  bool operator!=(const void *const rhs) const
  {
    return !this->operator==(rhs);
  }

  bool operator==(const Node &rhs) const;

  inline bool operator!=(const Node &rhs) const
  {
    return !this->operator==(rhs);
  }

  /**
   * Empty node constructor
   */
  Node();

  /**
   * New node with name and content
   */
  Node(const std::string &name, const std::string &content);

  /**
   * New node with name and content
   */
  explicit Node(const char *name, const char *content);

  /**
   * New node with name
   */
  explicit Node(const char *name);

  /**
   * New node with name
   */
  Node(const std::string &name);

  /**
   * Binds another node to this object
   */
  Node(const Node &bindFrom);
};

}
