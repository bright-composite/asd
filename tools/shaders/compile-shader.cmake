#--------------------------------------------------------
#	ASD cmake shader compiler wrapper
#--------------------------------------------------------

cmake_minimum_required(VERSION 3.0)

#--------------------------------------------------------

	get_filename_component(OutputDir ${Output} DIRECTORY)
	get_filename_component(OutputName ${Output} NAME_WE)

	set(TimestampFile ${OutputDir}/${OutputName}.timestamp)
	file(TIMESTAMP ${Input} Timestamp)

	if(NOT EXISTS ${Output})
		if(NOT EXISTS ${OutputDir})
			file(MAKE_DIRECTORY ${OutputDir})
		endif()

		file(WRITE ${Output})
	elseif(EXISTS ${TimestampFile})
		file(READ ${TimestampFile} SavedTimestamp)

		if(${SavedTimestamp} STREQUAL ${Timestamp})
			return()
		endif()
	endif()

	set(VARIABLE ${OutputVariable}_${ShaderType})
	set(TEMP ${OutputDir}/${OutputName}.tmp)

	if("${ShaderLang}" STREQUAL "HLSL")
		file(WRITE ${TEMP})
		execute_process(COMMAND fxc /nologo "/T${ShaderType}_4_0" /Zpr /Fh "${TEMP}" /Vn "${VARIABLE}" "${Input}")
		file(READ ${TEMP} CONTENTS)
		file(REMOVE ${TEMP})
		string(REPLACE "const" "constexpr" CONTENTS ${CONTENTS})
	elseif("${ShaderLang}" STREQUAL "GLSL")
		file(READ ${Input} CONTENTS)
		configure_file("${ASD_TOOLS}/templates/internal/glsl.inc" ${TEMP})
		message("embedded glsl shader save succeeded; see ${Output}")
		file(READ ${TEMP} CONTENTS)
		file(REMOVE ${TEMP})
	endif()

	if("${ShaderType}" STREQUAL "vs")
		file(READ ${Input} ShaderCode)
		string(REGEX MATCH "!vertex:[ a-z2-4-]*" layout ${ShaderCode})

		if(NOT "${layout}" STREQUAL "")
			string(REGEX REPLACE "!vertex:[ ]*" "" layout ${layout})
			string(REGEX REPLACE "\\s\\s+" " " layout ${layout})
			set(CONTENTS "${CONTENTS}\n\nstatic constexpr ::asd::opengl::vertex_layout ${OutputVariable}_layout = \"${layout}\";\n")
		else()
			set(CONTENTS "${CONTENTS}\n\n#error \"Shader must declare vertex layout! Example: /* !vertex: p3 c4 */\"\n")
		endif()
	endif()

	file(WRITE ${Output} "${CONTENTS}")
	file(WRITE ${TimestampFile} "${Timestamp}")

#--------------------------------------------------------
