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

#include "reader_mapping.hpp"

#include "reader_collection.hpp"
#include "reader_object.hpp"
#include "reader_impl.hpp"

namespace prio {

ReaderMapping::ReaderMapping(std::shared_ptr<ReaderMappingImpl> impl_) :
  m_impl(std::move(impl_))
{
}

ReaderMapping::ReaderMapping() :
  m_impl()
{
}

bool
ReaderMapping::read(const char* key, int& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read (const char* key, float& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read  (const char* key, bool& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read(const char* key, std::string& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read(const char* key, ReaderObject& object) const
{
  if (m_impl)
    return m_impl->read(key, object);
  else
    return false;
}

bool
ReaderMapping::read(const char* key, ReaderMapping& mapping) const
{
  if (m_impl)
    return m_impl->read(key, mapping);
  else
    return false;
}

bool
ReaderMapping::read(const char* key, ReaderCollection& collection) const
{
  if (m_impl)
    return m_impl->read(key, collection);
  else
    return false;
}

ReaderMapping
ReaderMapping::read_mapping(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderMapping result;
    read(key, result);
    return result;
  }
}

ReaderCollection
ReaderMapping::read_collection(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderCollection result;
    read(key, result);
    return result;
  }
}

ReaderObject
ReaderMapping::read_object(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderObject result;
    read(key, result);
    return result;
  }
}

std::vector<std::string>
ReaderMapping::get_keys() const
{
  if (m_impl)
    return m_impl->get_keys();
  else
    return {};
}

} // namespace prio

/* EOF */
