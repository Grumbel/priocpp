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

#include <prio/reader_document.hpp>
#include <prio/reader_error.hpp>

using namespace prio;

class ReaderDocumentTest : public ::testing::TestWithParam<std::string> {};

TEST_P(ReaderDocumentTest, from_file)
{
  EXPECT_NO_THROW(ReaderDocument::from_file("test/data/data" + GetParam(), true));
  EXPECT_NO_THROW(ReaderDocument::from_file("test/data/data" + GetParam(), false));
}

TEST_P(ReaderDocumentTest, from_file_fail)
{
  EXPECT_THROW(ReaderDocument::from_file("does-not-exist", true), ReaderError);
  EXPECT_THROW(ReaderDocument::from_file("does-not-exist", false), ReaderError);

  EXPECT_THROW(ReaderDocument::from_file("test/data/data-corrupt" + GetParam(), true), ReaderError);
  EXPECT_THROW(ReaderDocument::from_file("test/data/data-corrupt" + GetParam(), false), ReaderError);
}

TEST_P(ReaderDocumentTest, get_filename)
{
  ReaderDocument doc = ReaderDocument::from_file("test/data/data" + GetParam(), true);
  EXPECT_EQ(doc.get_filename(), "test/data/data" + GetParam());
}

TEST_P(ReaderDocumentTest, get_directory)
{
  ReaderDocument doc = ReaderDocument::from_file("test/data/data" + GetParam(), true);
  EXPECT_EQ(doc.get_directory(), "test/data");
}

TEST_P(ReaderDocumentTest, get_root)
{
  ReaderDocument doc = ReaderDocument::from_file("test/data/data" + GetParam(), true);
  EXPECT_EQ(doc.get_root().get_name(), "test-document");
}

INSTANTIATE_TEST_CASE_P(ParamReaderDocumentTest, ReaderDocumentTest,
                        ::testing::Values(".sexp", ".json"));

/* EOF */
