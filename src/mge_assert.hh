#ifndef MGE_MGE_ASSERT
#define MGE_MGE_ASSERT

#include "mgepch.hh"

namespace mge {
void vlog(const char* condition, const char* file, int line, fmt::string_view fmt, fmt::format_args args) {
  fmt::print("assertion failed: {}\n{}: {}: {}", condition, file, line, fmt::vformat(fmt, args));
}

template <typename... T>
void log(const char* condition, const char* file, int line, fmt::format_string<T...> fmt, T&&... args) {
  vlog(condition, file, line, fmt, fmt::make_format_args(args...));
}
}  // namespace mge

#ifdef NDEBUG
#define MGE_ASSERT(...)
#else
#define MGE_ASSERT_FMT(condition, fmt, ...)                     \
  if (!(condition)) {                                           \
    mge::log(#condition, __FILE__, __LINE__, fmt, __VA_ARGS__); \
    std::abort();                                               \
  }
#define MGE_ASSERT_MSG(condition, msg) MGE_ASSERT_FMT(condition, msg, "")
#define MGE_ASSERT_NOMSG(condition) MGE_ASSERT_FMT(condition, "", "")
#define MGE_ASSERT_HELPER(x, A, B, C, FUNC, ...) FUNC
#define MGE_ASSERT(...)                                                                        \
  MGE_ASSERT_HELPER(, ##__VA_ARGS__, MGE_ASSERT_FMT(__VA_ARGS__), MGE_ASSERT_MSG(__VA_ARGS__), \
                    MGE_ASSERT_NOMSG(__VA_ARGS__))
#endif

#endif  // MGE_MGE_ASSERT