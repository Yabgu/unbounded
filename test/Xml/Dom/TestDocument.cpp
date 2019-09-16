#include <gtest/gtest.h>
#include <Xml/Dom/Document.h>

using namespace un::Xml::Dom;
using namespace std;

namespace
{

// Test simple XML creation and string casting
TEST(Document, DocumentToString)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content</test>");

  document.root_node.push_back(Node("pushBack"));
  string asString2 = (string)document;
  EXPECT_EQ(asString2, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content<pushBack/></test>");
}

TEST(Document, DocumentFromString)
{
  Document document;
  document.parse("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content</test>");
  string asString = (string)document;
  EXPECT_EQ(document.root_node.name, "test");
  EXPECT_EQ(document.root_node.content, "content");
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content</test>");
}

TEST(Node, push_back)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  document.root_node.push_back(Node("pushBack"));
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content<pushBack/></test>");
}

TEST(Node, push_front)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  document.root_node.push_front(Node("pushFront", "dummy"));
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test><pushFront>dummy</pushFront>content</test>");
}

TEST(NodeAttributes, push_back)
{
  Document document("1.0");
  Node root_node("root");
  root_node.attributes.push_back("test", "testvalue");
  document.root_node = root_node;
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test=\"testvalue\"/>");
}

} // namespace

int main(int ac, char *av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
