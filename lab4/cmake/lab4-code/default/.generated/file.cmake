# The following variables contains the files used by the different stages of the build process.
set(lab4_code_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${lab4_code_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${lab4_code_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(lab4_code_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${lab4_code_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${lab4_code_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(lab4_code_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../ac.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../led.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../polling.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../usart.c")
set_source_files_properties(${lab4_code_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(lab4_code_default_default_XC8_FILE_TYPE_link)
set(lab4_code_default_image_name "default.elf")
set(lab4_code_default_image_base_name "default")

# The output directory of the final image.
set(lab4_code_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/lab4-code")

# The full path to the final image.
set(lab4_code_default_full_path_to_image ${lab4_code_default_output_dir}/${lab4_code_default_image_name})
