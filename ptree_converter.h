#pragma once

#include <iostream>
#include "iterate_struct.h"

#include <boost/property_tree/ptree.hpp>
#include <list>
#include <vector>

namespace itearate_struct {

class ptree_generator
{
public:
    template<class T>
    inline void operator()(T& value, const char *name) const
    {
        m_nodes.back().put_child(
                    name,
                    std::move(generate_priv(value)));    // Maybe one day there will be an overload taking rvalue
    }

    template<class T>
    static boost::property_tree::ptree generate(const T& value) {
        return ptree_generator().generate_priv(value);
    }

private:
    template <
            class T,
            std::enable_if_t<
                std::is_integral_v<T> ||
                std::is_floating_point_v<T> ||
                std::is_same_v<T, std::string>, int> = 0>
    boost::property_tree::ptree generate_priv(const T& x) const
    {
        boost::property_tree::ptree result;
        result.put_value(x);
        return result;
    }

    template <class T, std::enable_if_t<itearate_struct::has_iterate_struct_helper_v<T>, int> = 0>
    boost::property_tree::ptree generate_priv(const T& x) const
    {
        m_nodes.emplace_back();
        itearate_struct::for_each(x, *this);
        auto result = m_nodes.back();
        m_nodes.pop_back();
        return result;
    }

    mutable std::list<boost::property_tree::ptree> m_nodes;
};

template<class T>
inline boost::property_tree::ptree to_ptree(const T& value) {
    return ptree_generator::generate(value);
}



class ptree_parser
{
public:
    template<class T>
    inline void operator()(T& value, const char *name) const {
        value = parse_priv<T>(m_nodes.back()->get_child(name));
    }

    template<class T>
    static T parse(const boost::property_tree::ptree& node) {
        return ptree_parser().parse_priv<T>(node);
    }

private:
    template <
            class T,
            std::enable_if_t<
                std::is_integral_v<T> ||
                std::is_floating_point_v<T> ||
                std::is_same_v<T, std::string>, int> = 0>
    T parse_priv(const boost::property_tree::ptree& node) const {
        return node.get_value<T>();
    }

    template <class T, std::enable_if_t<itearate_struct::has_iterate_struct_helper_v<T>, int> = 0>
    T parse_priv(const boost::property_tree::ptree& node) const
    {
        T result;
        m_nodes.push_back(&node);
        itearate_struct::for_each(result, *this);
        m_nodes.pop_back();
        return result;
    }

    mutable std::vector<const boost::property_tree::ptree*> m_nodes;
};

template<class T>
inline T from_ptree(const boost::property_tree::ptree& node) {
    return ptree_parser::parse<T>(node);
}

} // namespace itearate_struct
