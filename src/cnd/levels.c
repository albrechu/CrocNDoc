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
#include <cnd/levels.h>

/////////////////////////////////////////////////////////////////////////
//	Global Data
//
const Tile sewers[1024] =
{
    Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_E4, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_SpikedBall, Tile_SpikedBall, Tile_SpikedBall, Tile_SpikedBall, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_Portal, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_BarrierVertical, Tile_Bottom2, Tile_NA, Tile_NA, Tile_Left, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_Left, Tile_Top, Tile_TopRight, Tile_MiddleRight, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_MiddleLeft, Tile_MiddleRight, Tile_NA, Tile_MiddleLeft, Tile_MiddleRight, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_BottomLeft, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Coin, Tile_NA, Tile_Right, Tile_Top2, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_Top2, Tile_Top2, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Bottom, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_Top, Tile_Left, Tile_NA, Tile_MiddleBottom, Tile_MiddleRight, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_E7, Tile_MiddleRight, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_Right, Tile_E1, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_SpikedBall, Tile_Top, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_NA, Tile_Jumper, Tile_NA, Tile_Right, Tile_Top, Tile_Right2, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_MiddleTop, Tile_Air, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E3, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_E6, Tile_NA, Tile_MiddleRightTop, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_Bottom2, Tile_MiddleRightTop, Tile_Air, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Jumper, Tile_NA, Tile_Right, Tile_E2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Left, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Right, Tile_MiddleBottom, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Spikes, Tile_Spikes, Tile_NA, Tile_Right, Tile_Middle, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_BarrierHorizontal, Tile_TopLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_Left2, Tile_MiddleLeft, Tile_Spikes, Tile_Spikes, Tile_Top, Tile_Right, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_SpikedBall, Tile_NA, Tile_Jumper, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E12, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E8, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_Top, Tile_Spikes, Tile_Top, Tile_Middle, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E14, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_Jumper, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_Right2, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_SpikedBall, Tile_Middle, Tile_NA, Tile_NA, Tile_Top, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_E9, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_Jumper, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_E13, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_E15, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_TopRight, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_Jumper, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E10, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E11, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_NA,
    Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Air,
};

static const EntityType sewersEntities[] =
{
    Enemy_Schuft,
    Enemy_Bandit,
    Prop_Barrel,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Gauner,
    Enemy_Gauner,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Gauner,
};

const Tile tutorial[1024] =
{
    Tile_Right, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_MiddleRightTop, Tile_NA, Tile_Left, Tile_Left2, Tile_NA, Tile_MiddleRightTop, Tile_NA, Tile_Left, Tile_Top, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E0, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_Bottom2, Tile_NA, Tile_Left, Tile_Top, Tile_TopRight, Tile_Spikes, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_Spikes, Tile_Left, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E1, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Left2, Tile_Left,
    Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Left2, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Bottom2, Tile_Bottom2, Tile_NA, Tile_NA, Tile_Bottom2, Tile_E3, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E2, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_NA, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_Right, Tile_MiddleLeft, Tile_NA, Tile_Left, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_Bottom2, Tile_MiddleRight, Tile_NA, Tile_MiddleRightTop, Tile_NA, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_TopLeft, Tile_Right, Tile_NA, Tile_Left, Tile_TopRight, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_Right, Tile_MiddleRight, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_Left, Tile_Right, Tile_Spikes, Tile_Left, Tile_Water, Tile_Top, Tile_Top, Tile_Top, Tile_Water,
    Tile_Right, Tile_NA, Tile_Left, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA,
    Tile_Right, Tile_NA, Tile_BottomLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_MiddleLeft, Tile_MiddleRight, Tile_Portal, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_TopRight, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_TopRight, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_MiddleLeft, Tile_MiddleLeft, Tile_MiddleLeft, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_TopRight, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_Middle, Tile_Left,
    Tile_Right, Tile_NA, Tile_BarrierVertical, Tile_NA, Tile_NA, Tile_NA, Tile_E7, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E5, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_TopRight, Tile_Right, Tile_NA, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_MiddleRight, Tile_MiddleRightTop, Tile_NA, Tile_Left,
    Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_TopRight, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_Middle, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_Bottom2, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_TopLeft, Tile_Right2, Tile_Jumper, Tile_Left2, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_E11, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E10, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Left, Tile_NA, Tile_Top, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Top, Tile_Top, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_NA, Tile_NA, Tile_Left, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_NA, Tile_E15, Tile_NA, Tile_Coin, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_NA, Tile_MiddleRight, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_NA, Tile_Left, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E9, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_Left, Tile_Left, Tile_NA, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_TopRight, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_Left, Tile_Left, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Left, Tile_NA, Tile_Bottom, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_NA, Tile_NA, Tile_Bottom, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_Right2, Tile_TopRight, Tile_WaterTop, Tile_WaterTop, Tile_Left, Tile_Top, Tile_NA,
    Tile_Right, Tile_Right2, Tile_Bottom, Tile_Bottom, Tile_Left2, Tile_NA, Tile_Left, Tile_Bottom, Tile_Bottom, Tile_Right, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_MiddleLeft, Tile_NA, Tile_Right2, Tile_Right, Tile_Water, Tile_Water, Tile_Left, Tile_NA, Tile_NA,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top2, Tile_E13, Tile_NA, Tile_Top2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E14, Tile_MiddleLeft, Tile_Right2, Tile_Right, Tile_Water, Tile_Water, Tile_Left, Tile_NA, Tile_NA,
    Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_Air,
};



