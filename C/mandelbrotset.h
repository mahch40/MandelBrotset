#ifndef __MANDELBROTSET_H__
#define __MANDELBROTSET_H__

#include"defs.h"
#include<math.h>
#include<stdio.h>

#define MAX_ITER 500

typedef struct tagcomplex{
    double real;
    double imag;
}complex;

int my_abs(int x)
{
    if(x<0)
    {
        return -x;
    }
    return x;
}

double power(double z,int power)
{
    double n=1;
    for(int i=0;i<power;i++)
    {
        n*=z;
    }
    return n;
}

int get_mbs_iter(double x, double y)
{
    complex z={0,0};
    int iter=0;
    
    while((iter<MAX_ITER) &&  (((z.real*z.real)+(z.imag*z.imag))<=16))
    {
        double tempx=z.real;
        z.real=(tempx*tempx)-(z.imag*z.imag)+x;
        z.imag=((2)*(tempx)*(z.imag))+y;
        iter++;
    }
    if(iter==MAX_ITER)
    {
        return iter;
    }
    else
    {
        double log_zn = log(z.real * z.real + z.imag * z.imag) / 2.0;
        double nu = log(log_zn / log(2.0)) / log(2.0);
        iter = iter + 1 - (int)nu;
        return iter;
    }
}

int juliaset(double zx, double zy, double cx, double cy)
{
    complex z = {zx, zy};
    complex c = {cx, cy};
    int iter = 0;
    while((iter<MAX_ITER) && (((z.real*z.real)+(z.imag*z.imag))<=4))
    {
        double tempx=z.real;
        z.real=(tempx*tempx)-(z.imag*z.imag)+c.real;
        z.imag=((2)*(tempx)*(z.imag))+c.imag;
        iter++;   
    }

    if(iter==MAX_ITER)
    {
        return iter;
    }
    else
    {
    double log_zn = log(z.real * z.real + z.imag * z.imag) / 2;
    double nu = log(log_zn / log(2.0)) / log(2.0);
    iter = iter + 1 - nu;
    return iter;
    }
}

void hsv_to_rgb(int hue, int min, int max, COLORINDEX* p)
{
    max/=100;
    min/=100;
    int C= max*min;
    int X = C*(1-my_abs(((hue/60)%2)-1));
    int m = max-C;
    int r2,b2,g2;
    if(hue>=0 && hue<60)
    {
        r2=C;
        g2=X;
        b2=0;
    }
    else if(hue>=60 && hue<120)
    {
        r2=X;
        g2=C;
        b2=0;
    }
    else if(hue>=120 && hue<180)
    {
        r2=0;
        g2=C;
        b2=X;
    }
    else if(hue>=180 && hue<240)
    {
        r2=0;
        g2=X;
        b2=C;
    }
    else if(hue>=240 && hue<300)
    {
        r2=X;
        g2=0;
        b2=C;
    }
    else if(hue>=300 && hue<360)
    {
        r2=C;
        g2=0;
        b2=X;
    }
    p->r=(unsigned char)(r2+m)*255;
    p->g=(unsigned char)(g2+m)*255;
    p->b=(unsigned char)(b2+m)*255;
    p->junk=(unsigned char)0;
}

void UpdateImageData_mandelbrotset(ImageState* state)
{
    for(int x=0; x<state->width; x++)
    {
        for(int y=0; y<state->height; y++)
        {
            double dx= state->minx + ((double)x / state->width) * (state->maxx - state->minx);
            double dy= state->miny + ((double)y / state->height) * (state->maxy - state->miny);

            // angle should be in radians
            double rx= dx * cos(state->angle) + dy * sin(state->angle);
            double ry= -dx * sin(state->angle) + dy * cos(state->angle);

            int iter = get_mbs_iter(rx, ry);
            if(iter==MAX_ITER)
            {
                state->bmFileData.bmData[y * state->width + x] = 0;
            }
            else if(iter<7)
            {
                state->bmFileData.bmData[y * state->width + x] = 1;
            }
            else
            {
                state->bmFileData.bmData[y * state->width + x] = (int)(((double)iter/MAX_ITER) * 255);
            }
        }
    }
    for(int i=0; i<256; i++)
    {
        state->bmFileData.bmHeader.colorIdx[i].b=(unsigned char)100.0 * (double) i / 20.0;
        state->bmFileData.bmHeader.colorIdx[i].g=(unsigned char)130.0 * (double) i / 10.0;
        state->bmFileData.bmHeader.colorIdx[i].r=(unsigned char)90.0 * (double) i / 8.0;
    }
}   

