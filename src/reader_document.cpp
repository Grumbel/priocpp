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

#include "reader_document.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include <json/reader.h>
#include <logmich/log.hpp>
#include <sexp/parser.hpp>

#include "json_reader_impl.hpp"
#include "reader_collection.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"
#include "sexpr_reader_impl.hpp"

namespace prio {

ReaderDocument
ReaderDocument::from_string(std::string_view text)
{
  std::istringstream in{std::string(text)};
  return ReaderDocument::from_stream(in);
}

ReaderDocument
ReaderDocument::from_stream(std::istream& stream, std::optional<std::string> const& filename)
{
  int c = stream.get();
  stream.unget();
  if (c == '{')
  { // json
    Json::CharReaderBuilder builder;
    std::string errs;
    Json::Value root;
    if (Json::parseFromStream(builder, stream, &root, &errs)) {
      return ReaderDocument(ReaderObject(std::make_shared<JsonReaderObjectImpl>(root)), filename);
    } else {
      throw std::runtime_error(fmt::format("json parse error: {}", errs));
    }
  }
  else
  { // sexp
    auto sx = sexp::Parser::from_stream(stream /*, sexp::Parser::USE_ARRAYS */); // FIXME
    return ReaderDocument(ReaderObject(std::make_shared<SExprReaderObjectImpl>(std::move(sx))), filename);
  }
}

ReaderDocument
ReaderDocument::from_file(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin) {
    throw std::runtime_error(fmt::format("{}: failed to open: {}", filename, strerror(errno)));
  } else {
    return from_stream(fin, filename);
  }
}

#if 0
std::vector<ReaderObject>
Reader::parse_many(const std::string& pathname)
{
  std::shared_ptr<lisp::Lisp> sexpr = lisp::Parser::parse(pathname.get_sys_path());
  if (sexpr)
  {
    std::vector<Reader> sections;
    for(size_t i = 0; i < sexpr->get_list_size(); ++i)
    {
      sections.push_back(Reader(std::make_shared<SExprReaderImpl>(sexpr->get_list_elem(i))));
    }

    return sections;
  }
  else
  {
    return std::vector<Reader>();
  }
}
#endif

ReaderDocument:: ReaderDocument() :
  m_root(),
  m_filename()
{
}

ReaderDocument::ReaderDocument(ReaderObject root, std::optional<std::string> const& filename) :
  m_root(root),
  m_filename(filename)
{
}

ReaderObject
ReaderDocument::get_root() const
{
  return m_root;
}

std::string
ReaderDocument::get_filename() const
{
  if (!m_filename) {
    return "<unknown>";
  } else {
    return *m_filename;
  }
}

std::string
ReaderDocument::get_directory() const
{
  if (!m_filename) {
    return std::filesystem::path("/");
  } else {
    return std::filesystem::path(*m_filename).parent_path();
  }
}

} // namespace prio

/* EOF */
