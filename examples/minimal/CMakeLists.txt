qt_add_executable(riveqtquick_minimal
    main.cpp
)

riveqt_set_runtime_output_directory(riveqtquick_minimal "apps")

target_link_libraries(riveqtquick_minimal
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        RiveQtQuick
)
target_compile_definitions(riveqtquick_minimal
    PRIVATE
        RIVEQT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_dependencies(riveqtquick_minimal RiveQtQuickPlugin)
add_custom_command(
    TARGET riveqtquick_minimal
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.qml"
        "$<TARGET_FILE_DIR:riveqtquick_minimal>/Main.qml"
    COMMENT "Copying minimal Main.qml"
)
riveqt_add_windeployqt(riveqtquick_minimal QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}")
riveqt_deploy_local_qml_module(riveqtquick_minimal RiveQtQuick "${CMAKE_BINARY_DIR}/src/RiveQtQuick" RiveQtQuickPlugin)
riveqt_deploy_runtime_dll(riveqtquick_minimal RiveQtQuick)
