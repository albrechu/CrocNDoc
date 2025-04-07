/*
 * MIT License
 * 
 * Copyright (c) 2025 Julian Albrecht
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <mesh.h>

/////////////////////////////////////////////////////////////////////////
//	Global Data
//
CONST i8 croc_idle_right[] = 
{
	LENGTH(17),
	-3, 0,
	0, 7,
	3, -3,
	3,  3,
	5, 0,
	3, -2,
	3,  9,
	2,  0,
	1, -7,
	3,  -1,
	0,  -4,
	-3, -1,
	-12, -5,
	-2, -4,
	3, -4,
	-7, 5,
	1, 6,
};

CONST i8 croc_idle_left[] = 
{
	LENGTH(17),
	-3, 0,
	0, -7,
	3, 3,
	3, -3,
	5,-0,
	3,  2,
	3, -9,
	2,  0,
	1,  7,
	3,   1,
	0,   4,
	-3,  1,
	-12,  5,
	-2, 4,
	3, 4,
	-7, -5,
	1, -6,
};

CONST i8 croc_arm[] = 
{
	LENGTH(6),
	-10, 0,
	3, 1,
	-3, 1,
	3, 1,
	-3, 1,
	10, 0,
};

CONST i8 croc_arm_forward[] = 
{
	LENGTH(6),
	5, 10,
	1, -3,
	 1, 3,
	 1,-3,
	 1, 3,
	 -5, -10,
};

CONST i8 croc_tail[] = 
{

};

CONST i8 doc_body_right[] = 
{
	LENGTH(12),
	1, -3,
	7, 3,
	-2, 2,
	7, 2,
	-1, 1,
	-2, 3,
	1, -4,
	-5, 1,
	-2, 2,
	-4, -7,
	-5, 2,
	0, 3,
};

CONST i8 doc_body_left[] = 
{
	LENGTH(12),
	1, 3,
	7, -3,
	-2, -2,
	7, -2,
	-1, -1,
	-2, -3,
	1, 4,
	-5, -1,
	-2, -2,
	-4, 7,
	-5, -2,
	0, -3,
};

CONST i8 barrel[] = 
{
	LENGTH(4),
	20, 0,
	0, 15,
	-20, 0,
	0, -15,
};

CONST i8 barrel_right[] = 
{
	LENGTH(4),
	20, -10,
	5, 15,
	-20, 10,
	-5, -15,
};

CONST i8 barrel_left[] = 
{
	LENGTH(4),
	20, 10,
	5, -15,
	-20, -10,
	-5, 15,
};

CONST i8 crate[] = 
{
	LENGTH(5),
	10, 0,
	0, 10,
	-10, 0,
	0, -10,
	10, 10,
};

CONST i8 crate_right[] = 
{
	LENGTH(5),
	7, 7,
	-7, 7,
	-7, -7,
	7, -7,
	0, 14,
};

CONST i8 mantis_right[] = 
{
	LENGTH(9),
    -8,   1, // Body
    -6,  -8, // Body
    -10,  1, // Lower Body
    -4, -20, // Lower Body
    14,  19, // Upper Line - Lower Body
    14,   8, // Upper Line - Body
     3,   6, // Upper Line - Head
    -5,   4, // Head
     2, -10, // Head
};

CONST i8 mantis_left[] = 
{
	LENGTH(9),
    -8,  -1, // Body
    -6,   8, // Body
    -10, -1, // Lower Body
    -4,  20, // Lower Body
    14, -19, // Upper Line - Lower Body
    14,  -8, // Upper Line - Body
     3,  -6, // Upper Line - Head
    -5,  -4, // Head
     2,  10, // Head
};

CONST i8 jumper[] = 
{
	LENGTH(5),
	0, -10,
	5, 10,
	0, -10,
	5, 10,
	0, -10,
};

CONST i8* g_meshes[] = 
{
	croc_idle_right,
	croc_idle_left,
	croc_arm,
	croc_arm_forward,
	croc_tail,
	doc_body_right,
	doc_body_left,
	mantis_right,
	mantis_left,
	barrel_left,
	barrel,
	barrel_right,
	crate_right,
	crate,
	crate_right,
	jumper,
};

/////////////////////////////////////////////////////////////////////////
//	Functions
//
i8 CONST* mesh_get(Mesh const mesh)
{
    return g_meshes[mesh];
}

void mesh_load(Mesh const mesh, bool const mirror, i8 __out* destination)
{
    i8* const m = mesh_get(mesh);
	const i8 length = *m;
	*destination = length;
	i8 v = 0, i = 1;
	if (mirror)
	{
		for (; v <= length; ++v)
		{
			destination[i] = m[i]; ++i;
			destination[i] = -m[i]; ++i;
		}
	}
	else
	{
		for (; v <= length; ++v)
		{
			destination[i] = m[i]; ++i;
			destination[i] = m[i]; ++i;
		}
	}
}
