// Copyright (c) 2013, Huang-Ming Huang,  Object Computing, Inc.
// All rights reserved.
//
// This file is part of mFAST.
//
//     mFAST is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Lesser General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     mFAST is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU Lesser General Public License
//     along with mFast.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef INL_GEN_H_G360YUN
#define INL_GEN_H_G360YUN

#include "codegen_base.h"

class inl_gen
  : public codegen_base
{
public:
  inl_gen(const char* filebase)
    : codegen_base(filebase, ".inl")
  {
  }

  void generate(mfast::dynamic_templates_description& desc);

  virtual void visit(const mfast::int32_field_instruction*, void*);
  virtual void visit(const mfast::uint32_field_instruction*, void*);
  virtual void visit(const mfast::int64_field_instruction*, void*);
  virtual void visit(const mfast::uint64_field_instruction*, void*);
  virtual void visit(const mfast::decimal_field_instruction*, void*);
  virtual void visit(const mfast::ascii_field_instruction*, void*);
  virtual void visit(const mfast::unicode_field_instruction*, void*);
  virtual void visit(const mfast::byte_vector_field_instruction*, void*);
  virtual void visit(const mfast::int32_vector_field_instruction*, void*);
  virtual void visit(const mfast::uint32_vector_field_instruction*, void*);
  virtual void visit(const mfast::int64_vector_field_instruction*, void*);
  virtual void visit(const mfast::uint64_vector_field_instruction*, void*);
  virtual void visit(const mfast::group_field_instruction*, void*);
  virtual void visit(const mfast::sequence_field_instruction*, void*);
  virtual void visit(const mfast::template_instruction*, void*);
  virtual void visit(const mfast::templateref_instruction*, void*);

private:
  virtual void traverse(const mfast::group_field_instruction* inst, const char* name_suffix);

  void gen_primitive (const char* cpp_type, const mfast::field_instruction* inst);

  std::stringstream mref_scope_;
};



#endif /* end of include guard: INL_GEN_H_G360YUN */
