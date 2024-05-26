# optionally you can also specify a specific version
# e.g. find_package(open62541 1.0.0)
# find_package(open62541 REQUIRED COMPONENTS Events FullNamespace)
find_package(open62541)

add_executable(nicla_opcua_server nicla_opcua_server.c nicla_sense.c)
target_link_libraries(nicla_opcua_server open62541::open62541)