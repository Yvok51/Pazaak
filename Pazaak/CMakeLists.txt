# CMakeList.txt : CMake project for Pazaak, include source and define
# project specific logic here.
#

# Dependencies
# SFML
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/Pazaak/dependencies) 
set(SFML_BASE ${DEPENDENCIES_DIR}/SFML_Linux)
set(SFML_INCLUDE ${SFML_BASE}/include)
set(SFML_LIB ${SFML_BASE}/lib)

# Cereal
set(CEREAL_INCLUDE ${DEPENDENCIES_DIR}/cereal/include)

# Add source to this project's executable.
file(GLOB PAZAAK_SRC CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
)

add_executable(${PROJECT_NAME} ${PAZAAK_SRC})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set(SFML_DIR "${SFML_BASE}/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics system window REQUIRED)


# Add SFML dependency
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE})

# Add Cereal dependency
target_include_directories(${PROJECT_NAME} PUBLIC ${CEREAL_INCLUDE})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)

# Copy texxtures and fonts
file(COPY "Textures" DESTINATION ".")
file(COPY "Fonts" DESTINATION ".")