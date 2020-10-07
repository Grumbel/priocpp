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

#ifndef HEADER_PRIO_READER_HPP
#define HEADER_PRIO_READER_HPP

#include <memory>
#include <vector>
#include <string>

#include "reader_object.hpp"

namespace prio {

class ReaderObject;
class ReaderMapping;
class ReaderCollection;

class ReaderObjectImpl;
class ReaderMappingImpl;
class ReaderCollectionImpl;

class Reader final // NOLINT
{
public:
  static ReaderObject parse(std::istream& stream);
  static ReaderObject parse(const std::string& filename);
  static ReaderObject parse_string(std::string const& text);

  /** Reads multiple trees from a file, for use with files that don't
      contain a root element */
  static std::vector<ReaderObject> parse_many(const std::string& pathname);

private:
  Reader() = delete;
};

} // namespace prio

#endif

/* EOF */
