qt_add_executable(riveqtquick_interactive
    main.cpp
)

riveqt_set_runtime_output_directory(riveqtquick_interactive "apps")

target_link_libraries(riveqtquick_interactive
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        RiveQtQuick
)
target_compile_definitions(riveqtquick_interactive
    PRIVATE
        RIVEQT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_dependencies(riveqtquick_interactive RiveQtQuickPlugin)
add_custom_command(
    TARGET riveqtquick_interactive
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.qml"
        "$<TARGET_FILE_DIR:riveqtquick_interactive>/Main.qml"
    COMMENT "Copying interactive Main.qml"
)
riveqt_add_windeployqt(riveqtquick_interactive QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}")
riveqt_deploy_local_qml_module(riveqtquick_interactive RiveQtQuick "${CMAKE_BINARY_DIR}/src/RiveQtQuick" RiveQtQuickPlugin)
riveqt_deploy_runtime_dll(riveqtquick_interactive RiveQtQuick)
