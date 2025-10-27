# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(lab4_code_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(lab4_code_default_default_XC8_FILE_TYPE_assemble)
add_library(lab4_code_default_default_XC8_assemble OBJECT ${lab4_code_default_default_XC8_FILE_TYPE_assemble})
    lab4_code_default_default_XC8_assemble_rule(lab4_code_default_default_XC8_assemble)
    list(APPEND lab4_code_default_library_list "$<TARGET_OBJECTS:lab4_code_default_default_XC8_assemble>")
endif()

# Handle files with suffix S, for group default-XC8
if(lab4_code_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(lab4_code_default_default_XC8_assemblePreprocess OBJECT ${lab4_code_default_default_XC8_FILE_TYPE_assemblePreprocess})
    lab4_code_default_default_XC8_assemblePreprocess_rule(lab4_code_default_default_XC8_assemblePreprocess)
    list(APPEND lab4_code_default_library_list "$<TARGET_OBJECTS:lab4_code_default_default_XC8_assemblePreprocess>")
endif()

# Handle files with suffix [cC], for group default-XC8
if(lab4_code_default_default_XC8_FILE_TYPE_compile)
add_library(lab4_code_default_default_XC8_compile OBJECT ${lab4_code_default_default_XC8_FILE_TYPE_compile})
    lab4_code_default_default_XC8_compile_rule(lab4_code_default_default_XC8_compile)
    list(APPEND lab4_code_default_library_list "$<TARGET_OBJECTS:lab4_code_default_default_XC8_compile>")
endif()

add_executable(lab4_code_default_image_RC9vr_y9 ${lab4_code_default_library_list})

set_target_properties(lab4_code_default_image_RC9vr_y9 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${lab4_code_default_output_dir})
set_target_properties(lab4_code_default_image_RC9vr_y9 PROPERTIES OUTPUT_NAME "default")
set_target_properties(lab4_code_default_image_RC9vr_y9 PROPERTIES SUFFIX ".elf")

target_link_libraries(lab4_code_default_image_RC9vr_y9 PRIVATE ${lab4_code_default_default_XC8_FILE_TYPE_link})


# Add the link options from the rule file.
lab4_code_default_link_rule(lab4_code_default_image_RC9vr_y9)



