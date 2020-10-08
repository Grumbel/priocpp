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

#include "json_writer_impl.hpp"

using namespace prio;

TEST(JsonWriterImplTest, write)
{
  std::ostringstream os;

  JsonWriterImpl writer(os);
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
            "\t\"testfile\" : \n"
            "\t{\n"
            "\t\t\"falseval\" : false,\n"
            "\t\t\"floatval\" : 123.5,\n"
            "\t\t\"floatvals\" : \n"
            "\t\t[\n"
            "\t\t\t1.5,\n"
            "\t\t\t2.5,\n"
            "\t\t\t3.5\n"
            "\t\t],\n"
            "\t\t\"intval\" : 123,\n"
            "\t\t\"intvals\" : \n"
            "\t\t[\n"
            "\t\t\t1,\n"
            "\t\t\t2,\n"
            "\t\t\t3\n"
            "\t\t],\n"
            "\t\t\"stringval\" : \"Hello World\",\n"
            "\t\t\"stringvals\" : \n"
            "\t\t[\n"
            "\t\t\t\"\\\"Hello\",\n"
            "\t\t\t\"World\\\"\"\n"
            "\t\t],\n"
            "\t\t\"trueval\" : true,\n"
            "\t\t\"truevals\" : \n"
            "\t\t[\n"
            "\t\t\ttrue,\n"
            "\t\t\tfalse,\n"
            "\t\t\ttrue\n"
            "\t\t]\n"
            "\t}\n}");
}

/* EOF */
