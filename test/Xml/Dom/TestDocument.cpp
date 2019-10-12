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
  EXPECT_EQ(document.root_node.count, 0);
  document.root_node.push_back(Node("pushBack"));
  EXPECT_EQ(document.root_node.count, 1);
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content<pushBack/></test>");
}

TEST(Node, remove)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  EXPECT_EQ(document.root_node.count, 0);

  Node test_node("pushBack");

  document.root_node.push_back(test_node);
  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content<pushBack/></test>");
  EXPECT_EQ(document.root_node.count, 1);

  document.root_node.remove(test_node);
  EXPECT_EQ(document.root_node.count, 0);

  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test>content</test>");
}

TEST(Node, push_front)
{
  Document document("1.0");
  document.root_node = Node("test", "content");
  EXPECT_EQ(document.root_node.count, 0);
  document.root_node.push_front(Node("pushFront", "dummy"));
  EXPECT_EQ(document.root_node.count, 1);
  string asString = (string)document;
  EXPECT_EQ(asString, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test><pushFront>dummy</pushFront>content</test>");
}

TEST(NodeAttributes, push_back)
{
  Document document("1.0");
  Node root_node("root");

  EXPECT_EQ(root_node.attributes.count, 0);
  EXPECT_EQ(root_node.attributes.is_empty, true);

  root_node.attributes.push_back("test", "testvalue");

  EXPECT_EQ(root_node.attributes.count, 1);
  EXPECT_EQ(root_node.attributes.is_empty, false);

  document.root_node = root_node;

  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test=\"testvalue\"/>");

  root_node.attributes.push_back("test1", "randomvalue");
  EXPECT_EQ(root_node.attributes.count, 2);

  root_node.attributes.push_back("test2", "value");
  EXPECT_EQ(root_node.attributes.count, 3);

  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test=\"testvalue\" test1=\"randomvalue\" test2=\"value\"/>");
}

TEST(NodeAttributes, remove)
{
  Document document("1.0");
  Node root_node("root");

  EXPECT_EQ(root_node.attributes.count, 0);
  EXPECT_EQ(root_node.attributes.is_empty, true);

  root_node.attributes.push_back("test", "testvalue");

  EXPECT_EQ(root_node.attributes.count, 1);
  EXPECT_EQ(root_node.attributes.is_empty, false);

  document.root_node = root_node;

  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test=\"testvalue\"/>");

  root_node.attributes.push_back("test1", "randomvalue");
  EXPECT_EQ(root_node.attributes.count, 2);

  root_node.attributes.remove("test");
  EXPECT_EQ(root_node.attributes.count, 1);
  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test1=\"randomvalue\"/>");
}

TEST(NodeAttributes, reference_update)
{
  Document document("1.0");
  Node root_node("root");
  root_node.attributes.push_back("test", "testvalue");
  document.root_node = root_node;
  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test=\"testvalue\"/>");

  root_node.attributes["test"].value = "hello world";

  EXPECT_EQ((string)document, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root test=\"hello world\"/>");
}

} // namespace

int main(int ac, char *av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}
