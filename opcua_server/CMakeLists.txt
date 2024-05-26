# optionally you can also specify a specific version
# e.g. find_package(open62541 1.0.0)
# find_package(open62541 REQUIRED COMPONENTS Events FullNamespace)
find_package(open62541)

add_executable(tutorial_server tutorial_server_firststeps.c)
target_link_libraries(tutorial_server open62541::open62541)

add_executable(server_nodeset_loader server_nodeset_loader.c nicla_sense.c)
target_link_libraries(server_nodeset_loader open62541::open62541)