//
//  sciter-gtk-main.mm
//  sciter
//
//  Created by andrew on 2014-12-05.
//  Copyright (c) 2014 Andrew Fedoniouk. All rights reserved.
//

#include <gtk/gtk.h>

#include "sciter-x-window.hpp"

int main (int argc, char *argv[])
{
  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  auto message_pump = []() -> int {
    gtk_main ();
    return 0;
  };

  return uimain(message_pump);
}

namespace sciter {

  GtkWidget* gview(HWINDOW hwnd) { return hwnd; }
  GtkWindow* gwindow(HWINDOW hwnd) { return hwnd ? GTK_WINDOW(gtk_widget_get_toplevel(hwnd)): nullptr; }

  HINSTANCE window::get_resource_instance()
  {
    return 0; // not used
  }

  bool window::load( aux::bytes utf8_html, const WCHAR* base_url)
  {
     return FALSE != SAPI()->SciterLoadHtml(_hwnd,utf8_html.start,utf8_html.length, base_url);
  }
  bool window::load( aux::chars utf8_html, const WCHAR* base_url)
  {
     return FALSE != SAPI()->SciterLoadHtml(_hwnd,(LPCBYTE)utf8_html.start,utf8_html.length, base_url);
  }
  bool window::load( const WCHAR* url)
  {
     return FALSE != SAPI()->SciterLoadFile(_hwnd,url);
  }

  void window::collapse() {
    if(_hwnd) gtk_window_iconify (gwindow(_hwnd));
  }
  void window::expand( bool maximize) {
    if(_hwnd) gtk_window_present (gwindow(_hwnd));
  }

  void window::dismiss() {
    if(_hwnd) gtk_window_close (gwindow(_hwnd));
    _hwnd = 0; //?
  }

  window::window( UINT creationFlags, RECT frame): _hwnd(NULL)
  {
    _hwnd = SAPI()->SciterCreateWindow(creationFlags, (frame.right - frame.left) > 0 ? &frame: NULL,NULL,this,NULL);
    if( _hwnd ) {
      add_ref();
      setup_callback();
      sciter::attach_dom_event_handler(get_hwnd(),this);
    }
  }

}
