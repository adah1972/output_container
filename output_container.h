#ifndef OUTPUT_CONTAINER_H
#define OUTPUT_CONTAINER_H

#include <ostream>      // std::ostream
#include <type_traits>  // std::false_type/true_type/decay_t/is_same_v
#include <utility>      // std::declval/pair

template <typename T>
struct is_pair : std::false_type {};
template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};
template <typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

template <typename T>
struct has_output_function {
    template <class U>
    static auto output(U* ptr)
        -> decltype(std::declval<std::ostream&>() << *ptr,
                    std::declval<std::true_type>());
    template <class U>
    static std::false_type output(...);
    static const bool value =
        std::decay_t<decltype(output<T>(nullptr))>::value;
};
template <typename T>
inline constexpr bool has_output_function_v =
    has_output_function<T>::value;
// NB: Visual Studio 2017 (or below) may have problems with
//     has_output_function_v<T>: you should then use
//     has_output_function<T>::value instead, or upgrade to
//     Visual Studio 2019.

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& pr);

template <typename T>
void output_element(std::ostream& os, const T& element, std::true_type);
template <typename T>
void output_element(std::ostream& os, const T& element, std::false_type);

template <typename T,
          typename = std::enable_if_t<!has_output_function_v<T>>>
auto operator<<(std::ostream& os, const T& container)
    -> decltype(container.begin(), container.end(), os)
{
    using std::decay_t;
    using std::is_same_v;

    using element_type = decay_t<decltype(*container.begin())>;
    constexpr bool is_char_v = is_same_v<element_type, char>;
    if constexpr (!is_char_v) {
        os << "{ ";
    }
    if (!container.empty()) {
        auto end = container.end();
        bool on_first_element = true;
        for (auto it = container.begin(); it != end; ++it) {
            if constexpr (is_char_v) {
                if (*it == '\0') {
                    break;
                }
            }
            if constexpr (!is_char_v) {
                if (!on_first_element) {
                    os << ", ";
                } else {
                    on_first_element = false;
                }
            }
            output_element(os, *it, is_pair<element_type>());
        }
    }
    if constexpr (!is_char_v) {
        os << " }";
    }
    return os;
}

template <typename T>
void output_element(std::ostream& os, const T& element, std::true_type)
{
    os << element.first << " => " << element.second;
}

template <typename T>
void output_element(std::ostream& os, const T& element, std::false_type)
{
    os << element;
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& pr)
{
    os << '(' << pr.first << ", " << pr.second << ')';
    return os;
}

#endif // OUTPUT_CONTAINER_H
