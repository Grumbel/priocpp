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

#ifndef HEADER_PRIO_FILE_WRITER_IMPL_HPP
#define HEADER_PRIO_FILE_WRITER_IMPL_HPP

#include <string>
#include <vector>

namespace prio {

/** Interface to write out name/value pairs out of some kind of file or
    structure */
class WriterImpl
{
public:
  virtual ~WriterImpl() {}

  virtual void begin_collection(const char* name) = 0;
  virtual void end_collection() = 0;

  virtual void begin_object(const char* type) = 0;
  virtual void end_object() = 0;

  virtual void begin_mapping(const char* name) = 0;
  virtual void end_mapping() = 0;

  virtual void write(const char* name, bool) = 0;
  virtual void write(const char* name, int) = 0;
  virtual void write(const char* name, float) = 0;
  virtual void write(const char* name, const std::string&) = 0;

  virtual void write(const char* name, std::vector<bool> const&) = 0;
  virtual void write(const char* name, std::vector<int> const&) = 0;
  virtual void write(const char* name, std::vector<float> const&) = 0;
  virtual void write(const char* name, std::vector<std::string> const&) = 0;
};

} // namespace prio

#endif

/* EOF */
