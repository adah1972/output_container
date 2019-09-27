#ifndef OUTPUT_CONTAINER_H
#define OUTPUT_CONTAINER_H

#include <ostream>              // std::ostream
#include <string>               // std::basic_string
#include <string_view>          // std::basic_string_view
#include <type_traits>          // std::false_type/true_type/is_same_v/...
#include <utility>              // std::pair

using std::ostream;
using std::false_type;
using std::true_type;

using std::enable_if_t;
using std::is_same_v;
using std::remove_cv_t;
using std::remove_reference_t;

template <typename T>
struct is_pair : false_type {};
template <typename T, typename U>
struct is_pair<std::pair<T, U>> : true_type {};
template <typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

template <typename T>
struct has_std_output : false_type {};
template <typename T>
struct has_std_output<std::basic_string<T>> : true_type {};
template <typename T>
struct has_std_output<std::basic_string_view<T>> : true_type {};
template <typename T>
inline constexpr bool has_std_output_v = has_std_output<T>::value;

template <typename T, typename U>
ostream& operator<<(ostream& os, const std::pair<T, U>& pr);

template <typename T>
void output_element(ostream& os, const T& element, true_type);
template <typename T>
void output_element(ostream& os, const T& element, false_type);

template <typename T,
          typename = enable_if_t<!has_std_output_v<T>>>
auto operator<<(ostream& os, const T& container)
    -> decltype(container.begin(), container.end(), os)
{
    using element_type = remove_cv_t<
        remove_reference_t<decltype(*container.begin())>>;
    using is_element_pair = is_pair<element_type>;
    constexpr bool is_char_v = is_same_v<element_type, char>;
    if constexpr (!is_char_v) {
        os << "{ ";
    }
    if (!container.empty()) {
        auto end = container.end();
        --end;
        for (auto it = container.begin(); it != end; ++it) {
            if constexpr (is_char_v) {
                if (*it == '\0') {
                    break;
                }
            }
            output_element(os, *it, is_element_pair());
            if constexpr (!is_char_v) {
                os << ", ";
            }
        }
        if constexpr (is_char_v) {
            if (*end != '\0') {
                os << *end;
            }
        } else {
            output_element(os, *end, is_element_pair());
        }
    }
    if constexpr (!is_char_v) {
        os << " }";
    }
    return os;
}

template <typename T>
void output_element(ostream& os, const T& element, true_type)
{
    os << element.first << " => " << element.second;
}

template <typename T>
void output_element(ostream& os, const T& element, false_type)
{
    os << element;
}

template <typename T, typename U>
ostream& operator<<(ostream& os, const std::pair<T, U>& pr)
{
    os << '(' << pr.first << ", " << pr.second << ')';
    return os;
}

#endif // OUTPUT_CONTAINER_H