void UpdateImageData_juliaset(ImageState* state,Circle* c)
{
    for(int x=0; x<state->width; x++)
    {
        for(int y=0; y<state->height; y++)
        {
            double dx= state->minx + ((double)x / state->width) * (state->maxx - state->minx);
            double dy= state->miny + ((double)y / state->height) * (state->maxy - state->miny);
            // angle should be in radians
            double rx= dx * cos(state->angle) + dy * sin(state->angle);
            double ry= -dx * sin(state->angle) + dy * cos(state->angle);

            double dxc= state->minx + ((double)c->cx / state->width) * (state->maxx - state->minx);
            double dyc= state->miny + ((double)c->cy / state->height) * (state->maxy - state->miny);
            double rxc= dxc * cos(state->angle) + dyc * sin(state->angle);
            double ryc= -dxc * sin(state->angle) + dyc * cos(state->angle);

            int iter = juliaset(rx, ry,rxc,ryc);
            if(iter==MAX_ITER)
            {
                state->bmFileData.bmData[y * state->width + x] = 0;
            }
            else if(iter<7)
            {
                state->bmFileData.bmData[y * state->width + x] = 1;
            }
            else
            {
                state->bmFileData.bmData[y * state->width + x] = (int)(((double)iter/MAX_ITER) * 255);
            }
        }
    }
    for(int i=0; i<256; i++)
    {
        state->bmFileData.bmHeader.colorIdx[i].b=(unsigned char)(201.0 * (double) i / 20.0);
        state->bmFileData.bmHeader.colorIdx[i].g=(unsigned char)(180.0 * (double) i / 10.0);
        state->bmFileData.bmHeader.colorIdx[i].r=(unsigned char)(210.0 * (double) i / 8.0);
    }
}   

void UpdateImageData_juliaset_raw(ImageState* state)
{
    for(int x=0; x<state->width; x++)
    {
        for(int y=0; y<state->height; y++)
        {
            double dx= state->minx + ((double)x / state->width) * (state->maxx - state->minx);
            double dy= state->miny + ((double)y / state->height) * (state->maxy - state->miny);

            // angle should be in radians
            double rx= dx * cos(state->angle) + dy * sin(state->angle);
            double ry= -dx * sin(state->angle) + dy * cos(state->angle);

            int iter = juliaset(rx, ry,-0.7269,0.1889);
            if(iter==MAX_ITER)
            {
                state->bmFileData.bmData[y * state->width + x] = 0;
            }
            else if(iter<70)
            {
                state->bmFileData.bmData[y * state->width + x] = 1;
            }
            else
            {
                state->bmFileData.bmData[y * state->width + x] = (int)(((double)iter/MAX_ITER) * 255);
            }
        }
    }
    for(int i=0; i<256; i++)
    {
        state->bmFileData.bmHeader.colorIdx[i].b=(unsigned char)(201.0 * (double) i / 20.0);
        state->bmFileData.bmHeader.colorIdx[i].g=(unsigned char)(180.0 * (double) i / 10.0);
        state->bmFileData.bmHeader.colorIdx[i].r=(unsigned char)(210.0 * (double) i / 8.0);
    }    
}   

