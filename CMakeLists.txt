cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(NES_Emulator CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME NES_Emulator)

################################################################################
# Source groups
################################################################################
set(Header_Files__Logger
    "src/logger/logger.h"
)
source_group("Header Files\\Logger" FILES ${Header_Files__Logger})

set(Header_Files__Machine
    "src/machine/machine.h"
)
source_group("Header Files\\Machine" FILES ${Header_Files__Machine})

set(Header_Files__Machine__APU
    "src/machine/apu/apu.h"
    "src/machine/apu/dmc_channel.h"
    "src/machine/apu/frame_counter.h"
    "src/machine/apu/noise_channel.h"
    "src/machine/apu/pulse_channel.h"
    "src/machine/apu/triangle_channel.h"
)
source_group("Header Files\\Machine\\APU" FILES ${Header_Files__Machine__APU})

set(Header_Files__Machine__Bus
    "src/machine/bus/bus.h"
)
source_group("Header Files\\Machine\\Bus" FILES ${Header_Files__Machine__Bus})

set(Header_Files__Machine__CPU
    "src/machine/cpu/addressingmode.h"
    "src/machine/cpu/cpu.h"
    "src/machine/cpu/flags.h"
    "src/machine/cpu/opcodes.h"
    "src/machine/cpu/register.h"
)
source_group("Header Files\\Machine\\CPU" FILES ${Header_Files__Machine__CPU})

set(Header_Files__Machine__Input
    "src/machine/input/input.h"
    "src/machine/input/joypad.h"
)
source_group("Header Files\\Machine\\Input" FILES ${Header_Files__Machine__Input})

set(Header_Files__Machine__Mapper
    "src/machine/mapper/cnrom.h"
    "src/machine/mapper/mapper.h"
    "src/machine/mapper/mmc1.h"
    "src/machine/mapper/mmc3.h"
    "src/machine/mapper/nrom.h"
    "src/machine/mapper/uxrom.h"
)
source_group("Header Files\\Machine\\Mapper" FILES ${Header_Files__Machine__Mapper})

set(Header_Files__Machine__Misc
    "src/machine/misc/constants.h"
    "src/machine/misc/instruction.h"
    "src/machine/misc/machine_status.h"
    "src/machine/misc/nes_data.h"
)
source_group("Header Files\\Machine\\Misc" FILES ${Header_Files__Machine__Misc})

set(Header_Files__Machine__PPU
    "src/machine/ppu/ppu.h"
    "src/machine/ppu/ppuregisters.h"
)
source_group("Header Files\\Machine\\PPU" FILES ${Header_Files__Machine__PPU})

set(Header_Files__Machine__PPU__Display
    "src/machine/ppu/display/display.h"
    "src/machine/ppu/display/palette.h"
)
source_group("Header Files\\Machine\\PPU\\Display" FILES ${Header_Files__Machine__PPU__Display})

set(Header_Files__Machine__PPU__Registers
    "src/machine/ppu/ppuaddr.h"
    "src/machine/ppu/ppuctrl.h"
    "src/machine/ppu/ppudata.h"
    "src/machine/ppu/ppumask.h"
    "src/machine/ppu/ppuscroll.h"
    "src/machine/ppu/ppustatus.h"
)
source_group("Header Files\\Machine\\PPU\\Registers" FILES ${Header_Files__Machine__PPU__Registers})

set(Header_Files__Machine__UserInterface
    "src/machine/user_interface/menu_bar.h"
    "src/machine/user_interface/user_interface.h"
    "src/machine/user_interface/window_about.h"
    "src/machine/user_interface/window_controls.h"
    "src/machine/user_interface/window_debugger.h"
)
source_group("Header Files\\Machine\\UserInterface" FILES ${Header_Files__Machine__UserInterface})

set(Header_Files__Machine__UserInterface__Units
    "src/machine/user_interface/asm_list.h"
    "src/machine/user_interface/button.h"
    "src/machine/user_interface/checkbox.h"
    "src/machine/user_interface/element.h"
    "src/machine/user_interface/list.h"
    "src/machine/user_interface/text.h"
    "src/machine/user_interface/textbox.h"
    "src/machine/user_interface/window.h"
)
source_group("Header Files\\Machine\\UserInterface\\Units" FILES ${Header_Files__Machine__UserInterface__Units})

set(Header_Files__Timing
    "src/timing/frame.h"
    "src/timing/timer.h"
)
source_group("Header Files\\Timing" FILES ${Header_Files__Timing})

set(Header_Files__Utility
    "src/utility/utility.h"
)
source_group("Header Files\\Utility" FILES ${Header_Files__Utility})

set(Source_Files
    "src/main.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__Logger
    "src/logger/logger.cpp"
)
source_group("Source Files\\Logger" FILES ${Source_Files__Logger})

set(Source_Files__Machine
    "src/machine/machine.cpp"
)
source_group("Source Files\\Machine" FILES ${Source_Files__Machine})

set(Source_Files__Machine__APU
    "src/machine/apu/apu.cpp"
    "src/machine/apu/dmc_channel.cpp"
    "src/machine/apu/frame_counter.cpp"
    "src/machine/apu/noise_channel.cpp"
    "src/machine/apu/pulse_channel.cpp"
    "src/machine/apu/triangle_channel.cpp"
)
source_group("Source Files\\Machine\\APU" FILES ${Source_Files__Machine__APU})

