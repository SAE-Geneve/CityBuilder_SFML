#ifndef CORE_PROFILING_PROFILING_H
#define CORE_PROFILING_PROFILING_H

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#define PROFILE_ZONE()           ZoneScoped
#define PROFILE_ZONE_N(name)     ZoneScopedN(name)
#define PROFILE_FRAME()          FrameMark
#define PROFILE_PLOT(name, val)  TracyPlot(name, val)
#else
#define PROFILE_ZONE()           ((void)0)
#define PROFILE_ZONE_N(name)     ((void)0)
#define PROFILE_FRAME()          ((void)0)
#define PROFILE_PLOT(name, val)  ((void)0)
#endif

#endif  // CORE_PROFILING_PROFILING_H
