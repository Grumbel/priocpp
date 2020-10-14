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

#ifndef HEADER_PRIO_READER_IMPL_HPP
#define HEADER_PRIO_READER_IMPL_HPP

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace prio {

class ReaderCollection;
class ReaderMapping;
class ReaderObject;

class ReaderDocumentImpl
{
public:
  virtual ~ReaderDocumentImpl() {}

  virtual ReaderObject get_root() const = 0;
  virtual std::optional<std::string> get_filename() const = 0;
};

class ReaderObjectImpl
{
public:
  virtual ~ReaderObjectImpl() {}

  virtual std::string get_name() const = 0;
  virtual ReaderMapping get_mapping() const = 0;
};

class ReaderCollectionImpl
{
public:
  virtual ~ReaderCollectionImpl() {}

  virtual std::vector<ReaderObject> get_objects() const = 0;
};

class ReaderMappingImpl
{
public:
  virtual ~ReaderMappingImpl() {}

  virtual std::vector<std::string> get_keys() const = 0;

  virtual bool read(std::string_view key, bool&) const = 0;
  virtual bool read(std::string_view key, int&) const = 0;
  virtual bool read(std::string_view key, float&) const = 0;
  virtual bool read(std::string_view key, std::string&) const = 0;

  virtual bool read(std::string_view key, std::vector<bool>& v) const = 0;
  virtual bool read(std::string_view key, std::vector<int>& v) const = 0;
  virtual bool read(std::string_view key, std::vector<float>& v) const = 0;
  virtual bool read(std::string_view key, std::vector<std::string>& v) const = 0;

  virtual bool read(std::string_view key, ReaderMapping&) const = 0;
  virtual bool read(std::string_view key, ReaderCollection&) const = 0;
  virtual bool read(std::string_view key, ReaderObject&) const = 0;

  virtual void error(std::string_view key, std::string_view message) const = 0;
  virtual void missing_key_error(std::string_view key) const = 0;
};

} // namespace prio

#endif

/* EOF */
