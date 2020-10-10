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
#include "reader_error.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"
#include "sexpr_reader_impl.hpp"

namespace prio {

ReaderDocument
ReaderDocument::from_string(std::string_view text, bool pedantic, std::optional<std::string> const& filename)
{
  std::istringstream in{std::string(text)};
  return ReaderDocument::from_stream(in, pedantic, filename);
}

ReaderDocument
ReaderDocument::from_stream(std::istream& stream, bool pedantic, std::optional<std::string> const& filename)
{
  int c = stream.get();
  stream.unget();
  if (c == '{')
  { // json
    Json::CharReaderBuilder builder;
    std::string errs;
    Json::Value root;
    if (Json::parseFromStream(builder, stream, &root, &errs)) {
      return ReaderDocument(std::make_unique<JsonReaderDocumentImpl>(std::move(root), pedantic, filename));
    } else {
      throw ReaderError(fmt::format("json parse error: {}", errs));
    }
  }
  else
  { // sexp
    try {
      auto sx = sexp::Parser::from_stream(stream, sexp::Parser::USE_ARRAYS);
      return ReaderDocument(std::make_unique<SExprReaderDocumentImpl>(std::move(sx), pedantic, filename));
    } catch(std::exception const& err) {
      std::throw_with_nested(ReaderError(fmt::format("{}: ReaderDocument::from_stream() failed", filename ?  *filename : "<unknown>")));
    }
  }
}

ReaderDocument
ReaderDocument::from_file(const std::string& filename, bool pedantic)
{
  std::ifstream fin(filename);
  if (!fin) {
    throw ReaderError(fmt::format("{}: failed to open: {}", filename, strerror(errno)));
  } else {
    return from_stream(fin, pedantic, filename);
  }
}

std::vector<ReaderDocument>
ReaderDocument::parse_many(const std::string& pathname)
{
#if 0
  std::shared_ptr<lisp::Lisp> sexpr = lisp::Parser::parse(pathname.get_sys_path());
  if (sexpr)
  {
    std::vector<Reader> sections;
    for(size_t i = 0; i < sexpr->get_list_size(); ++i)
    {
      sections.push_back(Reader(std::make_unique<SExprReaderImpl>(sexpr->get_list_elem(i))));
    }

    return sections;
  }
  else
#endif
  {
    return std::vector<ReaderDocument>();
  }
}

ReaderDocument:: ReaderDocument() :
  m_impl()
{
}

ReaderDocument::ReaderDocument(std::unique_ptr<ReaderDocumentImpl> impl) :
  m_impl(std::move(impl))
{
}

ReaderDocument::ReaderDocument(ReaderDocument&&) noexcept = default;

ReaderDocument::~ReaderDocument()
{
}

ReaderDocument&
ReaderDocument::operator=(ReaderDocument&&) noexcept = default;

ReaderObject
ReaderDocument::get_root() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_root();
}

std::string
ReaderDocument::get_name() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_root().get_name();
}

ReaderMapping
ReaderDocument::get_mapping() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_root().get_mapping();
}

std::string
ReaderDocument::get_filename() const
{
  if (!m_impl) { return {}; }

  if (!m_impl->get_filename()) {
    return "<unknown>";
  } else {
    return *m_impl->get_filename();
  }
}

std::string
ReaderDocument::get_directory() const
{
  if (!m_impl) { return {}; }

  if (!m_impl->get_filename()) {
    return std::filesystem::path("/");
  } else {
    return std::filesystem::path(*m_impl->get_filename()).parent_path();
  }
}

} // namespace prio

/* EOF */
