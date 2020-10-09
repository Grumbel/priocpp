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

#ifndef HEADER_PRIO_TEST_WRITER_TEST_IMPL_HPP
#define HEADER_PRIO_TEST_WRITER_TEST_IMPL_HPP

#include <prio/writer.hpp>

inline
void write_testfile(prio::WriterImpl& writer)
{
  writer.begin_object("testfile");
  writer.write("trueval", true);
  writer.write("falseval", false);
  writer.write("intval", 123);
  writer.write("floatval", 123.5f);
  writer.write("stringval", "Hello World");
  writer.write("escapedstringval", "\"Hello\\World\"");

  writer.write("truevals", std::vector<bool>({true, false, true}));
  writer.write("intvals", std::vector<int>({1, 2, 3}));
  writer.write("floatvals", std::vector<float>({1.5f, 2.5f, 3.5f}));
  writer.write("stringvals", std::vector<std::string>({"\"Hello", "World\""}));

  writer.begin_keyvalue("background");
  writer.begin_object("color");
  writer.write("red", 0.0f);
  writer.write("green", 0.0f);
  writer.write("blue", 0.0f);
  writer.end_object();
  writer.end_keyvalue();

  writer.end_object();
}

#endif

/* EOF */
