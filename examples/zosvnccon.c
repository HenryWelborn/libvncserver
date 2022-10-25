#include <rfb/rfb.h>
#include <rfb/default8x16.h>
#include <signal.h> 

#define FONT_HEIGHT 16
#define FONT_WIDTH 8

#define FONT_HEIGHT 16
#define FONT_WIDTH 8
/* Flag used for threaded mode that's global so we can set it via
   a signal handler... */
static int maintain_connection = 1;

void signal_int(int signo)
{
  maintain_connection = 0;
	return;
}

int main(int argc,char** argv)
{            
  static const char msg[] =
      "Guest has not initialized the display (yet).";
  int width = 640;
  int height = 480;
  int len = strlen(msg);
  int x = (width / 2 ) - ( len * FONT_WIDTH / 2) ;
  int y = (height / 2 ); 

  rfbScreenInfoPtr server=rfbGetScreen(&argc,argv,width,height,8,3,4);
  if(!server)
    return 1;
  server->frameBuffer=(char*)malloc(width*height*4);

  rfbDrawString(server,&default8x16Font,x,y,msg,0xffffff);
  rfbInitServer(server);           
	signal (SIGINT, signal_int); /*When signal SIGINT (2) is issued */
  rfbRunEventLoop(server,-1,TRUE);
  while (maintain_connection) {
      sleep(5); /* render(); */
  }
  fprintf(stderr, "\nShutting down...\n");
  rfbShutdownServer(server, TRUE);
  return(0);
}
