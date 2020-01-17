#pragma once

#include <iostream>
#include "iterate_struct.h"

#include <rapidjson/document.h>
#include <list>

namespace itearate_struct {

class json_doc_generator
{
public:
    template<class T>
    inline void operator()(T& value, const char *name) const
    {
        m_nodes.back().AddMember(
                    rapidjson::Value(name, m_doc.GetAllocator()),
                    generate_priv(value),
                    m_doc.GetAllocator());
    }

    template<class T>
    static rapidjson::Document generate(const T& value)
    {
        json_doc_generator generator;
        rapidjson::Value val = generator.generate_priv(value);
        generator.m_doc.Swap(val);
        return std::move(generator.m_doc);
    }

private:
    template <
            class T,
            std::enable_if_t<
                std::is_integral_v<T> ||
                std::is_floating_point_v<T>, int> = 0>
    rapidjson::Value generate_priv(const T& x) const
    {
        rapidjson::Value result;
        result.Set(x, m_doc.GetAllocator());
        return result;
    }

    rapidjson::Value generate_priv(const std::string& x) const
    {
        rapidjson::Value result;
        result.SetString(x.c_str(), m_doc.GetAllocator());
        return result;
    }

    template <class T, std::enable_if_t<itearate_struct::has_iterate_struct_helper_v<T>, int> = 0>
    rapidjson::Value generate_priv(const T& x) const
    {
        m_nodes.emplace_back(rapidjson::kObjectType);
        for_each(x, *this);
        auto result = std::move(m_nodes.back());
        m_nodes.pop_back();
        return result;
    }

    template<class T>
    rapidjson::Value generate_priv(const std::vector<T>& x) const
    {
        rapidjson::Value result(rapidjson::kArrayType);
        for (std::size_t i=0, n=x.size(); i<n; ++i)
            result.PushBack(std::move(generate_priv(x[i])), m_doc.GetAllocator());
        return result;
    }

    mutable rapidjson::Document m_doc;
    mutable std::list<rapidjson::Value> m_nodes;
};

template<class T>
inline rapidjson::Document to_json_doc(const T& value) {
    return json_doc_generator::generate(value);
}

} // namespace itearate_struct
