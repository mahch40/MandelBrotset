#include<stdio.h>
#include<stdlib.h>
#include"defs.h"
#include"utils.h"
#include"mandelbrotset.h"

int main(int argc, char** argv)
{
    Config cfg; memset(&cfg, 0, sizeof(Config));
    ImageState state; memset(&state, 0, sizeof(ImageState));
    ProcessArgs(argc, argv, &cfg, &state);

    Circle c; memset(&c,0,sizeof(Circle));
    c.R=state.width/4;
    c.cx=sqrt(c.R*c.R - (c.cy-c.y)*(c.cy-c.cy))+c.x;

    WriteBitmapFile(state.image_count++, & state.bmFileData);

    for(int i=0;i<cfg.CmdCount; i++)
    {
        switch (cfg.Commands[i].cmd)
        {
            case ct_Center:
                ChangeCenter(&state, cfg.Commands[i].cx, cfg.Commands[i].cy, cfg.Commands[i].steps);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_Zoom:
                ChangeZoom(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_Zoomout:
                Zoomout(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomRotate:
                Zoomrotate(&state, cfg.Commands[i].zoom, cfg.Commands[i].angle, cfg.Commands[i].steps);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_Rotate:
                ChangeRotation(&state, cfg.Commands[i].angle, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_Hold:
                Hold(&state, cfg.Commands[i].steps);
                break;

            case ct_CenterJ:
                ChangeCenterJ(&state, cfg.Commands[i].cx, cfg.Commands[i].cy, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomJ:
                ChangeZoomJ(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomoutJ:
                ZoomoutJ(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomRotateJ:
                ZoomrotateJ(&state,cfg.Commands[i].zoom,cfg.Commands[i].angle, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_RotateJ:
                ChangeRotationJ(&state, cfg.Commands[i].angle, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_HoldJ:
                HoldJ(&state, cfg.Commands[i].steps,&c);
                break;

            case ct_CenterJD:
                ChangeCenterJD(&state, cfg.Commands[i].cx, cfg.Commands[i].cy, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomJD:
                ChangeZoomJD(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomoutJD:
                ZoomoutJD(&state, cfg.Commands[i].zoom, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_ZoomRotateJD:
                ZoomrotateJD(&state,cfg.Commands[i].zoom,cfg.Commands[i].angle, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_RotateJD:
                ChangeRotationJD(&state, cfg.Commands[i].angle, cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_HoldJD:
                HoldJD(&state, cfg.Commands[i].steps,&c);
                break;

            case ct_dynamic_color:
                dynamic_color(&state,cfg.Commands[i].steps);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_dynamic_color_two:
                dynamic_color_two(&state,cfg.Commands[i].steps);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;
            case ct_dynamic_color_J:
                dynamic_color_J(&state,cfg.Commands[i].steps,&c);
                state.cx=(state.maxx + state.minx) / 2;
                state.cy=(state.maxy + state.miny) / 2;
                break;

            case ct_Hold_two:
                Hold_two(&state, cfg.Commands[i].steps);
                break;

            case ct_fade:
                fade(&state, cfg.Commands[i].steps);
                break;
            case ct_Zoomout_fade:
                Zoomout_fade(&state, cfg.Commands[i].zoom,cfg.Commands[i].steps);
                break;
            case ct_fadeJ:
                fadeJ(&state, &c, cfg.Commands[i].steps);
                break;

            case ct_zoom_black:
                zoom_black(&state,cfg.Commands[i].zoom, cfg.Commands[i].steps);
                break;
        }   
    }
    return 0;
}
