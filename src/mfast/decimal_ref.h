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
#ifndef DECIMAL_REF_H_V02M5O0J
#define DECIMAL_REF_H_V02M5O0J

#include <cmath>
#include <cfloat>
#include "mfast_export.h"
#include "mfast/field_ref.h"
#include "mfast/int_ref.h"

#include <boost/multiprecision/cpp_dec_float.hpp>

#if defined(MAST_NO_SHARED_LIBS) && !defined(BOOST_NO_CXX11_EXTERN_TEMPLATE)
namespace boost { namespace multiprecision {
MFAST_EXTERN_TEMPLATE template class cpp_dec_float<18>;
}}
#endif

namespace mfast {

typedef boost::multiprecision::cpp_dec_float<18> decimal_backend;
typedef boost::multiprecision::number<decimal_backend> decimal;
class allocator;

namespace detail {
class codec_helper;
}


class exponent_cref
  : public field_cref
{
  public:
    typedef decimal_field_instruction instruction_type;
    typedef const decimal_field_instruction* instruction_cptr;
    typedef int8_t value_type;

    exponent_cref()
    {
    }

    exponent_cref(const value_storage*     storage,
                  const field_instruction* instruction)
      : field_cref(storage, instruction)
    {
    }

    exponent_cref(const exponent_cref& other)
      : field_cref(other)
    {
    }

    instruction_cptr instruction() const
    {
      return static_cast<instruction_cptr>(instruction_);
    }

    bool is_initial_value() const
    {
      return (this->absent() == instruction()->initial_value().is_empty()) &&
        ( this->absent() || this->value() == instruction()->initial_value().of_decimal.exponent_);
    }

    int8_t value() const
    {
      return this->storage()->of_decimal.exponent_;
    }

    value_storage default_base_value() const
    {
      value_storage v;
      v.defined(true);
      v.present(true);
      return v;
    }

  private:

    friend class mfast::detail::codec_helper;

    void save_to(value_storage& v) const
    {
      v.of_decimal.exponent_ = value();
      v.defined(true);
      v.present(this->present());
    }

};


class decimal_cref
  : public field_cref
{
  public:
    typedef decimal_field_instruction instruction_type;
    typedef const instruction_type* instruction_cptr;
    typedef decimal value_type;

    decimal_cref()
    {
    }

    decimal_cref(const value_storage*     storage,
                 const field_instruction* instruction)
      : field_cref(storage, instruction)
    {
    }

    decimal_cref(const decimal_cref& other)
      : field_cref(other)
    {
    }

    explicit decimal_cref(const field_cref& other)
      : field_cref(other)
    {
    }

    uint32_t id() const
    {
      return instruction_->id();
    }

    int64_t mantissa() const
    {
      return this->storage()->of_decimal.mantissa_;
    }

    int8_t exponent() const
    {
      return static_cast<int8_t>(this->storage()->of_decimal.exponent_);
    }

    bool is_initial_value() const;

    decimal value() const
    {
      decimal r( mantissa() );
      r *= decimal_backend(1.0, exponent());
      return r;
    }

    instruction_cptr instruction() const
    {
      return static_cast<instruction_cptr>(instruction_);
    }

    value_storage default_base_value() const
    {
      value_storage v;
      v.defined(true);
      v.present(true);
      return v;
    }

    bool has_individual_operators() const
    {
      return this->instruction()->field_type() == field_type_exponent;
    }

    exponent_cref for_exponent() const
    {
      return exponent_cref(this->storage(), this->instruction());
    }

    int64_cref for_mantissa() const
    {
      return int64_cref(this->storage(), this->instruction()->mantissa_instruction());
    }

  private:
    friend class mfast::detail::codec_helper;

    void save_to(value_storage& v) const
    {
      v.of_decimal.exponent_ = exponent();
      v.of_decimal.mantissa_ = mantissa();
      v.defined(true);
      v.present(this->present());
    }

};

inline bool operator == (const decimal_cref& lhs, const decimal_cref& rhs)
{
  return (lhs.absent() == rhs.absent()) && ( lhs.absent() ||  (lhs.mantissa() == rhs.mantissa() && lhs.exponent() == rhs.exponent()));
}

inline bool operator != (const decimal_cref& lhs, const decimal_cref& rhs)
{
  return !(lhs == rhs);
}

inline
bool decimal_cref::is_initial_value() const
{
  return *this == decimal_cref(&this->instruction()->initial_value(), this->instruction());
}

class exponent_mref
  : public exponent_cref
{
  public:
    exponent_mref()
    {
    }

    exponent_mref(allocator*               /* alloc */,
                  value_storage*           storage,
                  const field_instruction* instruction)
      : exponent_cref(storage, instruction)
    {
    }

    exponent_mref(const exponent_mref& other)
      : exponent_cref(other)
    {
    }

    void as_absent() const
    {
      this->storage()->present(false);
    }

    void as_initial_value() const
    {
      this->storage()->of_decimal.exponent_ = this->instruction()->initial_value().of_decimal.exponent_;
      this->storage()->present(this->instruction()->initial_value().of_decimal.present_);
    }

    void as(int8_t v) const
    {
      this->storage()->of_decimal.exponent_ = v;
      this->storage()->present(true);
    }

    void as (const exponent_cref& cref) const
    {
      if (cref.absent()) {
        as_absent();
      }
      else {
        as(cref.value());
      }
    }

  private:
    friend fast_istream& operator >> (fast_istream& strm, const exponent_mref& mref);
    int8_t& value_ref() const
    {
      this->storage()->present(true);
      return this->storage()->of_decimal.exponent_;
    }

    value_storage* storage() const
    {
      return const_cast<value_storage*>(field_cref::storage());
    }

    friend class mfast::detail::codec_helper;

    void copy_from(value_storage v) const
    {
      this->storage()->of_decimal.exponent_ = v.of_decimal.exponent_;
      this->storage()->present(!v.is_empty());
    }

};

template <>
struct mref_of<exponent_cref>
{
  typedef exponent_mref type;
};

class fast_istream;

class decimal_mref
  : public make_field_mref<decimal_cref>
{
  typedef make_field_mref<decimal_cref> base_type;

  public:
    typedef const decimal_field_instruction* instruction_cptr;

    decimal_mref()
    {
    }

    decimal_mref(allocator_type*  alloc,
                 value_storage*   storage,
                 instruction_cptr instruction)
      : base_type(alloc, storage, instruction)
    {
    }

    decimal_mref(const decimal_mref& other)
      : base_type(other)
    {
    }

    explicit decimal_mref(const field_mref_base& other)
      : base_type(other)
    {
    }

    void as (const decimal_cref& cref) const
    {
      if (cref.absent()) {
        as_absent();
      }
      else {
        as(cref.mantissa(), cref.exponent());
      }
    }

    int64_t mantissa() const
    {
      return this->storage()->of_decimal.mantissa_;
    }

    int8_t exponent() const
    {
      return static_cast<int8_t>(this->storage()->of_decimal.exponent_);
    }

    void as(int64_t mant, int8_t exp=0) const
    {
      assert (exp <= 64 && exp >= -64);
      this->storage()->of_decimal.mantissa_ = mant;
      this->storage()->of_decimal.exponent_ = exp;
      this->storage()->present(1);
    }

    void as (decimal d) const
    {
      double m;
      int32_t exp;
      d.backend().extract_parts(m, exp);
      d *= decimal(decimal_backend(1.0, 18-exp));
      this->storage()->of_decimal.mantissa_ = d.backend().extract_unsigned_long_long();
      this->storage()->of_decimal.exponent_ = exp-18;
      normalize();
      this->storage()->present(1);
    }

    template <unsigned Digits10, class ExponentType, class Allocator>
    void as(boost::multiprecision::number<boost::multiprecision::cpp_dec_float<Digits10,ExponentType,Allocator> > d) const
    {
      double m;
      int32_t exp;
      d.backend().extract_parts(m, exp);
      d *= decimal_backend(1.0, 18-exp);
      this->storage()->of_decimal.mantissa_ = d.backend().extract_unsigned_long_long();
      this->storage()->of_decimal.exponent_ = exp-18;
      normalize();
      this->storage()->present(1);
    }

    void set_mantissa(int64_t v) const
    {
      this->storage()->present(1);
      this->storage()->of_decimal.mantissa_ = v;
    }

    void set_exponent(int32_t v) const
    {
      this->storage()->present(1);
      this->storage()->of_decimal.exponent_ = v;
    }

    void as_initial_value() const
    {
      *this->storage() = instruction()->initial_value();
    }

    exponent_mref for_exponent() const
    {
      return exponent_mref(0, this->storage(), this->instruction());
    }

    int64_mref for_mantissa() const
    {
      return int64_mref(0, this->storage(), this->instruction()->mantissa_instruction());
    }

    void normalize() const
    {
      while (mantissa() != 0 && mantissa() % 10 == 0) {
        this->set_mantissa(mantissa()/10);
        this->set_exponent(exponent()+1);
      }

      if (mantissa() == 0)
        this->set_exponent(0);
    }

  private:
    friend fast_istream& operator >> (fast_istream& strm, const decimal_mref& mref);
    friend class mfast::detail::codec_helper;

    void copy_from(value_storage v) const
    {
      *this->storage() = v;
    }

};

template <>
struct mref_of<decimal_cref>
{
  typedef decimal_mref type;
};

}

#endif /* end of include guard: DECIMAL_REF_H_V02M5O0J */
