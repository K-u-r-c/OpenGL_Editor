add_library(feather STATIC
        public/feather.h
        private/core.cpp
        private/core.h
        private/precision.h private/random.cpp public/random.h private/particle.cpp private/particle.h)

target_include_directories(feather PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(feather PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/private)
target_include_directories(feather PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)