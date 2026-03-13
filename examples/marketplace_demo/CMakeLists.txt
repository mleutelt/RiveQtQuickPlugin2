qt_add_executable(riveqtquick_marketplace_demo
    main.cpp
    marketplacecatalog.cpp
    marketplacecatalog.h
)

riveqt_set_runtime_output_directory(riveqtquick_marketplace_demo "apps")

target_link_libraries(riveqtquick_marketplace_demo
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Network
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        RiveQtQuick
)
target_compile_definitions(riveqtquick_marketplace_demo
    PRIVATE
        RIVEQT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

add_dependencies(riveqtquick_marketplace_demo RiveQtQuickPlugin)
set(riveqtquick_marketplace_demo_asset_files
    "${CMAKE_CURRENT_SOURCE_DIR}/Main.qml"
)

add_custom_target(riveqtquick_marketplace_demo_assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.qml"
        "$<TARGET_FILE_DIR:riveqtquick_marketplace_demo>/Main.qml"
    DEPENDS ${riveqtquick_marketplace_demo_asset_files}
    COMMENT "Syncing marketplace demo assets"
)
add_dependencies(riveqtquick_marketplace_demo riveqtquick_marketplace_demo_assets)
riveqt_add_windeployqt(riveqtquick_marketplace_demo QMLDIR "${CMAKE_CURRENT_SOURCE_DIR}")
riveqt_deploy_local_qml_module(riveqtquick_marketplace_demo RiveQtQuick "${CMAKE_BINARY_DIR}/src/RiveQtQuick" RiveQtQuickPlugin)
riveqt_deploy_runtime_dll(riveqtquick_marketplace_demo RiveQtQuick)
