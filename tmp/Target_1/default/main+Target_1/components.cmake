# components.cmake

# component ARM::CMSIS:CORE@5.6.0
add_library(ARM_CMSIS_CORE_5_6_0 INTERFACE)
target_include_directories(ARM_CMSIS_CORE_5_6_0 INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${CMSIS_PACK_ROOT}/ARM/CMSIS/5.9.0/CMSIS/Core/Include"
)
target_compile_definitions(ARM_CMSIS_CORE_5_6_0 INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_link_libraries(ARM_CMSIS_CORE_5_6_0 INTERFACE
  ${CONTEXT}_ABSTRACTIONS
)

# component Keil::Device:Startup@1.11.0
add_library(Keil_Device_Startup_1_11_0 OBJECT
  "${SOLUTION_ROOT}/RTE/Device/STM32H750VBTx/startup_stm32h750xx.s"
  "${SOLUTION_ROOT}/RTE/Device/STM32H750VBTx/system_stm32h7xx.c"
)
target_include_directories(Keil_Device_Startup_1_11_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  "${CMSIS_PACK_ROOT}/Keil/STM32H7xx_DFP/3.1.1/Drivers/CMSIS/Device/ST/STM32H7xx/Include"
)
target_compile_definitions(Keil_Device_Startup_1_11_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Keil_Device_Startup_1_11_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Keil_Device_Startup_1_11_0 PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  STM32H750xx
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/RTE/Device/STM32H750VBTx/startup_stm32h750xx.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)
