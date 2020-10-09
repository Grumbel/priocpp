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

#include "sexpr_writer_impl.hpp"
#include "writer_impl_test.hpp"

using namespace prio;

TEST(SExprWriterImplTest, write)
{
  std::ostringstream os;
  SExprWriterImpl writer(os);
  write_testfile(writer);

  ASSERT_EQ(os.str(),
            "(testfile\n"
            "  (trueval #t)\n"
            "  (falseval #f)\n"
            "  (intval 123)\n"
            "  (floatval 123.5)\n"
            "  (stringval \"Hello World\")\n"
            "  (escapedstringval \"\\\"Hello\\\\World\\\"\")\n"
            "  (truevals #t #f #t)\n"
            "  (intvals 1 2 3)\n"
            "  (floatvals 1.5 2.5 3.5)\n"
            "  (stringvals \"\\\"Hello\" \"World\\\"\")\n"
            "  (background\n"
            "    (color\n"
            "      (red 0)\n"
            "      (green 0)\n"
            "      (blue 0))))\n"
            "\n"
            ";; EOF ;;\n");
}

/* EOF */
