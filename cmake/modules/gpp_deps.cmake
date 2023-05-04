function (init_gpp_deps TARGET_NAME)

   if (NOT TARGET hashmap_static)
  FetchContent_Declare(
    hashmap_static
    GIT_REPOSITORY https://github.com/sebbekarlsson/hashmap.git
    GIT_TAG master
    )
  FetchContent_MakeAvailable(hashmap_static)
  endif()

  if (NOT TARGET json_static)
  FetchContent_Declare(
    json_static
    GIT_REPOSITORY https://github.com/sebbekarlsson/libjson.git
    GIT_TAG master
    )
  FetchContent_MakeAvailable(json_static)
endif()

  set(GPP_LIBRARIES m hashmap_static json_static)
  target_link_libraries(${TARGET_NAME} PRIVATE ${GPP_LIBRARIES})
endfunction()
