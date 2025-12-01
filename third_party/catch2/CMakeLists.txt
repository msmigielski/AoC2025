Include(FetchContent)

set(FETCHCONTENT_QUIET off)
set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/third_party/catch2)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        b3fb4b9feafcd8d91c5cb510a4775143fdbef02f # v3.11.0
)

FetchContent_MakeAvailable(Catch2)