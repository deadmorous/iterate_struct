#pragma once

#include "iterate_struct.h"
#include <ostream>

namespace iterate_struct {

template<class stream>
class value_printer
{
public:
    value_printer(stream& s) : m_s(s) {}

    template<class T>
    void operator()(T& value, const char *name) const
    {
        maybePad(true);
        m_s << name << " = ";
        if (print_priv(value, false))
            m_s << std::endl;
    }

    template <class T>
    void print(const T& x) const {
        print_priv(x, true);
    }

private:
    class scoped_inc
    {
    public:
        scoped_inc(int& x) : m_x(x) {
            ++m_x;
        }
        ~scoped_inc() {
            --m_x;
        }
    private:
        int& m_x;
    };

    template <class T, std::enable_if_t<iterate_struct::has_output_operator_v<stream, T>, int> = 0>
    bool print_priv(const T& x, bool needPad) const
    {
        maybePad(needPad);
        m_s << x;
        return true;
    }

    template <class T, std::enable_if_t<iterate_struct::has_iterate_struct_helper_v<T>, int> = 0>
    bool print_priv(const T& x, bool needPad) const
    {
        if (needPad)
            for_each(x, *this);
        else {
            scoped_inc scinc(m_depth);
            for_each(x, *this);
        }
        return false;
    }

    template<class T>
    bool print_priv(const std::vector<T>& x, bool needPad) const
    {
        maybePad(needPad);
        m_s << "[" << std::endl;
        {
            scoped_inc scinc(m_depth);
            for (auto& xi : x) {
                if (print_priv(xi, true))
                    m_s << std::endl;
            }
        }
        maybePad(true);
        m_s << ']' << std::endl;
        return false;
    }

    void maybePad(bool needPad) const
    {
        if (needPad)
            m_s << std::string(m_depth<<2, ' ');
    }

    stream& m_s;
    mutable int m_depth = 0;
};

template<class stream, class T>
inline void print(stream& s, const T& x) {
    value_printer<stream>(s).print(x);
}

} // namespace iterate_struct
