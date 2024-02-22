#ifndef __UTILS_H__
#define __UTILS_H__

#include"defs.h"
#include"image_utils.h"
#include<stdlib.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<string.h>
#include<stdbool.h>

//    rad / pi = deg / 180
//    J = Juliaset only
//    JD = Juliaset Dance

inline double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}

static inline double to_radian(double degree) {
    return degree * (M_PI/180.0);
}

void ProcessArgs(int argc, char** argv, Config* outcfg, ImageState* state)
{
    FILE* fp = fopen(argv[1], "r");
    fscanf(fp, "%d%*c%d ", &(outcfg->width), &(outcfg->height));
    state->height=outcfg->height;
    state->width=outcfg->width;
    printf("config file image resolution: %d * %d", outcfg->width, outcfg->height);
    fscanf(fp, "%lf%*c%lf%*c%lf%*c%lf", 
        &(state->minx),
        &(state->maxx),
        &(state->miny),
        &(state->maxy)
    );
    state->cx = (state->maxx + state->minx) / 2;
    state->cy = (state->maxy + state->miny) / 2;

    Cmd Commands[1000];
    int cmdno = 0;
    while(true)
    {
        char cmdname[10];
        int ret = fscanf(fp, "%s ", cmdname);
        if (ret <= 0)
            break;

        if (strcmp(cmdname, "zoom") == 0)
        {
            Commands[cmdno].cmd = ct_Zoom;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "zoomout") == 0)
        {
            Commands[cmdno].cmd = ct_Zoomout;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "zoomrotate") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomRotate;
            fscanf(fp, "%lf%*c%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].angle), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "rotate")==0)
        {
            Commands[cmdno].cmd = ct_Rotate;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].angle), &(Commands[cmdno].steps));
            Commands[cmdno].angle = to_radian(Commands[cmdno].angle);
        }
        else if (strcmp(cmdname, "center")==0)
        {
            Commands[cmdno].cmd = ct_Center;
            fscanf(fp, "%lf%*c%lf%*c%d", &(Commands[cmdno].cx),&(Commands[cmdno].cy),&(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "hold")==0)
        {
            Commands[cmdno].cmd= ct_Hold;
            fscanf(fp, "%d", &(Commands[cmdno].steps));
        }

        else if (strcmp(cmdname, "zoomJ") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomJ;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "zoomoutJ") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomoutJ;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "zoomrotateJ") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomRotateJ;
            fscanf(fp, "%lf%*c%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].angle), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "rotateJ")==0)
        {
            Commands[cmdno].cmd = ct_RotateJ;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].angle), &(Commands[cmdno].steps));
            Commands[cmdno].angle = to_radian(Commands[cmdno].angle);
        }
        else if (strcmp(cmdname, "centerJ")==0)
        {
            Commands[cmdno].cmd = ct_CenterJ;
            fscanf(fp, "%lf%*c%lf%*c%d", &(Commands[cmdno].cx),&(Commands[cmdno].cy),&(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "holdJ")==0)
        {
            Commands[cmdno].cmd= ct_HoldJ;
            fscanf(fp, "%d", &(Commands[cmdno].steps));
        }

        else if (strcmp(cmdname, "zoomJD") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomJD;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "zoomoutJD") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomoutJD;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "zoomrotateJD") == 0)
        {
            Commands[cmdno].cmd = ct_ZoomRotateJD;
            fscanf(fp, "%lf%*c%lf%*c%d", &(Commands[cmdno].zoom), &(Commands[cmdno].angle), &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "rotateJD")==0)
        {
            Commands[cmdno].cmd = ct_RotateJD;
            fscanf(fp, "%lf%*c%d", &(Commands[cmdno].angle), &(Commands[cmdno].steps));
            Commands[cmdno].angle = to_radian(Commands[cmdno].angle);
        }
        else if (strcmp(cmdname, "centerJD")==0)
        {
            Commands[cmdno].cmd = ct_CenterJD;
            fscanf(fp, "%lf%*c%lf%*c%d", &(Commands[cmdno].cx),&(Commands[cmdno].cy),&(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "holdJD")==0)
        {
            Commands[cmdno].cmd= ct_HoldJD;
            fscanf(fp, "%d", &(Commands[cmdno].steps));
        }

        else if (strcmp(cmdname, "dynamic_color")==0)
        {
            Commands[cmdno].cmd = ct_dynamic_color;
            fscanf(fp, "%d",&(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "dynamic_color_two")==0)
        {
            Commands[cmdno].cmd = ct_dynamic_color_two;
            fscanf(fp, "%d",&(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "dynamic_color_J")==0)
        {
            Commands[cmdno].cmd = ct_dynamic_color_J;
            fscanf(fp, "%d",&(Commands[cmdno].steps));
        }  

        else if (strcmp(cmdname, "hold_two")==0)
        {
            Commands[cmdno].cmd= ct_Hold_two;
            fscanf(fp, "%d", &(Commands[cmdno].steps));
        }
        else if (strcmp(cmdname, "fade")==0)
        {
            Commands[cmdno].cmd= ct_fade;
            fscanf(fp, "%d", &(Commands[cmdno].steps));
        } 
        else if (strcmp(cmdname, "zoomout_fade")==0)
        {
            Commands[cmdno].cmd= ct_Zoomout_fade;
            fscanf(fp, "%lf%*c%d",&(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        } 
        else if (strcmp(cmdname, "fadeJ")==0)
        {
            Commands[cmdno].cmd= ct_fadeJ;
            fscanf(fp, "%d", &(Commands[cmdno].steps));
        } 
        else if (strcmp(cmdname, "zoom_black")==0)
        {
            Commands[cmdno].cmd= ct_zoom_black;
            fscanf(fp, "%lf%*c%d",&(Commands[cmdno].zoom), &(Commands[cmdno].steps));
        }
        cmdno++;
    }

    outcfg->CmdCount = cmdno;
    outcfg->Commands = (Cmd*) malloc(sizeof(Cmd) * cmdno);
    for(int i=0; i<cmdno; i++)
    {
        outcfg->Commands[i] = Commands[i];
    }
    InitImage(& state->bmFileData, state->height, state->width, 256);
}

#endif