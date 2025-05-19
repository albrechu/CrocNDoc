#include <cnd/levels.h>

const Tile sewers[1024] =
{
    Tile_Air, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Air, Tile_Air, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Air,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_SpikedBall, Tile_SpikedBall, Tile_SpikedBall, Tile_SpikedBall, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Portal1, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_BarrierVertical, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Left, Tile_Right2, Tile_Right2, Tile_E4, Tile_Air, Tile_Left, Tile_Top, Tile_TopRight, Tile_MiddleLeft, Tile_MiddleRight, Tile_MiddleLeft, Tile_MiddleRight, Tile_Air, Tile_MiddleLeft, Tile_MiddleRight, Tile_Air, Tile_MiddleLeft, Tile_MiddleRight, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_MiddleLeftTop, Tile_Air, Tile_Left, Tile_Air, Tile_BottomLeft, Tile_Top, Tile_Top, Tile_Air, Tile_Air, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Top, Tile_Top, Tile_TopRight, Tile_Right, Tile_Top2, Tile_MiddleRight, Tile_Left, Tile_Air, Tile_Air, Tile_Top2, Tile_BottomLeft, Tile_Air, Tile_Air, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Bottom, Tile_Portal0, Tile_Right, Tile_Right, Tile_MiddleLeft, Tile_Top, Tile_Left, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Middle, Tile_E7, Tile_MiddleRight, Tile_Air, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_SpikedBall, Tile_Jumper, Tile_Right, Tile_Right, Tile_E1, Tile_MiddleLeftTop, Tile_Left, Tile_Air, Tile_BottomLeft, Tile_TopRight, Tile_Air, Tile_BottomLeft, Tile_Air, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_SpikedBall, Tile_Top, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right, Tile_Right, Tile_Top, Tile_Right2, Tile_Jumper, Tile_Jumper, Tile_Air, Tile_Top, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_BottomRight, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_Air, Tile_Right, Tile_Right, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_E3, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_MiddleTop, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E6, Tile_Air, Tile_MiddleRightTop, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_Bottom2, Tile_MiddleRightTop, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Jumper, Tile_Right, Tile_Right, Tile_E2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_TopLeft, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Air, Tile_Right, Tile_MiddleBottom, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Spikes, Tile_Spikes, Tile_Right, Tile_Right, Tile_MiddleBottom, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_BarrierHorizontal, Tile_TopLeft, Tile_Air, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Air, Tile_Right, Tile_Bottom, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Jumper, Tile_Left, Tile_Left2, Tile_Air, Tile_Air, Tile_Right2, Tile_Top, Tile_Right, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Air, Tile_Right, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_SpikedBall, Tile_Air, Tile_SpikedBall, Tile_BottomLeft, Tile_Top, Tile_Top, Tile_Left2, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E12, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Right, Tile_Air, Tile_Air, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Spikes, Tile_Spikes, Tile_Air, Tile_Right, Tile_Air, Tile_BottomLeft, Tile_BottomRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_TopRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E8, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Air, Tile_Air, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_TopRight, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E14, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Jumper, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Top, Tile_TopRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Air, Tile_Air, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Bottom2, Tile_BottomRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_E9, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Jumper, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_SpikedBall, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Right2, Tile_Bottom, Tile_Air, Tile_Air, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_E13, Tile_Air, Tile_SpikedBall, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_E15, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Top, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Right2, Tile_TopRight, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_BottomRight, Tile_Jumper, Tile_Jumper, Tile_Jumper, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E10, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E11, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Jumper, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Air, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Air,
    Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air,
};


