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
#include <cnd/mesh.h>

/////////////////////////////////////////////////////////////////////////
//	Global Data
//
const i8 croc_idle_right[] = 
{
	LENGTH(17),
	-2,  0,
	 0,  5,
	 2, -2,
	 2,  2,
	 3,  0,
	 2, -1,
	 2,  6,
	 1,  0,
	 0, -5,
	 2,  0,
	 0, -3,
	-2,  0,
	-9, -3,
	-1, -3,
	 2, -3,
	-5,  3,
	 0,  4,
};

const i8 croc_idle_left[] = 
{
	LENGTH(17),
	-2,  0,
	 0, -5,
	 2,  2,
	 2, -2,
	 3,  0,
	 2,  1,
	 2, -6,
	 1,  0,
	 0,  5,
	 2,  0,
	 0,  3,
	-2,  0,
	-9,  3,
	-1,  3,
	 2,  3,
	-5, -3,
	 0, -4,
};

const i8 croc_arm[] = 
{
	LENGTH(6),
	-3, 0,
	1, 1,
	-1, 1,
	1, 1,
	-1, 1,
	3, 0,
};

const i8 croc_arm_forward[] = 
{
	LENGTH(6),
	5, 10,
	1, -3,
	 1, 3,
	 1,-3,
	 1, 3,
	 -5, -10,
};

const i8 doc_body_right[] = 
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

const i8 doc_body_left[] = 
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

const i8 barrel[] = 
{
	LENGTH(4),
	20, 0,
	0, 15,
	-20, 0,
	0, -15,
};

const i8 barrel_right[] = 
{
	LENGTH(4),
	20, -10,
	5, 15,
	-20, 10,
	-5, -15,
};

const i8 barrel_left[] = 
{
	LENGTH(4),
	20, 10,
	5, -15,
	-20, -10,
	-5, 15,
};

const i8 crate[] = 
{
	LENGTH(5),
	10, 0,
	0, 10,
	-10, 0,
	0, -10,
	10, 10,
};

const i8 crate_right[] = 
{
	LENGTH(5),
	7, 7,
	-7, 7,
	-7, -7,
	7, -7,
	0, 14,
};

const i8 mantis_right[] = 
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

const i8 mantis_left[] = 
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

const i8 jumper[] = 
{
	LENGTH(5),
	0, -10,
	5, 10,
	0, -10,
	5, 10,
	0, -10,
};

const i8 ground [] = 
{
	LENGTH(0),
	0, 64,
};

const i8 wall [] = 
{
	LENGTH(0),
	-64, 0,
};

const i8 ceiling [] = 
{
	LENGTH(0),
	0, 64,
};

const i8 platform [] = 
{
	LENGTH(0),
	0, 32,
};

const i8 breakable [] = 
{
	LENGTH(2),
	0, 32,
	-5, -32,
	0, 32,
};

const i8* const g_meshes[] = 
{
	ground,
    wall,
    ceiling,
    platform,
    breakable,
	croc_idle_right,
	croc_idle_left,
	croc_arm,
	croc_arm_forward,
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
    return (i8 CONST*) g_meshes[mesh];
}

void mesh_load(Mesh const mesh, bool const mirror, i8 __out* destination)
{
    const i8* const m = mesh_get(mesh);
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
