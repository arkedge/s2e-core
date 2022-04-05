/**
 * @file
 * @brief header for physical constant values
 */

#ifndef PHYSICAL_CONSTANT_HPP_
#define PHYSICAL_CONSTANT_HPP_

#include <type_traits>  // std::is_floating_point_v

namespace libra {

// instead of C++20 std::numbers
inline namespace numbers {
template <typename T>
using enable_if_float = std::enable_if_t<std::is_floating_point_v<T>, T>;

#define DEFINE_PHYSICAL_CONSTANT(name, value)              \
  template <typename T>                                    \
  inline constexpr T name##_v = enable_if_float<T>(value); \
  inline constexpr double name = name##_v<double>;

// TODO: Is the unit information needed in variable names?
// Physics
DEFINE_PHYSICAL_CONSTANT(speed_of_light_m_s, 299792458.0L);              /* in vacuum m/s */
DEFINE_PHYSICAL_CONSTANT(boltzmann_constant_J_K, 1.380649e-23L);         /* J/K */
DEFINE_PHYSICAL_CONSTANT(absolute_zero_degC, -273.15L);                  /* degC */

// Astronomy Ref: https://iau-a3.gitlab.io/NSFA/
DEFINE_PHYSICAL_CONSTANT(astronomical_unit_m, 1.49597870700e11L);               /* fixed m */
DEFINE_PHYSICAL_CONSTANT(gravitational_constant_Nm2_kg2, 6.67428e-11L);         /* best estimates Nm2/kg2 */
DEFINE_PHYSICAL_CONSTANT(earth_equatorial_radius_m, 6378136.6L);                /* best estimates m */
DEFINE_PHYSICAL_CONSTANT(earth_polar_radius_m, 6378136.6L);                     /* best estimates m */
DEFINE_PHYSICAL_CONSTANT(earth_gravitational_constant_m3_s2, 3.986004415e14L);  /* best estimates, TT m3/s2 */


#undef DEFINE_PHYSICAL_CONSTANT
}  // namespace numbers

}  // namespace libra

#endif  // PHYSICAL_CONSTANT_HPP_
