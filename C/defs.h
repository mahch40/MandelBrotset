#ifndef __DEFS_H__
#define __DEFS_H__
#include"image_utils.h"

typedef enum 
{
    ct_Center = 1,
    ct_Zoom = 2,
    ct_Rotate = 3,
    ct_Hold = 4,
    ct_CenterJ = 5,
    ct_ZoomJ = 6,
    ct_RotateJ = 7,
    ct_HoldJ = 8,
    ct_CenterJD = 9,
    ct_ZoomJD = 10,
    ct_RotateJD = 11,
    ct_HoldJD = 12,
    ct_dynamic_color = 13,
    ct_dynamic_color_J = 14,
    ct_Zoomout = 15,
    ct_ZoomoutJ = 16,
    ct_ZoomoutJD = 17,
    ct_ZoomRotate = 18,
    ct_ZoomRotateJ = 19,
    ct_ZoomRotateJD = 20,
    ct_dynamic_color_two = 21,
    ct_Hold_two = 22, 
    ct_fade = 23,
    ct_Zoomout_fade = 24,
    ct_fadeJ = 25,
    ct_zoom_black = 26
} cmd_type;

typedef struct _cmd
{
    cmd_type cmd;
    int steps;
    double cx,cy;
    double zoom;
    double angle;
} Cmd;

typedef struct _config
{
    int height;
    int width;
    Cmd* Commands;
    int CmdCount;

} Config;

typedef struct _image_state
{
    double cx, cy;
    double minx, maxx, miny, maxy;
    double angle;
    int height, width;
    int image_count;
    BitMapFile bmFileData;
} ImageState;

typedef struct tagCircle
{
    double R;
    //center positions: x,y
    double x;
    double y;
    // circle : cx , cy
    double cx;
    double cy;
} Circle;

#endif