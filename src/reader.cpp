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

#include "reader.hpp"

#include <sstream>
#include <fstream>
#include <json/reader.h>
#include <utility>

#include <logmich/log.hpp>
#include <sexp/parser.hpp>

#include "json_reader_impl.hpp"
#include "reader_impl.hpp"
#include "sexpr_reader_impl.hpp"
#include "reader_collection.hpp"
#include "reader_mapping.hpp"

namespace prio {

ReaderObject
Reader::parse_string(std::string const& text)
{
  std::istringstream in(text);
  return Reader::parse(in);
}

ReaderObject
Reader::parse(std::istream& stream)
{
  int c = stream.get();
  stream.unget();
  if (c == '{')
  {
    Json::CharReaderBuilder builder;
    std::string errs;
    Json::Value root;
    if (Json::parseFromStream(builder, stream, &root, &errs))
    {
      return ReaderObject(std::make_shared<JsonReaderObjectImpl>(root));
    }
    else
    {
      log_error("json parse error: {}", errs);
      return ReaderObject();
    }
  }
  else
  {
    try
    {
      auto sx = sexp::Parser::from_stream(stream);
      return ReaderObject(std::make_shared<SExprReaderObjectImpl>(std::move(sx)));
    }
    catch(std::exception const& err)
    {
      log_error("sexp parse error: {}", err.what());
      return ReaderObject();
    }
  }
}

ReaderObject
Reader::parse(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin)
  {
    return ReaderObject();
  }
  else
  {
    return parse(fin);
  }
}

std::vector<ReaderObject>
Reader::parse_many(const std::string& pathname)
{
  return {};
#if 0
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
#endif
}

} // namespace prio

/* EOF */
