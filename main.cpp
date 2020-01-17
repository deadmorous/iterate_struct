#include <iostream>
#include "iterate_struct.h"
#include "value_printer.h"
#include "ptree_converter.h"

#include <boost/property_tree/json_parser.hpp>

// Example of usage

namespace my {

struct Foo {
    int bar = 1;
    std::string s = "s";
    double d = 2.3;
};

struct Bar {
    int x = 1;
    Foo foo;
};

} // namespace my

DESCRIBE_STRUCTURE_FIELDS(my::Foo, bar, s, d)
DESCRIBE_STRUCTURE_FIELDS(my::Bar, x, foo)

using namespace std;

int main()
{
    using namespace my;
    Foo foo;
    auto t = itearate_struct::asTuple(foo);
    get<0>(t) = 42;

    const Foo& cfoo = foo;
    auto ct = itearate_struct::asTuple(cfoo);
    // get<0>(ct) = 42;    // Doesn't compile

    cout << "Printing fields of Foo:" << endl;
    itearate_struct::value_printer(cout).print(foo);

    Bar bar;
    bar.foo.s = "Hello";
    cout << "Printing fields of Bar:" << endl;
    itearate_struct::value_printer(cout).print(bar);

    cout << "Bar as JSON:" << endl;
    auto pt = itearate_struct::to_ptree(bar);
    boost::property_tree::write_json(cout, pt);

    cout << "Bar read from JSON:" << endl;
    auto bar2 = itearate_struct::from_ptree<Bar>(pt);
    itearate_struct::value_printer(cout).print(bar2);

    return 0;
}
