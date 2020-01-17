#include <iostream>
#include "iterate_struct.h"
#include "value_printer.h"
#include "ptree_converter.h"
#include "json_doc_converter.h"
#include "json_doc_io.h"

#include <boost/property_tree/json_parser.hpp>
// #include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/pointer.h>

// Example of usage

namespace my {

struct Foo {
    std::vector<int> bar = {1, 2, 3};
    std::string s = "s";
    double d = 2.3;
};

struct Bar {
    int x = 1;
    std::vector<Foo> foo {Foo(), Foo()};
};

} // namespace my

DESCRIBE_STRUCTURE_FIELDS(my::Foo, bar, s, d)
DESCRIBE_STRUCTURE_FIELDS(my::Bar, x, foo)

using namespace std;

int main()
{
    using namespace my;
    Foo foo;
    auto t = iterate_struct::asTuple(foo);
    get<0>(t)[1] = 42;

    const Foo& cfoo = foo;
    auto ct = iterate_struct::asTuple(cfoo);
    // get<0>(ct) = 42;    // Doesn't compile

    cout << "Printing fields of Foo:" << endl;
    iterate_struct::print(cout, foo);

    Bar bar;
    bar.foo[0].s = "Hello";
    bar.foo[1].bar[2] = 100500;
    cout << "Printing fields of Bar:" << endl;
    iterate_struct::print(cout, bar);

    cout << "Bar as JSON using ptree:" << endl;
    auto pt = iterate_struct::to_ptree(bar);
    boost::property_tree::write_json(cout, pt);

    cout << "Bar read from JSON using ptree:" << endl;
    auto bar2 = iterate_struct::from_ptree<Bar>(pt);
    iterate_struct::print(cout, bar2);

    cout << "JSON generated with rapidjson" << endl;
    auto jsdoc = iterate_struct::to_json_doc(bar);

    auto p = rapidjson::GetValueByPointer(jsdoc, "/foo/0/bar");
    auto v = p->GetArray();
    rapidjson::SetValueByPointer(jsdoc, "/a/s/d", 123);

    write_json_doc(cout, jsdoc);
    cout << endl;

    cout << "Bar read from JSON: using rapidjson" << endl;
    auto bar3 = iterate_struct::from_json_doc<Bar>(jsdoc);
    iterate_struct::print(cout, bar);

    // auto doc2 = read_json_doc(cin);

    return 0;
}
