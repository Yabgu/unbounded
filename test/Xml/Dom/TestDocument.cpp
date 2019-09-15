#include <gtest/gtest.h>
#include <Xml/Dom/Document.h>

using namespace un::Xml::Dom;
using namespace std;

namespace
{

// Test simple XML creation and string casting
TEST(XmlDomParser, DocumentToString)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content</test>");

  document.root_node.push_back(Node("pushBack"));
  string asString2 = (string)document;
  EXPECT_EQ(asString2, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content<pushBack/></test>");
}

TEST(XmlDomParser, push_back)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  document.root_node.push_back(Node("pushBack"));
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content<pushBack/></test>");
}

TEST(XmlDomParser, push_front)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  document.root_node.push_front(Node("pushFront", "dummy"));
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test><pushFront>dummy</pushFront>content</test>");
}

} // namespace

int main(int ac, char *av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
