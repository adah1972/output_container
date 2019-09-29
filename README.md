This is a simple C++ header file to allow outputting of C++ containers.
It is generic and can work with any container that has `begin` and `end`
member functions. It avoids conflicts with existing output functions
(like that for `std::string`) by detecting whether they can be used.

Using this function requires a C++17-compliant compiler.
