/* MIT License
*
* Copyright (c) 2022 Iman Ahmadvand
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef ODB_TRAITS_CHRONO
#define ODB_TRAITS_CHRONO

#include <chrono>
#include <odb/pgsql/traits.hxx>

namespace odb {
namespace pgsql {
///
/// This header includes the ODB value_traits specialization to perform std chrono types conversion to/from other PostgreSql types.
/// more: https://www.codesynthesis.com/~boris/blog/2012/07/18/custom-database-to-cxx-type-mapping-in-odb/
///

namespace chrono {
template<class T>
struct is_duration: std::false_type {
};

template<class Rep, class Period>
struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {
};

template<class T>
constexpr bool is_duration_t = is_duration<T>::value;

template<typename duration, typename value>
constexpr inline auto  set_duration(duration& output, const value& input, bool is_nul)
->std::enable_if_t < is_duration<duration>::value&& std::is_arithmetic<value>::value, void > {
    if (is_nul) {
        output = details::endian_traits::ntoh(std::numeric_limits<duration>::epsilon());
    } else {
        output = duration { details::endian_traits::ntoh(input) };
    }
}

template<typename duration, typename value>
constexpr inline auto  set_value(value& output, const duration& input, bool& is_null)
->std::enable_if_t < is_duration<duration>::value&& std::is_arithmetic<value>::value, void > {
    is_null = false;
    output  = details::endian_traits::hton(input.count());
}
} // namespace chrono

template<>
class value_traits<std::chrono::milliseconds, id_bigint> {
    using underlying_type = image_traits<id_bigint>::image_type;

  public:
    using value_type = std::chrono::milliseconds;
    using query_type = value_type;

    static constexpr inline void  set_value(value_type& output, const underlying_type& input, bool is_null) {
        chrono::set_duration(output, input, is_null);
    }

    static inline void  set_image(underlying_type& output, bool& is_null, const value_type& input) {
        chrono::set_value(output, input, is_null);
    }
};
} // namespace pgsql
} // namespace odb

#endif // ODB_TRAITS_CHRONO
