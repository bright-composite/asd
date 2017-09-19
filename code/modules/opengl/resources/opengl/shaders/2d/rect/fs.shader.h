//---------------------------------------------------------------------------

#include <opengl/vertex_layout.h>

static constexpr char const shader_code_2d_rect_fs[] = R"SHADER(
/**
 *	
 */
#version 330 core

layout(std140) uniform Color
{
	vec4 color;
};

out vec4 fscolor;

void main(void)
{
	fscolor = color;
}

)SHADER";

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
