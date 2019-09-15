// Copyright 2016 Abdurrahim Cakar
/**
 * @file DocumentHandlerLibxml2.h
 * @date Oct 10, 2012
 * @author Abdurrahim Cakar <abdurrahimcakar@gmail.com>
 * @brief Xml document handler class using libxml2
 */

#pragma once

#include <Xml/Dom/Document.h>
#include "NodeHandlerLibxml2.h"
#include <cctype>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlsave.h>

namespace un
{
namespace Xml
{
namespace Dom
{

class Document::_Handler
{
private:
  xmlDocPtr _doc;

public:
  _Handler(const char *version) : _doc(NULL)
  {
    _doc = xmlNewDoc(BAD_CAST version);
    if (_doc == NULL)
    {
      throw std::runtime_error("xmlNewDoc failed");
    }
  }

  ~_Handler()
  {
    this->safe_free();
  }

  inline void safe_free()
  {
    if (this->_doc != NULL)
    {
      xmlFreeDoc(this->_doc);
    }
    this->_doc = NULL;
  }

  inline void reset(xmlDocPtr doc)
  {
    safe_free();
    this->_doc = doc;
  }

  inline void set_root_node(Node &rnode, const Node &node)
  {
    if (!node._handler->is_owner)
    {
      throw std::runtime_error(
          "Node is already owned by another node or document.");
    }

    node._handler->is_owner = false;

    xmlNodePtr old = xmlDocSetRootElement(this->_doc, node._handler->_handler);

    if (rnode._handler != nullptr)
    {
      rnode._handler->is_owner = true;
    }
    else if (old != NULL)
    {
      xmlFreeNode(old);
    }

    rnode._handler = node._handler;
  }

  inline void write_to(std::ostream &_cout)
  {
    int buffersize2;
    xmlChar *xmlbuff;

    xmlDocDumpFormatMemory(_doc, &xmlbuff, &buffersize2, 1);

    _cout.write((const char *)xmlbuff, buffersize2);

    xmlFree(xmlbuff);
  }

  inline void write_to_c(FILE *fp) { xmlDocFormatDump(fp, _doc, 1); }

  inline std::string as_string(bool pretty_print = false,
                               bool skip_headers = false)
  {
    xmlBufferPtr buff = xmlBufferCreate();
    if (buff == NULL)
    {
      throw std::runtime_error(xmlGetLastError()->message);
    }

    std::shared_ptr<xmlBuffer> buffguard(buff, xmlBufferFree);

    xmlSaveCtxtPtr saveCtxt = xmlSaveToBuffer(
        buff, "UTF-8", (pretty_print ? (XML_SAVE_FORMAT) : (0)) | (skip_headers ? (XML_SAVE_NO_DECL) : (0)));

    if (saveCtxt == NULL)
    {
      xmlErrorPtr err = xmlGetLastError();
      throw std::runtime_error(err->message);
    }

    int n = xmlSaveDoc(saveCtxt, _doc);
    xmlSaveClose(saveCtxt);
    if (n < 0)
    {
      throw std::runtime_error(xmlGetLastError()->message);
    }

    int trim_end = buff->use;
    const char *const content = (char *)buff->content;

    if (!pretty_print) // Trim trailing whitespace
    {
      for (; trim_end > 0 && std::isspace(content[trim_end - 1]); --trim_end)
      {
      }
    }

    return std::string(content, trim_end);
  }

  inline void parse(const char *data, std::size_t size)
  {
    xmlDocPtr doc;

    doc = xmlParseMemory(data, size);

    if (doc == NULL)
    {
      xmlErrorPtr err = xmlGetLastError();
      throw std::runtime_error(err->message);
    }

    reset(doc);
  }

  inline void parse_file(const char *path)
  {
    xmlDocPtr doc;

    doc = xmlParseFile(path);
    if (doc == NULL)
    {
      xmlErrorPtr err = xmlGetLastError();
      throw std::runtime_error(err->message);
    }
    reset(doc);
  }

  inline Node &get_root_node(Node &rnode)
  {
    xmlNodePtr root_node = xmlDocGetRootElement(_doc);
    if (root_node == NULL)
    {
      throw std::runtime_error("Document does not have root node");
    }

    rnode._handler = std::shared_ptr<::un::Xml::Dom::Node::_Handler>(
        new ::un::Xml::Dom::Node::_Handler(root_node, false));

    return rnode;
  }
};

} // namespace Dom
} // namespace Xml
} // namespace un
