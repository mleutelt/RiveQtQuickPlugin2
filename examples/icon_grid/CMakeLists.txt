qt_add_executable(riveqtquick_icon_grid
    main.cpp
    icongridmodel.cpp
    icongridmodel.h
    "${CMAKE_SOURCE_DIR}/3rdparty/rive-cpp/utils/no_op_factory.cpp"
)

riveqt_set_runtime_output_directory(riveqtquick_icon_grid "apps")

target_link_libraries(riveqtquick_icon_grid
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        RiveQtQuick
        Rive::Official
)
target_compile_definitions(riveqtquick_icon_grid
    PRIVATE
        RIVEQT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_dependencies(riveqtquick_icon_grid RiveQtQuickPlugin)
add_custom_command(
    TARGET riveqtquick_icon_grid
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.qml"
        "$<TARGET_FILE_DIR:riveqtquick_icon_grid>/Main.qml"
    COMMENT "Copying icon grid Main.qml"
)
riveqt_add_windeployqt(riveqtquick_icon_grid QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}")
riveqt_deploy_local_qml_module(riveqtquick_icon_grid RiveQtQuick "${CMAKE_BINARY_DIR}/src/RiveQtQuick" RiveQtQuickPlugin)
riveqt_deploy_runtime_dll(riveqtquick_icon_grid RiveQtQuick)
