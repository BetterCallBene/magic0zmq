
find_package(cppzmq REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main 
    PRIVATE 
    cppzmq::cppzmq
    nlohmann_json::nlohmann_json
)
