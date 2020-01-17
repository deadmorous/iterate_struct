#pragma once

#include "iterate_struct.h"
#include <ostream>

namespace itearate_struct {

template<class stream>
class value_printer
{
public:
    value_printer(stream& s) : m_s(s) {}

    template<class T>
    void operator()(T& value, const char *name) const
    {
        m_s << std::string(m_depth<<2, ' ');
        m_s << name << " = ";
        print_priv(value);
        m_s << std::endl;
    }

    template <class T, std::enable_if_t<itearate_struct::has_output_operator_v<stream, T>, int> = 0>
    void print(const T& x) const {
        m_s << x;
    }

    template <class T, std::enable_if_t<itearate_struct::has_iterate_struct_helper_v<T>, int> = 0>
    void print(const T& x) const {
        itearate_struct::for_each(x, *this);
    }

private:
    template <class T, std::enable_if_t<itearate_struct::has_output_operator_v<stream, T>, int> = 0>
    void print_priv(const T& x) const {
        print(x);
    }

    template <class T, std::enable_if_t<itearate_struct::has_iterate_struct_helper_v<T>, int> = 0>
    void print_priv(const T& x) const
    {
        m_s << std::endl;
        ++m_depth;
        print(x);
        --m_depth;
    }

    stream& m_s;
    mutable unsigned int m_depth = 0;
};

} // namespace itearate_struct
