cmake_minimum_required(VERSION 3.16)
project(the_ark)

set(CMAKE_CXX_STANDARD 17)

add_executable(the_ark
        main.cpp
        TheArk.cpp
        Service.cpp
        TechnicalService.cpp
        BiologicalService.cpp
        MedicalService.cpp
        NavigationBlock.cpp
        NavigationService.cpp
        EmergencyService.cpp
        SocialService.cpp
        Population.cpp
        Resources.cpp
        Human.cpp
        Enums.cpp
        Interface.cpp)
