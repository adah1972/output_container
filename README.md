This is a simple C++ header file to allow outputting of C++ containers.
It is generic and can work with any containers that have `begin` and
`end` member functions.

To avoid ambiguity with standard output functions that work with
`std::string` and `std::string_view`, they are specifically disabled
from using this `operator<<` by specializing the `has_std_output` trait.
If other classes conflicted with this implementation, they could be
disabled similarly.