static const EntityType sewersEntities[] =
{
    Enemy_Schuft,
    Enemy_Schuft,
    Prop_Barrel,
    Enemy_Schuft,
    Enemy_Schuft,
    Enemy_Gauner,
    Enemy_Gauner,
    Enemy_Gauner,
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
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_MiddleRightTop, Tile_Air, Tile_Left, Tile_Left2, Tile_Air, Tile_MiddleRightTop, Tile_Air, Tile_Left, Tile_Top, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E0, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_Bottom2, Tile_Air, Tile_Left, Tile_Top, Tile_TopRight, Tile_Spikes, Tile_Air, Tile_Left, Tile_Air, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Top, Tile_Air, Tile_Top, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Air, Tile_Bottom, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_Spikes, Tile_Left, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Top, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E1, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Left2, Tile_Left,
    Tile_Air, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Left2, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Bottom2, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Bottom2, Tile_E3, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E2, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Right, Tile_MiddleLeft, Tile_Air, Tile_Left, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_Bottom2, Tile_MiddleRight, Tile_Air, Tile_MiddleRightTop, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_Air, Tile_TopLeft, Tile_Right, Tile_Air, Tile_Left, Tile_TopRight, Tile_WaterTop, Tile_WaterTop, Tile_WaterTop, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Right, Tile_MiddleRight, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Spikes, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Middle, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Left, Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Spikes, Tile_Left, Tile_Right, Tile_Spikes, Tile_Left, Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Water,
    Tile_Right, Tile_Air, Tile_Left, Tile_Air, Tile_Spikes, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Spikes, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Air,
    Tile_Right, Tile_Air, Tile_BottomLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_MiddleLeft, Tile_MiddleRight, Tile_Portal0, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_TopRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_TopRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_MiddleLeft, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Right2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_TopRight, Tile_Air, Tile_Air, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_BottomRight, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_MiddleLeftTop, Tile_Left,
    Tile_Right, Tile_Air, Tile_BarrierVertical, Tile_Air, Tile_Air, Tile_Air, Tile_E7, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E5, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_TopRight, Tile_Right, Tile_Air, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_Bottom2, Tile_MiddleRight, Tile_MiddleRightTop, Tile_E12, Tile_Left,
    Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_TopRight, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_Top2, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right2, Tile_Right, Tile_Bottom2, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E10, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_TopLeft, Tile_Right2, Tile_Jumper, Tile_Left2, Tile_MiddleRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_E11, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Left, Tile_Air, Tile_Top, Tile_MiddleRight, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_TopLeft, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Right, Tile_Top, Tile_Top, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_Air, Tile_Air, Tile_Left, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_E15, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_Air, Tile_MiddleRight, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_BottomLeft, Tile_Right, Tile_Spikes, Tile_Air, Tile_Left, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Right, Tile_MiddleRight, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E9, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Right, Tile_Air, Tile_Left, Tile_Left, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_TopRight, Tile_Air, Tile_Air, Tile_Air, Tile_Top, Tile_Top, Tile_Air, Tile_Left, Tile_Left, Tile_Air, Tile_MiddleLeft, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Left, Tile_Air, Tile_Bottom, Tile_Bottom2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Bottom, Tile_Air, Tile_Air, Tile_Bottom, Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Air, Tile_Left, Tile_Air, Tile_MiddleRight, Tile_Right2, Tile_TopRight, Tile_WaterTop, Tile_WaterTop, Tile_TopLeft, Tile_Left, Tile_Left,
    Tile_Right, Tile_Right2, Tile_Bottom, Tile_Bottom, Tile_Left2, Tile_Air, Tile_Left, Tile_Bottom, Tile_Bottom, Tile_Right, Tile_Right2, Tile_Bottom, Tile_Bottom, Tile_Left2, Tile_Left, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Bottom, Tile_Left, Tile_MiddleLeft, Tile_Air, Tile_Right2, Tile_Right, Tile_Water, Tile_Water, Tile_Left, Tile_Top, Tile_Air,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top2, Tile_E13, Tile_Air, Tile_Top2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Top2, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_E14, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Right2, Tile_Right, Tile_Water, Tile_Water, Tile_Left, Tile_Air, Tile_Air,
    Tile_Air, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_Top, Tile_Spikes, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Air, Tile_Top, Tile_Top, Tile_Air, Tile_Air, Tile_Air,
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


static const Tile waterlevel[1024] =
{
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_MiddleRight, Tile_Air, Tile_MiddleBottom, Tile_SpikedBall, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Portal0, Tile_MiddleRight, Tile_SpikedBall, Tile_Middle, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left, Tile_Air, Tile_Air, Tile_MiddleLeft, Tile_Spikes, Tile_Spikes, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Air, Tile_Left,
    Tile_Right, Tile_Top2, Tile_Top2, Tile_WaterTop, Tile_WaterTop, Tile_BottomLeft, Tile_Air, Tile_Air, Tile_TopLeft, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left, Tile_WaterTop, Tile_WaterTop, Tile_Left, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Water, Tile_Spikes, Tile_Water, Tile_Water, Tile_BottomLeft, Tile_BarrierHorizontal, Tile_BottomRight, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Water, Tile_TopRight, Tile_Spikes, Tile_Spikes, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Water, Tile_Water, Tile_Top, Tile_Top, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_SpikedBall, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Bottom, Tile_SpikedBall, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_TopRight, Tile_Spikes, Tile_Spikes, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Portal1, Tile_Top, Tile_TopRight, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Jumper, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_SpikedBall, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Jumper, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Air, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_Air, Tile_Jumper, Tile_Right2, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Right, Tile_WaterTop, Tile_Top, Tile_Top, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Portal1, Tile_Water, Tile_Left,
    Tile_Right, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Water, Tile_Left,
    Tile_Water, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Top, Tile_Water,
};

static const EntityType waterwaysEntities[] =
{
    Prop_Barrel,
    Enemy_Tunichtgut,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
    Enemy_Halunke,
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
        .entities       = tutorialEntities,
        .level          = tutorial,
        .startingTile   = {2, 1},
        .adjacentStages = { Stage_JumpWorld, Stage_JumpWorld },
    },
    // Stage_Sewers
    {
        .entities       = sewersEntities,
        .level          = sewers,
        .startingTile   = { 2, 1 },
        .adjacentStages = { Stage_Water, Stage_Water },
    },
    // Stage_Waterways
    {
        .entities       = waterwaysEntities,
        .level          = waterlevel,
        .startingTile   = { 2, 1 },
        .adjacentStages = { Stage_Tutorial, Stage_Tutorial },
    },
};