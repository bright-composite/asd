#--------------------------------------------------------
#	asd UI Themes
#--------------------------------------------------------

project(uithemes CXX)

#--------------------------------------------------------

include(${ASD_TOOLS}/module.cmake)

#--------------------------------------------------------

module(STATIC)
	dependencies(gui	0.*)

	sources(modules)
		domain(ui/themes)

		group(include Headers)
		files(
			asd.h
		)

		group(src Sources)
		files(
			asd.cpp
		)
	endsources()
endmodule()

#--------------------------------------------------------
