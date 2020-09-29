#include "home.h"

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

const char *home_dir()
{
   char *name;	
   struct passwd *g;

   g = getpwuid( getuid() ); 

   name = g->pw_dir;

   return(name);

}

const char *share_dir()
{
#ifdef SHARE_DIR
    static const char *share = SHARE_DIR;
#else
    static const char *share = "/usr/local/share/Qtradio";
#endif
    return share;
}
