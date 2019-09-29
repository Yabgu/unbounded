// Copyright 2016 Abdurrahim Cakar
/**
 * @file XmlNode.h
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

#pragma once

#include "Node.h"
#include <memory>
#include <string>

namespace un
{
namespace Xml
{
namespace Dom
{
class Node;

class Document
{
private: // To allow dependency injection change this to protected
  class Handler;
  friend class Document::Handler;
  std::shared_ptr<Document::Handler> handler;

public:
  /**
   * Root node property class
   */
  class RootNodePropertyType : public Node
  {
  private:
    friend class Document;
    Document *get_parent() const;
    Node &get_node();
    void set_node(const Node &node);
    RootNodePropertyType();

  public:
    /**
     * Setter of root node.
     *
     * @return Returns root node of current document.
     */
    Node &operator=(const Node &node);
  };

  /// Root node property object. Just an interface to node class
  RootNodePropertyType root_node;

public:
  /**
   * Parse document from raw data (UTF-8 Encoding will be used)
   *
   * @param data Read data from.
   * @param size Size of data to read from
   */
  void parse(const char *data, std::size_t size);

  template <int size>
  inline void parse(const char (&data)[size])
  {
    static_assert(size > 1, "Size of data must be greater than one.");
    this->parse(static_cast<const char *>(data), size - 1);
  }

  inline void parse(const std::string &str)
  {
    this->parse(str.c_str(), str.length());
  }

  /**
   * Parse xml document from xml file
   *
   * @param path Xml document file path
   */
  void parse_file(const char *path);

  inline void parse_file(const std::string &path)
  {
    this->parse_file(path.c_str());
  }

public:
  /**
   * Create new xml document
   */
  explicit Document(const char *version);

  Document(const std::string &version = "1.0");

  virtual ~Document();

public:
  operator std::string() const;
  std::string to_string(bool pretty_print = false, bool skip_headers = true) const;

  friend std::ostream &operator<<(std::ostream &_cout, const Document &val);
};

std::ostream &operator<<(std::ostream &_cout, const Document &val);

} // namespace Dom
} // namespace Xml
} // namespace un
