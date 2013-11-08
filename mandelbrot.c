//  Mandelbrotset in C
//  There is lots of room for improvements, this is just a scribble
//  Based on the pseudocode from wikipedia
//  https://en.wikipedia.org/wiki/Mandelbrot_set
//  2013 Rutger Plak

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

struct winsize w;
void INThandler(int);
void printboard();
void colorr();
void exitt();

int main(int argc, char *argv[]) {
  signal(SIGINT, INThandler);

  ioctl(0, TIOCGWINSZ, &w);
  char command;
  char undo[w.ws_row*w.ws_col];
  int zoom;
  int xoffset;
  int yoffset;
  zoom = 1;
  int x;
  xoffset = 0;
  yoffset = 0;

  for(x=0; x < w.ws_row * w.ws_col; x++) 
    undo[x] = '\b';
  

  printboard(1,0,0);
  while(1) {

    printf("%s",undo);
    printf("+ = zoom in, - = zoom out, use wasd for moving:\n");
    scanf("%c",&command);
    switch(command) {
      case 'q':
        exitt();
        break;
      case '+':
        zoom++;
        break;
      case '-':
        if(zoom > 0) zoom--;
        break;
      case 'd':
        xoffset++;
        break;
      case 'a':
        xoffset--;
        break;
      case 'w':
        yoffset++;
        break;
      case 's':
        yoffset--;
        break;
      case 'b':
        break;
    }
    printboard(zoom,xoffset,yoffset);
  }
  return 1;
}

void printboard(int zoom, int xoffset, int yoffset) {

  char characters[11] = {' ',' ', '.', ',', '-', '~', '>', '+', '=', '#'};
  float x0;
  float y0;
  float i;
  float j;
  float x;
  float y;
  int iteration;
  int max_iteration;
  max_iteration = 1000;
  float xtemp;
  int charac;
  char c;
  for(j=0; j < w.ws_row; j++) {
    for(i=0; i<w.ws_col; i++) {
      iteration = 0;
      x = 0.0;
      y = 0.0;
      xtemp = 0.0;
      x0 = (i/zoom + xoffset) / (w.ws_col / 3.5)-2.5;
      y0 = (j/zoom - yoffset) / (w.ws_row / 2)-1;
      while(x*x + y*y < 2*2 && iteration < max_iteration) {
        xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
      }
      charac = iteration%7;
      colorr(0, charac, charac);
      printf("%c",characters[iteration%9]);
    }
  }
}

void  INThandler(int sig)
{
  char undo[w.ws_row*w.ws_col];
  int x;
  signal(sig, SIG_IGN);
  exitt();
}
void exitt() {
  int x;
  char undo[w.ws_row*w.ws_col];

  for(x=0; x < w.ws_row * w.ws_col; x++) {
    undo[x] = ' ';
  }
  printf("%s", undo);
  exit(0);
}
void colorr(int attr, int fg, int bg){
  int colors[8] = {31,33,32,34,35,36,31,30};
  attr = 1;
  bg = 40;
  fg = colors[fg];
  char command[13];
  sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg, bg);
  printf("%s", command);
}
