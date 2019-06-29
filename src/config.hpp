#ifndef TRIGON_CONFIG_HPP_
#define TRIGON_CONFIG_HPP_

#define COLOR_DEPTH 8
#if COLOR_DEPTH == 16
#define COLOR uint64_t
#else
#define COLOR uint32_t
#endif

#endif  // TRIGON_CONFIG_HPP_
