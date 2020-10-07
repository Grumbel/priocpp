// Pingus - A free Lemmings clone
// Copyright (C) 1998-2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include <fstream>

#include <prio/reader.hpp>
#include <prio/reader_collection.hpp>
#include <prio/reader_mapping.hpp>

using namespace prio;

class ReaderTest : public ::testing::TestWithParam<std::string>
{
public:
  ReaderTest() :
    doc(),
    body()
  {}

  void SetUp() override
  {
    std::ifstream stream(GetParam());
    doc = ReaderDocument::from_stream(stream);
    body = doc.get_root().get_mapping();
  }

public:
  ReaderDocument doc;
  ReaderMapping body;
};

TEST_P(ReaderTest, parse)
{
  EXPECT_EQ("test-document", doc.get_root().get_name());
}

TEST_P(ReaderTest, read_bool)
{
  bool boolvalue;
  ASSERT_TRUE(body.read_bool("boolvalue", boolvalue));
  EXPECT_EQ(true, boolvalue);
}

TEST_P(ReaderTest, read_int)
{
  int intvalue;
  ASSERT_TRUE(body.read_int("intvalue", intvalue));
  EXPECT_EQ(5, intvalue);
}

TEST_P(ReaderTest, read_float)
{
  float floatvalue;
  ASSERT_TRUE(body.read_float("floatvalue", floatvalue));
  EXPECT_EQ(5.5f, floatvalue);
}

TEST_P(ReaderTest, read_string)
{
  std::string stringvalue;
  ASSERT_TRUE(body.read_string("stringvalue", stringvalue));
  EXPECT_EQ("Hello World", stringvalue);
}

namespace {

struct CustomType {
  int value = 0;
};

bool read_custom(ReaderMapping const& mapping, char const* key, CustomType& value)
{
  return mapping.read_int(key, value.value);
}

} // namespace

TEST_P(ReaderTest, read_custom)
{
  CustomType customvalue;
  ASSERT_TRUE(body.read("customvalue", customvalue));
  EXPECT_EQ(5, customvalue.value);
}

TEST_P(ReaderTest, read_mapping)
{
  ReaderMapping submap;
  ASSERT_TRUE(body.read_mapping("submap", submap));
  {
    int intvalue;
    ASSERT_TRUE(submap.read_int("int", intvalue));
    EXPECT_EQ(7, intvalue);
  }
  {
    float floatvalue;
    ASSERT_TRUE(submap.read_float("float", floatvalue));
    EXPECT_EQ(9.9f, floatvalue);
  }
}

TEST_P(ReaderTest, read_collection)
{
  ReaderCollection collection;
  ASSERT_TRUE(body.read_collection("collection", collection));
  EXPECT_EQ(3, collection.get_objects().size());
  auto objs = collection.get_objects();
  std::vector<std::string> result;
  for(auto const& e : objs)
  {
    result.push_back(e.get_name()); // NOLINT
  }
  EXPECT_EQ(std::vector<std::string>({"obj1", "obj2", "obj3"}), result);
}

TEST_P(ReaderTest, read_object)
{
  ReaderObject object;
  ASSERT_TRUE(body.read_object("object", object));
  EXPECT_EQ("realthing", object.get_name());
  ReaderMapping object_mapping = object.get_mapping();
  int prop1 = 0;
  int prop2 = 0;
  ASSERT_TRUE(object_mapping.read_int("prop1", prop1));
  ASSERT_TRUE(object_mapping.read_int("prop2", prop2));
  EXPECT_EQ(5, prop1);
  EXPECT_EQ(7, prop2);
}

INSTANTIATE_TEST_CASE_P(ParamReaderTest, ReaderTest,
                        ::testing::Values("test/sexpr.scm", "test/json.json"));

/* EOF */
