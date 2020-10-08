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

#include <iostream>
#include <fmt/format.h>

#include <prio/prio.hpp>

using namespace prio;

namespace {

void write(Writer& writer, ReaderMapping const& body);

void write(Writer& writer, ReaderObject const& root)
{
  write(writer, root.get_mapping());
}

void write(Writer& writer, ReaderCollection const& collection)
{
  for (auto const& obj : collection.get_objects()) {
    write(writer, obj);
  }
}

void write(Writer& writer, ReaderMapping const& body)
{
  for(auto const& key : body.get_keys()) {
    bool bool_value;
    int int_value;
    float float_value;
    std::string string_value;

    std::vector<bool> bool_values;
    std::vector<int> int_values;
    std::vector<float> float_values;
    std::vector<std::string> string_values;

    ReaderMapping mapping;
    ReaderObject object;
    ReaderCollection collection;

    if (body.read(key.c_str(), bool_value)) {
      writer.write(key.c_str(), bool_value);
    } else  if (body.read(key.c_str(), int_value)) {
      writer.write(key.c_str(), int_value);
    } else if (body.read(key.c_str(), float_value)) {
      writer.write(key.c_str(), float_value);
    } else if (body.read(key.c_str(), string_value)) {
      writer.write(key.c_str(), string_value);
    }

    else if (body.read(key.c_str(), bool_values)) {
      writer.write(key.c_str(), bool_values);
    } else  if (body.read(key.c_str(), int_values)) {
      writer.write(key.c_str(), int_values);
    } else if (body.read(key.c_str(), float_values)) {
      writer.write(key.c_str(), float_values);
    } else if (body.read(key.c_str(), string_values)) {
      writer.write(key.c_str(), string_values);
    }

    else if (body.read(key.c_str(), mapping)) {
      writer.begin_mapping(key.c_str());
      write(writer, mapping);
      writer.end_mapping();
    } else if (body.read(key.c_str(), object)) {
      writer.begin_object(key.c_str());
      write(writer, object);
      writer.end_object();
    } else if (body.read(key.c_str(), collection)) {
      writer.begin_collection(key.c_str());
      write(writer, collection);
      writer.end_collection();
    }
  }
}

enum class OutputFormat { JSON, FASTJSON, SEXP };

struct Options
{
  OutputFormat format = OutputFormat::JSON;
  std::vector<std::string> files = {};
};

void print_usage(char const* arg0)
{
  std::cout << "Usage: " << arg0 << "[OPTION]... [FILE]...\n"
            << "Little toy format converter\n"
            << "\n"
            << "  --help        Display this help text\n"
            << "  --json        Output pretty json\n"
            << "  --fastjson    Output fastjson\n"
            << "  --sexp        Output s-expressions\n";
}

Options parse_args(int argc, char** argv)
{
  Options opts;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "--help") == 0) {
        print_usage(argv[0]);
      } else if (strcmp(argv[i], "--json") == 0) {
        opts.format = OutputFormat::JSON;
      } else if (strcmp(argv[i], "--fastjson") == 0) {
        opts.format = OutputFormat::FASTJSON;
      } else if (strcmp(argv[i], "--sexp") == 0) {
        opts.format = OutputFormat::SEXP;
      } else {
        throw std::runtime_error(fmt::format("invalid argument {}", argv[i]));
      }
    } else {
      opts.files.emplace_back(argv[i]);
    }
  }

  return opts;
}

Writer make_writer(OutputFormat format)
{
  switch (format) {
    case OutputFormat::JSON:
      return Writer::json(std::cout);

    case OutputFormat::FASTJSON:
      return Writer::fastjson(std::cout);

    case OutputFormat::SEXP:
      return Writer::sexpr(std::cout);

    default:
      throw std::runtime_error("invalid output format");
  }
}

} // namespace

int main(int argc, char** argv)
{
  try {
    Options opts = parse_args(argc, argv);

    for (auto const& filename : opts.files) {
      try {
        ReaderDocument doc = ReaderDocument::from_file(filename, false);
        ReaderObject const& root = doc.get_root();

        Writer writer = make_writer(opts.format);
        writer.begin_object(root.get_name().c_str());
        write(writer, root);
        writer.end_object();
      } catch (std::exception& err) {
        std::cerr << filename << ": " << err.what() << std::endl;
      }
    }
  } catch (std::exception const& err) {
    std::cerr << err.what() << std::endl;
  }
  return 0;
}

/* EOF */