set(Source_Files__Machine__Bus
    "src/machine/bus/bus.cpp"
)
source_group("Source Files\\Machine\\Bus" FILES ${Source_Files__Machine__Bus})

set(Source_Files__Machine__CPU
    "src/machine/cpu/cpu.cpp"
    "src/machine/cpu/flags.cpp"
    "src/machine/cpu/is_load.cpp"
    "src/machine/cpu/opcodes.cpp"
    "src/machine/cpu/register.cpp"
)
source_group("Source Files\\Machine\\CPU" FILES ${Source_Files__Machine__CPU})

set(Source_Files__Machine__Input
    "src/machine/input/input.cpp"
    "src/machine/input/joypad.cpp"
)
source_group("Source Files\\Machine\\Input" FILES ${Source_Files__Machine__Input})

set(Source_Files__Machine__Mapper
    "src/machine/mapper/cnrom.cpp"
    "src/machine/mapper/mapper.cpp"
    "src/machine/mapper/mmc1.cpp"
    "src/machine/mapper/mmc3.cpp"
    "src/machine/mapper/nrom.cpp"
    "src/machine/mapper/uxrom.cpp"
)
source_group("Source Files\\Machine\\Mapper" FILES ${Source_Files__Machine__Mapper})

set(Source_Files__Machine__Misc
    "src/machine/misc/instruction.cpp"
)
source_group("Source Files\\Machine\\Misc" FILES ${Source_Files__Machine__Misc})

set(Source_Files__Machine__PPU
    "src/machine/ppu/ppu.cpp"
    "src/machine/ppu/ppuregisters.cpp"
)
source_group("Source Files\\Machine\\PPU" FILES ${Source_Files__Machine__PPU})

set(Source_Files__Machine__PPU__Display
    "src/machine/ppu/display/display.cpp"
    "src/machine/ppu/display/palette.cpp"
)
source_group("Source Files\\Machine\\PPU\\Display" FILES ${Source_Files__Machine__PPU__Display})

set(Source_Files__Machine__PPU__Registers
    "src/machine/ppu/ppuaddr.cpp"
    "src/machine/ppu/ppuctrl.cpp"
    "src/machine/ppu/ppudata.cpp"
    "src/machine/ppu/ppumask.cpp"
    "src/machine/ppu/ppuscroll.cpp"
    "src/machine/ppu/ppustatus.cpp"
)
source_group("Source Files\\Machine\\PPU\\Registers" FILES ${Source_Files__Machine__PPU__Registers})

set(Source_Files__Machine__UserInterface
    "src/machine/user_interface/menu_bar.cpp"
    "src/machine/user_interface/user_interface.cpp"
    "src/machine/user_interface/window_about.cpp"
    "src/machine/user_interface/window_controls.cpp"
    "src/machine/user_interface/window_debugger.cpp"
)
source_group("Source Files\\Machine\\UserInterface" FILES ${Source_Files__Machine__UserInterface})

set(Source_Files__Machine__UserInterface__Units
    "src/machine/user_interface/asm_list.cpp"
    "src/machine/user_interface/button.cpp"
    "src/machine/user_interface/checkbox.cpp"
    "src/machine/user_interface/element.cpp"
    "src/machine/user_interface/list.cpp"
    "src/machine/user_interface/text.cpp"
    "src/machine/user_interface/textbox.cpp"
    "src/machine/user_interface/window.cpp"
)
source_group("Source Files\\Machine\\UserInterface\\Units" FILES ${Source_Files__Machine__UserInterface__Units})

set(Source_Files__Timing
    "src/timing/frame.cpp"
    "src/timing/timer.cpp"
)
source_group("Source Files\\Timing" FILES ${Source_Files__Timing})

set(Source_Files__Utility
    "src/utility/utility.cpp"
)
source_group("Source Files\\Utility" FILES ${Source_Files__Utility})

set(ALL_FILES
    ${Header_Files__Logger}
    ${Header_Files__Machine}
    ${Header_Files__Machine__APU}
    ${Header_Files__Machine__Bus}
    ${Header_Files__Machine__CPU}
    ${Header_Files__Machine__Input}
    ${Header_Files__Machine__Mapper}
    ${Header_Files__Machine__Misc}
    ${Header_Files__Machine__PPU}
    ${Header_Files__Machine__PPU__Display}
    ${Header_Files__Machine__PPU__Registers}
    ${Header_Files__Machine__UserInterface}
    ${Header_Files__Machine__UserInterface__Units}
    ${Header_Files__Timing}
    ${Header_Files__Utility}
    ${Source_Files}
    ${Source_Files__Logger}
    ${Source_Files__Machine}
    ${Source_Files__Machine__APU}
    ${Source_Files__Machine__Bus}
    ${Source_Files__Machine__CPU}
    ${Source_Files__Machine__Input}
    ${Source_Files__Machine__Mapper}
    ${Source_Files__Machine__Misc}
    ${Source_Files__Machine__PPU}
    ${Source_Files__Machine__PPU__Display}
    ${Source_Files__Machine__PPU__Registers}
    ${Source_Files__Machine__UserInterface}
    ${Source_Files__Machine__UserInterface__Units}
    ${Source_Files__Timing}
    ${Source_Files__Utility}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE NESEmulator)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "_CRT_SECURE_NO_WARNINGS"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "SDL2;"
        "SDL2main;"
        "SDL2_ttf;"
        "SDL2_image"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

