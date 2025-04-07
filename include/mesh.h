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

#ifndef MESH_H
#define MESH_H

/////////////////////////////////////////////////////////////////////////
//	Includes
//
#include <types.h>
#include <defines.h>

/////////////////////////////////////////////////////////////////////////
//	Types
//
/**
 * @brief Mesh_ is a look up enumeration
 */
enum Mesh_
{
	Mesh_CrocIdleRight,
	Mesh_CrocIdleLeft,
	Mesh_CrocArm,
	Mesh_CrocArmForward,
	Mesh_CrocTail,
	Mesh_DocIdleRight,
	Mesh_DocIdleLeft,
	Mesh_MantisRight,
	Mesh_MantisLeft,
	Mesh_BarrelLeft,
	Mesh_Barrel,
	Mesh_BarellRight,
	Mesh_CrateLeft,
	Mesh_Crate,
	Mesh_CrateRight,
	Mesh_Jumper,
};
typedef i8 Mesh;

/////////////////////////////////////////////////////////////////////////
//	Getter Functions
//
i8 CONST* mesh_get(Mesh const mesh);
void      mesh_load(Mesh const mesh, bool const mirror, i8 __out* destination);

#endif /* MESH_H */