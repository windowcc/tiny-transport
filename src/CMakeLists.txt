
set(${PROJECT_NAME}_udp_source_files
    udp/UDPReceiverResource.cpp
    udp/UDPTransportInterface.cpp
    udp/UDPv4Transport.cpp
    udp/UDPv6Transport.cpp
)

set(${PROJECT_NAME}_source_files
    TransportFactory.cpp
    IPFinder.cpp
    IPLocator.cpp
)

if (LIBIPC_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED
    ${${PROJECT_NAME}_udp_source_files}
    ${${PROJECT_NAME}_source_files}
    ${HEAD_FILES}
  )

target_compile_definitions(${PROJECT_NAME}
    INTERFACE
      LIBTRANSPORT_LIBRARY_SHARED_USING__
    PRIVATE
      LIBTRANSPORT_LIBRARY_SHARED_BUILDING__)
else()
  add_library(${PROJECT_NAME} STATIC 
    ${${PROJECT_NAME}_source_files}
    ${${PROJECT_NAME}_sync_source_files}
    ${HEAD_FILES}
  )
endif()

# set (PACKAGE_VERSION 1.3.0)
# set output directory
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_include_directories(${PROJECT_NAME}
  PUBLIC  
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  PRIVATE ${PROJECT_SOURCE_DIR}
          ${PROJECT_SOURCE_DIR}/build)

if(NOT MSVC)
  target_link_libraries(${PROJECT_NAME} PUBLIC
    $<$<NOT:$<STREQUAL:${CMAKE_SYSTEM_NAME},QNX>>:pthread>
    $<$<NOT:$<OR:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,$<STREQUAL:${CMAKE_SYSTEM_NAME},QNX>>>:rt>
    PRIVATE
    uv
    uvw
    )
endif()

install(
  TARGETS ${PROJECT_NAME}
  EXPORT tiny-transport-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(EXPORT tiny-transport-targets
	FILE tiny-transport-targets.cmake
	NAMESPACE tiny-transport::
	DESTINATION share/tiny-transport
)
