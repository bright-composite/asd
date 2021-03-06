#--------------------------------------------------------
#	asd
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.3)

project(asd VERSION 0.1)

#--------------------------------------------------------

include(tools/workspace.cmake)
include(tools/module.cmake)

add_module(benchmark)
add_module(window)
add_module(graphics3d)
add_module(image)
add_module(text)
add_module(input)
add_module(widget)
add_module(scene)
add_module(ui-themes)

if(WIN32)
    add_command(check)
    add_command(clear_shaders)
endif()

add_test(core)
add_test(math)
add_test(gfx)
add_test(ui)
add_test(handle)
add_test(aligned)
add_test(space)
add_test(glx)
add_test(functions)
add_test(maps)

add_test(batch_gfx)
add_test(immediate_gfx)
add_test(window)
add_test(sdl)
add_test(flow)
