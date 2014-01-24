/*
 *  task4.cpp
 *   by oZ/acy  1999-2001
 *   last update : 9 Apr. 2001
 */

#include "gpgui/mdi.h"
#include "gpgui/sys.h"


bool gplib::gui::System::taskRT_MDI(MDIFrame* frm)
{
   MSG msg;
   while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
   {
      if(msg.message==WM_QUIT)
      {
         ::PostQuitMessage(0);
         return false;
      }
      if(!TranslateMDISysAccel(frm->mdiclient_D,&msg))
      {
         ::TranslateMessage (&msg);
         ::DispatchMessage (&msg);
      }
   }
   return true;
}


//eof
