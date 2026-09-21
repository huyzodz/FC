# groups.cmake

# group main
add_library(Group_main OBJECT
  "${SOLUTION_ROOT}/main/main.c"
)
target_include_directories(Group_main PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_main PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_main_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_main_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_main PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_main PUBLIC
  Group_main_ABSTRACTIONS
)

# group lib
add_library(Group_lib OBJECT
  "${SOLUTION_ROOT}/lib/clock.c"
  "${SOLUTION_ROOT}/lib/dma.c"
  "${SOLUTION_ROOT}/lib/dshot.c"
  "${SOLUTION_ROOT}/lib/gpio.c"
  "${SOLUTION_ROOT}/lib/i2c_master.c"
  "${SOLUTION_ROOT}/lib/usart.c"
  "${SOLUTION_ROOT}/lib/timer.c"
  "${SOLUTION_ROOT}/lib/spi_master.c"
)
target_include_directories(Group_lib PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_lib PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_lib_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "none" "on" "" "" CC_OPTIONS_FLAGS_Group_lib)
target_compile_options(Group_lib_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_Group_lib}"
  >
)
target_compile_options(Group_lib PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_lib PUBLIC
  Group_lib_ABSTRACTIONS
)

# group test
add_library(Group_test OBJECT
  "${SOLUTION_ROOT}/test/test_i2c.c"
  "${SOLUTION_ROOT}/test/test_bmi270.c"
  "${SOLUTION_ROOT}/test/test_task.c"
  "${SOLUTION_ROOT}/test/test_spi.c"
)
target_include_directories(Group_test PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_test PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_test_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_test_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_test PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_test PUBLIC
  Group_test_ABSTRACTIONS
)

# group system
add_library(Group_system OBJECT
  "${SOLUTION_ROOT}/main/system/init.c"
)
target_include_directories(Group_system PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_system PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_system_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_system_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_system PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_system PUBLIC
  Group_system_ABSTRACTIONS
)

# group driver
add_library(Group_driver OBJECT
  "${SOLUTION_ROOT}/main/driver/imu_bmi270.c"
  "${SOLUTION_ROOT}/main/driver/rasp_com.c"
  "${SOLUTION_ROOT}/main/driver/gps_be880.c"
)
target_include_directories(Group_driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_driver_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "speed" "on" "" "" CC_OPTIONS_FLAGS_Group_driver)
target_compile_options(Group_driver_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_Group_driver}"
  >
)
target_compile_options(Group_driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_driver PUBLIC
  Group_driver_ABSTRACTIONS
)

# group flight
add_library(Group_flight OBJECT
  "${SOLUTION_ROOT}/main/flight/esekf.c"
  "${SOLUTION_ROOT}/main/flight/pid.c"
  "${SOLUTION_ROOT}/main/flight/smc.c"
  "${SOLUTION_ROOT}/main/flight/drone.c"
  "${SOLUTION_ROOT}/main/flight/task.c"
  "${SOLUTION_ROOT}/main/flight/mixer.c"
)
target_include_directories(Group_flight PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_flight PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_flight_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "speed" "on" "" "" CC_OPTIONS_FLAGS_Group_flight)
target_compile_options(Group_flight_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_Group_flight}"
  >
)
target_compile_options(Group_flight PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_flight PUBLIC
  Group_flight_ABSTRACTIONS
)

# group scheduler
add_library(Group_scheduler OBJECT
  "${SOLUTION_ROOT}/main/scheduler/drone_scheduler.c"
)
target_include_directories(Group_scheduler PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_scheduler PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_scheduler_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "speed" "on" "" "" CC_OPTIONS_FLAGS_Group_scheduler)
target_compile_options(Group_scheduler_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_Group_scheduler}"
  >
)
target_compile_options(Group_scheduler PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_scheduler PUBLIC
  Group_scheduler_ABSTRACTIONS
)

# group simulation
add_library(Group_simulation OBJECT
  "${SOLUTION_ROOT}/simulation/simulate.c"
)
target_include_directories(Group_simulation PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_simulation PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
add_library(Group_simulation_ABSTRACTIONS INTERFACE)
target_link_libraries(Group_simulation_ABSTRACTIONS INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)
target_compile_options(Group_simulation PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_simulation PUBLIC
  Group_simulation_ABSTRACTIONS
)
