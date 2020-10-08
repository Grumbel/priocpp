// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include "jsonpretty_writer_impl.hpp"

using namespace prio;

TEST(JsonPrettyWriterImplTest, write)
{
  std::ostringstream os;

  JsonPrettyWriterImpl writer(os);
  writer.begin_object("testfile");
  writer.write_bool("trueval", true);
  writer.write_bool("falseval", false);
  writer.write_int("intval", 123);
  writer.write_float("floatval", 123.5);
  writer.write_string("stringval", "Hello World");

  writer.write_bools("truevals", std::vector<bool>({true, false, true}));
  writer.write_ints("intvals", std::vector<int>({1, 2, 3}));
  writer.write_floats("floatvals", std::vector<float>({1.5, 2.5, 3.5}));
  writer.write_strings("stringvals", std::vector<std::string>({"\"Hello", "World\""}));
  writer.end_object();

  ASSERT_EQ(os.str(),
            "{\n"
            "  \"testfile\": {\n"
            "    \"trueval\": true,\n"
            "    \"falseval\": false,\n"
            "    \"intval\": 123,\n"
            "    \"floatval\": 123.5,\n"
            "    \"stringval\": \"Hello World\",\n"
            "    \"truevals\": [true, false, true],\n"
            "    \"intvals\": [1, 2, 3],\n"
            "    \"floatvals\": [1.5, 2.5, 3.5],\n"
            "    \"stringvals\": [\"\"Hello\", \"World\"\"]\n"
            "  }\n"
            "}\n");
}

/* EOF */
