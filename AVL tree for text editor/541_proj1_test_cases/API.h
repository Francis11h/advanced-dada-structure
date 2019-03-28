#ifndef __CSC541_API_H__
#define __CSC541_API_H__

class text_t;
extern text_t * create_text();
extern int length_text( text_t *txt);
extern char * get_line( text_t *txt, int index);
extern void append_line( text_t *txt, char * new_line);
extern char * set_line( text_t *txt, int index, char * new_line);
extern void insert_line( text_t *txt, int index, char * new_line);
extern char * delete_line( text_t *txt, int index);

#endif
