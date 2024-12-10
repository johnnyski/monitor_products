/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "products.h"

FD_products *create_form_products(void)
{
  FL_OBJECT *obj;
  FD_products *fdui = (FD_products *) fl_calloc(1, sizeof(*fdui));

  fdui->products = fl_bgn_form(FL_NO_BOX, 780, 460);
  obj = fl_add_box(FL_UP_BOX,0,0,780,460,"");
    fl_set_object_color(obj,FL_DARKCYAN,FL_COL1);
  obj = fl_add_text(FL_NORMAL_TEXT,50,130,120,30,"Raw (1B-51)");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,560,130,140,30,"Conv/Strat Map");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_text(FL_NORMAL_TEXT,120,20,500,50,"GVS Product Monitor");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->date1b51_text = obj = fl_add_text(FL_NORMAL_TEXT,60,320,110,20,"Date/time");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->date1c51_text = obj = fl_add_text(FL_NORMAL_TEXT,220,320,110,20,"Date/time");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->date2a53_text = obj = fl_add_text(FL_NORMAL_TEXT,390,320,110,20,"Date/time");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  obj = fl_add_button(FL_NORMAL_BUTTON,290,400,100,40,"Exit");
    fl_set_object_color(obj,FL_DARKGOLD,FL_COL1);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_exit_button,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,50,400,220,40,"Select Computer");
    fl_set_object_color(obj,FL_DARKGOLD,FL_COL1);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_select_computer_button,0);
  fdui->image_2a53 = obj = fl_add_pixmap(FL_NORMAL_PIXMAP,370,160,150,150,"");
  fdui->image_2a54 = obj = fl_add_pixmap(FL_NORMAL_PIXMAP,550,160,150,150,"");
  fdui->image_1b51 = obj = fl_add_pixmap(FL_NORMAL_PIXMAP,40,160,150,150,"");
  fdui->image_1c51 = obj = fl_add_pixmap(FL_NORMAL_PIXMAP,200,160,150,150,"");
  obj = fl_add_text(FL_NORMAL_TEXT,220,130,120,30,"QC (1C-51)");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->date2a54_text = obj = fl_add_text(FL_NORMAL_TEXT,570,320,110,20,"Date/time");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fdui->computer_text = obj = fl_add_text(FL_NORMAL_TEXT,260,70,200,40,"Text");
    fl_set_object_boxtype(obj,FL_FRAME_BOX);
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  fdui->port_text = obj = fl_add_text(FL_NORMAL_TEXT,690,20,60,20,"Text");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,630,20,60,20,"Port#");
    fl_set_object_color(obj,FL_DARKCYAN,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,410,400,100,40,"SIGIO");
    fl_set_object_color(obj,FL_RED,FL_COL1);
    fl_set_object_lsize(obj,FL_HUGE_SIZE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
    fl_set_object_callback(obj,do_sigio_button,0);
  obj = fl_add_text(FL_NORMAL_TEXT,360,130,160,30,"Rainmap (2A-53)");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_BOLD_STYLE);
  fl_end_form();

  fdui->products->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

