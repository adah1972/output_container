This is a simple C++ header file to allow outputting of C++ containers.
It is generic and can work with any container that has `begin` and `end`
member functions. It avoids conflicts with existing output functions
(like that for `std::string`) by detecting whether they can be used.

You may want to check out [output_range][1] as well, which is a natural
extension of *output\_container*, supporting not only containers, but
also *ranges* like a C array or other C++20 ranges.

Using this file requires a C++17-compliant compiler.

[1]: https://github.com/adah1972/output_range