void ChangeCenter(ImageState* state, double newcx, double newcy, int steps)
{
    double spanx = newcx - state->cx;
    double spany = newcy - state->cy;

    state->cx = newcx;
    state->cy = newcy;

    double xdif = spanx / steps;
    double ydif = spany / steps;

    double spx = state->maxx - state->minx;
    double spy = state->maxy - state->miny;


    for(int i=0;i<steps; i++)
    {
        state->minx += xdif;
        state->maxx = spx + state->minx;

        state->miny += ydif;
        state->maxy = spy + state->miny;

        UpdateImageData_mandelbrotset(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeZoom(ImageState* state, double zoom, int steps)
{
    double originalZoom = 1.0;
    double zoomFactor = pow(zoom / originalZoom, 1.0 / steps);

    for (int i = 0; i < steps; i++)
    {
        state->maxx = state->cx + (state->maxx - state->cx) / zoomFactor;
        state->minx = state->cx - (state->cx - state->minx) / zoomFactor;
        state->maxy = state->cy + (state->maxy - state->cy) / zoomFactor;
        state->miny = state->cy - (state->cy - state->miny) / zoomFactor;

        UpdateImageData_mandelbrotset(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);

        originalZoom *= zoomFactor;
    }
}

void Zoomout(ImageState* state, double zoom, int steps)
{
    double originalZoom = 1.0;
    double zoomFactor = pow(zoom / originalZoom, 1.0 / steps);

    for (int i = 0; i < steps; i++)
    {
        state->maxx = state->cx + (state->maxx - state->cx) * zoomFactor;
        state->minx = state->cx - (state->cx - state->minx) * zoomFactor;
        state->maxy = state->cy + (state->maxy - state->cy) * zoomFactor;
        state->miny = state->cy - (state->cy - state->miny) * zoomFactor;

        UpdateImageData_mandelbrotset(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);

        originalZoom /= zoomFactor;
    }
}

void Zoomrotate(ImageState* state, double zoom, double angle,int steps)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx / zoom;
    double finallenghty= lenghty /zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);

    double angle_step = angle / (57*steps);

    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;

        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;

        UpdateImageData_mandelbrotset(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeRotation(ImageState* state, double angle, int steps,Circle* c)
{
    double angle_step = angle / steps;
    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        UpdateImageData_mandelbrotset(state);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void Hold(ImageState* state, int steps)
{
    for(int i=0; i<steps; i++)
    {
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

//J = Juliaset only

void ChangeCenterJ(ImageState* state, double newcx, double newcy, int steps,Circle* c)
{
    double spanx = newcx - state->cx;
    double spany = newcy - state->cy;

    state->cx = newcx;
    state->cy = newcy;


    double xdif = spanx / steps;
    double ydif = spany / steps;

    double spx = state->maxx - state->minx;
    double spy = state->maxy - state->miny;


    for(int i=0;i<steps; i++)
    {
        state->minx += xdif;
        state->maxx = spx + state->minx;

        state->miny += ydif;
        state->maxy = spy + state->miny;
        UpdateImageData_juliaset(state,c);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeZoomJ(ImageState* state, double zoom, int steps,Circle* c)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx / zoom;
    double finallenghty= lenghty /zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);



    for(int i=0; i<steps; i++)
    {
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;
        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;

        UpdateImageData_juliaset(state,c);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ZoomoutJ(ImageState* state, double zoom, int steps,Circle* c)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx * zoom;
    double finallenghty= lenghty * zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);

    for(int i=0; i<steps; i++)
    {
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;
        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;

        UpdateImageData_juliaset(state,c);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ZoomrotateJ(ImageState* state, double zoom, double angle, int steps,Circle* c)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx / zoom;
    double finallenghty= lenghty /zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);

    double angle_step = angle / steps;

    for(int i=0; i<steps; i++)
    {
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;
        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;
        state->angle += angle_step;
        UpdateImageData_juliaset(state,c);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeRotationJ(ImageState* state, double angle, int steps,Circle* c)
{
    double angle_step = angle / steps;
    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        UpdateImageData_juliaset(state,c);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void HoldJ(ImageState* state, int steps,Circle* c)
{
    for(int i=0; i<steps; i++)
    {
        UpdateImageData_juliaset(state,c);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

//JD = juliaset Dance

void ChangeCenterJD(ImageState* state, double newcx, double newcy, int steps,Circle* c)
{
    double spanx = newcx - state->cx;
    double spany = newcy - state->cy;

    state->cx = newcx;
    state->cy = newcy;


    double xdif = spanx / steps;
    double ydif = spany / steps;

    double spx = state->maxx - state->minx;
    double spy = state->maxy - state->miny;

    for(int i=0;i<steps; i++)
    {
        state->minx += xdif;
        state->maxx = spx + state->minx;

        state->miny += ydif;
        state->maxy = spy + state->miny;
        state->cx=(state->maxx + state->minx) / 2;
        state->cy=(state->maxy + state->miny) / 2;

        UpdateImageData_juliaset(state,c);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 5;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 5;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 5;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 5;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeZoomJD(ImageState* state, double zoom, int steps,Circle* c)
{
    double originalZoom = 1.0;
    double zoomFactor = pow(zoom / originalZoom, 1.0 / steps);

    for (int i = 0; i < steps; i++)
    {
        state->maxx = state->cx + (state->maxx - state->cx) / zoomFactor;
        state->minx = state->cx - (state->cx - state->minx) / zoomFactor;
        state->maxy = state->cy + (state->maxy - state->cy) / zoomFactor;
        state->miny = state->cy - (state->cy - state->miny) / zoomFactor;

        UpdateImageData_juliaset(state,c);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 0.1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 0.1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 0.1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 0.1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, &state->bmFileData);
        originalZoom *= zoomFactor;
    }
}

void ZoomrotateJD(ImageState* state, double zoom, double angle, int steps,Circle* c)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx / zoom;
    double finallenghty= lenghty /zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);

    double angle_step = angle / steps;

    for(int i=0; i<steps; i++)
    {
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;
        state->angle += angle_step;
        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;
        UpdateImageData_juliaset(state,c);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ZoomoutJD(ImageState* state, double zoom, int steps,Circle* c)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx * zoom;
    double finallenghty= lenghty * zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);

    for(int i=0; i<steps; i++)
    {
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;
        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;
        UpdateImageData_juliaset(state,c);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void ChangeRotationJD(ImageState* state, double angle, int steps,Circle* c)
{
    double angle_step = angle / steps;
    for(int i=0; i<steps; i++)
    {
        state->angle += angle_step;
        UpdateImageData_juliaset(state,c);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 5;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 5;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 5;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 5;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void HoldJD(ImageState* state, int steps,Circle* c)
{
    for(int i=0; i<steps; i++)
    {
        UpdateImageData_juliaset(state,c);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 5;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 5;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 5;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 5;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void UpdateImageData_color(ImageState* state,double n)
{
    for(int x=0; x<state->width; x++)
    {
        for(int y=0; y<state->height; y++)
        {
            double dx= state->minx + ((double)x / state->width) * (state->maxx - state->minx);
            double dy= state->miny + ((double)y / state->height) * (state->maxy - state->miny);

            // angle should be in radians
            double rx= dx * cos(state->angle) + dy * sin(state->angle);
            double ry= -dx * sin(state->angle) + dy * cos(state->angle);

            int iter = get_mbs_iter(rx, ry);
            if(iter==MAX_ITER)
            {
                state->bmFileData.bmData[y * state->width + x] = 0;
            }
            else if(iter<7)
            {
                state->bmFileData.bmData[y * state->width + x] = 1;
            }
            else
            {
                state->bmFileData.bmData[y * state->width + x] = (int)(((double)iter/MAX_ITER) * 255);
            }
        }
    }
    for(int i=0; i<256; i++)
    {

        state->bmFileData.bmHeader.colorIdx[i].b=(unsigned char)100.0 * (double) i / 20.0*(n/25.0);
        state->bmFileData.bmHeader.colorIdx[i].g=(unsigned char)130.0 * (double) i / 10.0*(n/25.0);
        state->bmFileData.bmHeader.colorIdx[i].r=(unsigned char)90.0 * (double) i / 8.0*(n/25.0);
    }
}   

void dynamic_color(ImageState* state, int steps)
{
    for(int i=1; i<steps; i++)
    {
        UpdateImageData_color(state,i);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void dynamic_color_two(ImageState* state, int steps)
{
    for(int i=23; i<steps+20; i++)
    {
        UpdateImageData_color(state,i);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

//J = juliaset

void UpdateImageData_color_J(ImageState* state,Circle* c,double n)
{
    for(int x=0; x<state->width; x++)
    {
        for(int y=0; y<state->height; y++)
        {
            double dx= state->minx + ((double)x / state->width) * (state->maxx - state->minx);
            double dy= state->miny + ((double)y / state->height) * (state->maxy - state->miny);
            // angle should be in radians
            double rx= dx * cos(state->angle) + dy * sin(state->angle);
            double ry= -dx * sin(state->angle) + dy * cos(state->angle);

            double dxc= state->minx + ((double)c->cx / state->width) * (state->maxx - state->minx);
            double dyc= state->miny + ((double)c->cy / state->height) * (state->maxy - state->miny);
            double rxc= dxc * cos(state->angle) + dyc * sin(state->angle);
            double ryc= -dxc * sin(state->angle) + dyc * cos(state->angle);

            int iter = juliaset(rx, ry,rxc,ryc);
            if(iter==MAX_ITER)
            {
                state->bmFileData.bmData[y * state->width + x] = 0;
            }
            else if(iter<7)
            {
                state->bmFileData.bmData[y * state->width + x] = 1;
            }
            else
            {
                state->bmFileData.bmData[y * state->width + x] = (int)(((double)iter/MAX_ITER) * 255);
            }
        }
    }
    for(int i=0; i<256; i++)
    {
        state->bmFileData.bmHeader.colorIdx[i].b=(unsigned char)(100.0 * (double) i / 20.0*(n/50.0));
        state->bmFileData.bmHeader.colorIdx[i].g=(unsigned char)(130.0 * (double) i / 10.0*(n/50.0));
        state->bmFileData.bmHeader.colorIdx[i].r=(unsigned char)(90.0 * (double) i / 8.0*(n/50.0));
    }
}   

void dynamic_color_J(ImageState* state, int steps,Circle* c)
{
    for(int i=1; i<steps; i++)
    {
        UpdateImageData_color_J(state,c,i);
        if( 0<c->cx<c->R && 0<c->cy<c->R )
        {
            c->cx -= 1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( -c->R<c->cx<0 && 0<c->cy<c->R )
        {
            c->cx += 1;
            c->cy = sqrt(c->R * c->R - c->cx * c->cx);
        }
        else if( 0<c->cx<c->R && -c->R<c->cy<0 )
        {
            c->cx -= 1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        else if( -c->R<c->cx<0 && -c->R<c->cy<0 )
        {
            c->cx += 1;
            c->cy = -(sqrt(c->R * c->R - c->cx * c->cx));
        }
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void Hold_two(ImageState* state, int steps)
{
    for(int i=0; i<steps; i++)
    {
        UpdateImageData_color(state,i+23);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void fade(ImageState* state, int steps)
{
    for(int i=steps; i>-1;i--)
    {
        UpdateImageData_color(state,i);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void Zoomout_fade(ImageState* state, double zoom, int steps)
{
    double lenghtx=(state->maxx - state->minx);
    double lenghty=(state->maxy - state->miny);

    double finallenghtx= lenghtx * zoom;
    double finallenghty= lenghty * zoom;

    double spanx = (lenghtx - finallenghtx)/(2*steps);
    double spany = (lenghty - finallenghty)/(2*steps);

    for(int i=steps; i>-1 ;i--)
    {
        state->maxx -= spanx;
        state->minx += spanx;
        state->maxy -= spany;
        state->miny += spany;

        state->cx = (state->maxx + state->minx) / 2 ;
        state->cy = (state->maxy + state->miny) / 2 ;

        UpdateImageData_color(state,i);
        WriteBitmapFile(state->image_count++, & state->bmFileData);
    }
}

void fadeJ(ImageState* state,Circle* c,int steps)
{
    for(int i=steps; i>-1;i--)
    { 
        UpdateImageData_color_J(state,c,i);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void UpdateImageData_mandelbrotset_black(ImageState* state)
{
    for(int x=0; x<state->width; x++)
    {
        for(int y=0; y<state->height; y++)
        {
            double dx= state->minx + ((double)x / state->width) * (state->maxx - state->minx);
            double dy= state->miny + ((double)y / state->height) * (state->maxy - state->miny);

            // angle should be in radians
            double rx= dx * cos(state->angle) + dy * sin(state->angle);
            double ry= -dx * sin(state->angle) + dy * cos(state->angle);

            int iter = get_mbs_iter(rx, ry);
            if(iter==MAX_ITER)
            {
                state->bmFileData.bmData[y * state->width + x] = 0;
            }
            else if(iter<7)
            {
                state->bmFileData.bmData[y * state->width + x] = 1;
            }
            else
            {
                state->bmFileData.bmData[y * state->width + x] = (int)(((double)iter/MAX_ITER) * 255);
            }
        }
    }
    for(int i=0; i<256; i++)
    {
        state->bmFileData.bmHeader.colorIdx[i].b=(unsigned char)0;
        state->bmFileData.bmHeader.colorIdx[i].g=(unsigned char)0;
        state->bmFileData.bmHeader.colorIdx[i].r=(unsigned char)0;
    }
} 

void zoom_black(ImageState* state, double zoom, int steps)
{
    double originalZoom = 1.0;
    double zoomFactor = pow(zoom / originalZoom, 1.0 / steps);

    for (int i = 0; i < steps; i++)
    {
        state->maxx = state->cx + (state->maxx - state->cx) / zoomFactor;
        state->minx = state->cx - (state->cx - state->minx) / zoomFactor;
        state->maxy = state->cy + (state->maxy - state->cy) / zoomFactor;
        state->miny = state->cy - (state->cy - state->miny) / zoomFactor;

        UpdateImageData_mandelbrotset_black(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);

        originalZoom *= zoomFactor;
    }
}

#endif