static const EntityType tutorialEntities[] =
{
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Prop_Barrel,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Schuft,
};

const Tile water[1024] =
{
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA,
    Tile_Right, Tile_NA, Tile_NA, Tile_Air, Tile_Air, Tile_Left, Tile_Left2, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_WaterTop, Tile_WaterTop, Tile_Left, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA,
    Tile_Right, Tile_Top2, Tile_Top2, Tile_WaterTop, Tile_WaterTop, Tile_Left, Tile_Left2, Tile_Jumper, Tile_Spikes, Tile_Jumper, Tile_Spikes, Tile_Jumper, Tile_Spikes, Tile_TopLeft, Tile_Top, Tile_Top, Tile_Right, Tile_Water, Tile_Water, Tile_Top2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_Left,
    Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left, Tile_Left2, Tile_MiddleBottom, Tile_Left, Tile_Top, Tile_Bottom, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Spikes, Tile_NA, Tile_NA, Tile_Left, Tile_Left2, Tile_MiddleBottom, Tile_Left, Tile_NA, Tile_Left2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Left, Tile_Right, Tile_Top2, Tile_Top2, Tile_Top2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_Left, Tile_Left2, Tile_Hole1, Tile_Left, Tile_Right, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E6, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_MiddleTop, Tile_Air, Tile_Left, Tile_Right, Tile_NA, Tile_Right2, Tile_Top, Tile_Top, Tile_Left2, Tile_NA, Tile_Left, Tile_Right, Tile_E4, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikesDown, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_NA, Tile_Spikes, Tile_Left, Tile_NA, Tile_Left2, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_Spikes, Tile_NA, Tile_Left, Tile_NA, Tile_Left2, Tile_NA, Tile_Left, Tile_Right, Tile_Hole2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_SpikedBall, Tile_SpikedBall, Tile_NA, Tile_SpikedBall, Tile_SpikedBall, Tile_Left,
    Tile_Right, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_Spikes, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Left2, Tile_E0, Tile_Left, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left2, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_Spikes, Tile_NA, Tile_NA, Tile_Hole0, Tile_Left, Tile_NA, Tile_Left2, Tile_Top, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left2, Tile_NA, Tile_NA, Tile_Spikes, Tile_NA, Tile_NA, Tile_MiddleRight, Tile_Bottom2, Tile_Left, Tile_NA, Tile_Left2, Tile_NA, Tile_Left, Tile_Right, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left2, Tile_NA, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Left, Tile_NA, Tile_Bottom, Tile_Left2, Tile_NA, Tile_Left, Tile_Right, Tile_SpikedBall, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_Hole3, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Left, Tile_NA, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_Coin, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Left, Tile_NA, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_Spikes, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Left,
    Tile_Right, Tile_NA, Tile_SpikedBall, Tile_MiddleBottom, Tile_TopLeft, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Left2, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_NA, Tile_Right2, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Top, Tile_Right, Tile_Top2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA,
    Tile_Right, Tile_NA, Tile_SpikedBall, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E1, Tile_NA, Tile_Right2, Tile_Top, Tile_Right, Tile_Top2, Tile_Top2, Tile_NA, Tile_E7, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_Portal, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E8, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_MiddleBottom, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_Left, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Middle, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_NA, Tile_E9, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_NA, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_Jumper, Tile_Right2, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_Bottom2, Tile_Bottom2, Tile_NA, Tile_NA, Tile_NA, Tile_E5, Tile_NA, Tile_Top, Tile_Middle, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_WaterTop, Tile_WaterTop, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_GravitasUp, Tile_Left,
    Tile_Right, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Top2, Tile_Top2, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top2, Tile_Top2, Tile_Water, Tile_Water, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Middle, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Left,
    Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_NA, Tile_NA, Tile_NA, Tile_E3, Tile_Right2, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E10, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Water,
};


