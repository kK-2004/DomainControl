cmake_minimum_required(VERSION 3.16)

project(domin_control VERSION 0.1 LANGUAGES CXX)

# 启用 Qt 的自动处理
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 模块，包括 QtWidgets 和 QtSql
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Sql Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Core)

# 包含自定义头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)


# 定义项目源文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    registwindow.cpp
    registwindow.h
    registwindow.ui
    dataBase.cpp
    dataBase.h
)

# 创建项目的可执行文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Qt 6 的处理方式
    qt_add_executable(domin_control WIN32
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        userwindow.ui
        userwindow.h
        userwindow.cpp
        user_updatepassword.ui
        user_updatepassword.cpp
        user_updatepassword.h
        authentication.ui
        authentication.cpp
        authentication.h
        managerwindow.ui
        managerwindow.cpp
        managerwindow.h
        dealauthen.ui
        dealauthen.cpp
        dealauthen.h
        subdealauthen.ui
        subdealauthen.cpp
        subdealauthen.h

        tools.cpp
        createTree.cpp
        findTree.cpp

        tools.h
        headers.h
        createTree.h
        findTree.h
        domainDetails.ui
        domainDetails.h
        domainDetails.cpp
        deleteDomain.h
        deleteDomain.cpp
        insertDomain.cpp
        insertDomain.h
        userdomainGet.h
        userdomainGet.cpp
        userDetails.ui
        userDetails.cpp
        userDetails.h
        headers.cpp
        saveDomain.h
        saveDomain.cpp
        categoryCount.h
        categoryCount.cpp
        domainOverview.ui
        domainOverview.h
        domainOverview.cpp
        categoryFind.h
        categoryFind.cpp
        categoryDetails.ui
        categoryDetails.cpp
        categoryDetails.h
        resource.qrc



    )
else()
    # Qt 5 或者其他版本处理方式
    add_executable(domin_control
        ${PROJECT_SOURCES}
    )
endif()


# 链接 Qt 库
target_link_libraries(domin_control PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)
