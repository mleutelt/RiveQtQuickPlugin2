function(riveqt_set_runtime_output_directory target group_name)
    if(CMAKE_CONFIGURATION_TYPES)
        set(output_dir "${CMAKE_BINARY_DIR}/${group_name}/$<CONFIG>/${target}")
    else()
        set(output_dir "${CMAKE_BINARY_DIR}/${group_name}/${target}")
    endif()

    set_target_properties(${target}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${output_dir}"
            PDB_OUTPUT_DIRECTORY "${output_dir}"
            VS_DEBUGGER_WORKING_DIRECTORY "${output_dir}"
    )
endfunction()

function(riveqt_add_windeployqt target)
    if(NOT WIN32)
        return()
    endif()

    find_program(WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt windeployqt.exe
        HINTS
            "${QT_HOST_PATH}/bin"
            "C:/Qt/6.9.2/msvc2022_64/bin"
    )

    if(NOT WINDEPLOYQT_EXECUTABLE)
        message(WARNING "windeployqt was not found; ${target} will not get post-build Qt deployment.")
        return()
    endif()

    set(oneValueArgs QMLDIR)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "" ${ARGN})

    set(command
        "${WINDEPLOYQT_EXECUTABLE}"
        "--$<IF:$<CONFIG:Debug>,debug,release>"
        "--force"
        "--no-compiler-runtime"
        "--dir" "$<TARGET_FILE_DIR:${target}>"
    )

    if(ARG_QMLDIR)
        list(APPEND command "--qmldir" "${ARG_QMLDIR}")
    endif()

    list(APPEND command "$<TARGET_FILE:${target}>")

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${command}
        COMMENT "Deploying Qt runtime for ${target}"
        VERBATIM
    )
endfunction()

function(riveqt_deploy_local_qml_module target module_name module_build_dir plugin_target)
    if(NOT WIN32)
        return()
    endif()

    set(module_target_dir "$<TARGET_FILE_DIR:${target}>/qml/${module_name}")
    set(copy_target "${target}_${module_name}_module_deploy")

    add_custom_target(${copy_target}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${module_target_dir}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${module_build_dir}/qmldir"
            "${module_target_dir}/qmldir"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${module_build_dir}/${module_name}.qmltypes"
            "${module_target_dir}/${module_name}.qmltypes"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${plugin_target}>"
            "${module_target_dir}/$<TARGET_FILE_NAME:${plugin_target}>"
        DEPENDS
            ${plugin_target}
            "${module_build_dir}/qmldir"
            "${module_build_dir}/${module_name}.qmltypes"
        COMMENT "Deploying local QML module ${module_name} for ${target}"
        VERBATIM
    )
    add_dependencies(${target} ${copy_target})
endfunction()

function(riveqt_deploy_runtime_dll target runtime_target)
    if(NOT WIN32)
        return()
    endif()

    set(copy_target "${target}_${runtime_target}_runtime_deploy")

    add_custom_target(${copy_target}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${runtime_target}>"
            "$<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_NAME:${runtime_target}>"
        DEPENDS ${runtime_target}
        COMMENT "Deploying runtime library $<TARGET_FILE_NAME:${runtime_target}> for ${target}"
        VERBATIM
    )
    add_dependencies(${target} ${copy_target})
endfunction()

function(riveqt_deploy_platform_plugin target plugin_name)
    if(NOT WIN32)
        return()
    endif()

    set(source_plugin "C:/Qt/6.9.2/msvc2022_64/plugins/platforms/${plugin_name}")
    if(NOT EXISTS "${source_plugin}")
        message(WARNING "Requested platform plugin ${plugin_name} was not found at ${source_plugin}.")
        return()
    endif()

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${source_plugin}"
            "$<TARGET_FILE_DIR:${target}>/platforms/${plugin_name}"
        COMMENT "Deploying platform plugin ${plugin_name} for ${target}"
        VERBATIM
    )
endfunction()