static const EntityType waterEntities[] =
{
    Prop_Barrel,
    Enemy_Halunke,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Prop_Barrel,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
};

const Tile terra[1024] =
{
    Tile_Right, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_GravitasDown, Tile_GravitasDown, Tile_GravitasDown, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Portal, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_SpikedBall,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikedBall,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikedBall,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_GravitasUp, Tile_Warning, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Coin, Tile_SpikedBall,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_Spikes, Tile_Top, Tile_Spikes, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA,
    Tile_Right, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleBottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_MiddleBottom, Tile_NA, Tile_SpikedBall, Tile_Jumper, Tile_NA, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA,
    Tile_Right, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Jumper, Tile_Left, Tile_Left2, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom2, Tile_NA, Tile_NA, Tile_Left, Tile_Left2, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_Top, Tile_Top, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_Right, Tile_E3, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_E2, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_MiddleTop, Tile_MiddleRight, Tile_NA, Tile_MiddleLeft, Tile_GravitasDown, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleTop, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_Left, Tile_NA, Tile_Right, Tile_Top, Tile_NA, Tile_NA, Tile_Air, Tile_Top2, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_SpikesDown, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Air, Tile_GravitasDown, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_Bottom, Tile_SpikesDown, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Air, Tile_Air, Tile_Air, Tile_NA, Tile_NA, Tile_Top2, Tile_Top2, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_NA, Tile_Right2, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_Left2, Tile_E4, Tile_Left, Tile_NA, Tile_Right, Tile_SpikesDown, Tile_SpikesDown, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_GravitasUp, Tile_NA, Tile_NA, Tile_Jumper, Tile_GravitasDown, Tile_NA, Tile_GravitasDown, Tile_NA, Tile_NA, Tile_Bottom, Tile_E5, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_GravitasUp, Tile_NA, Tile_Left, Tile_Left2, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_GravitasUp, Tile_MiddleBottom, Tile_NA, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_MiddleTop, Tile_NA, Tile_Left, Tile_Left2, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_SpikedBall, Tile_NA, Tile_E1, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_Jumper, Tile_NA, Tile_NA, Tile_Jumper, Tile_MiddleTop, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Spikes, Tile_Spikes, Tile_MiddleRightTop, Tile_MiddleLeft, Tile_MiddleBottom, Tile_Top2, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Right, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Middle, Tile_NA, Tile_NA, Tile_Left, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_Left,
    Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_GravitasDown, Tile_NA, Tile_SpikesDown, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_SpikesDown, Tile_NA, Tile_NA, Tile_Bottom, Tile_NA, Tile_SpikesDown, Tile_SpikesDown, Tile_SpikesDown, Tile_NA, Tile_NA, Tile_NA, Tile_Right2, Tile_Right, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Top2, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_Right, Tile_NA, Tile_NA, Tile_E0, Tile_NA, Tile_GravitasUp, Tile_Spikes, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_GravitasUp, Tile_NA, Tile_NA, Tile_SpikedBall, Tile_SpikedBall, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_NA, Tile_Left,
    Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_NA, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Air,
};







static const EntityType terraEntities[] =
{
    Prop_Barrel,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Enemy_Bandit,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
    Prop_Barrel,
};


/////////////////////////////////////////////////////////////////////////
//	Table with all the levels 
//
const level_t levels[] =
{
    // Stage_Tutorial
    {
        .entities      = tutorialEntities,
        .level         = tutorial,
        .startingTile  = {2, 1},
        .adjacentStage = Stage_JumpWorld,
    },
    // Stage_Sewers
    {
        .entities      = sewersEntities,
        .level         = sewers,
        .startingTile  = { 2, 1 },
        .adjacentStage = Stage_Water,
    },
    // Stage_Water
    {
        .entities      = waterEntities,
        .level         = water,
        .startingTile  = { 2, 1 },
        .adjacentStage = Stage_Gravitas,
    },
    // Stage_
    {
        .entities      = terraEntities,
        .level         = terra,
        .startingTile  = { 30, 2 },
        .adjacentStage = Stage_Tutorial,
    },
};