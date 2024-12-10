/** Header file generated with fdesign on Tue Dec 21 15:52:03 1999.**/

#ifndef FD_products_h_
#define FD_products_h_

/** Callbacks, globals and object handlers **/
extern void do_exit_button(FL_OBJECT *, long);
extern void do_select_computer_button(FL_OBJECT *, long);
extern void do_sigio_button(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *products;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *date1b51_text;
	FL_OBJECT *date1c51_text;
	FL_OBJECT *date2a53_text;
	FL_OBJECT *image_2a53;
	FL_OBJECT *image_2a54;
	FL_OBJECT *image_1b51;
	FL_OBJECT *image_1c51;
	FL_OBJECT *date2a54_text;
	FL_OBJECT *computer_text;
	FL_OBJECT *port_text;
} FD_products;

extern FD_products * create_form_products(void);

#endif /* FD_products_h_ */
