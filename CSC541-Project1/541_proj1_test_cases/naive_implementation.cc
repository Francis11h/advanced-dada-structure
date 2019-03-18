#include <vector>
#include <iostream>
#include <stdlib.h>

struct text_t{
    //std::vector<char*> _text;
    struct text_t *left;
    struct text_t *right;
    int key;
    int height;
    char* _text;
};

//creates an empty text, whose length is 0
text_t * create_text(){
    return new text_t;
}


//returns the number of lines of the current text
int length_text(text_t *txt){
    if (txt == NULL) return -1;
    return txt->_text.size();
}

//if such a line exists, return the line of number index; 
//if such a line does not exist, returns NULL.
char * get_line( text_t *txt, int index){
    return txt->_text[index-1];
}

//appends new line as new last line.
void append_line( text_t *txt, char * new_line)
{
    txt->_text.push_back(new_line);
}

//sets the line of number index, if such a line exists, to new line, and returns a pointer to the previous line of that number. 
//If no line of that number exists, it does not change the structure and returns NULL.

char * set_line( text_t *txt, int index, char * new_line){
    char *old = txt->_text[index-1];
    txt->_text[index-1] = new_line;
    return old;
}

//inserts the line before the line of number index, if such a line exists, to new line, renumbering all lines after that line. 
//If no such line exists, it appends new line as new last line.

void insert_line( text_t *txt, int index, char * new_line){
    txt->_text.insert(txt->_text.begin() + index - 1, new_line);
}

//deletes the line of number index, renumbering all lines after that line, and returns a pointer to the deleted line.
char * delete_line( text_t *txt, int index)
{
    char *old = txt->_text[index-1];
    txt->_text.erase(txt->_text.begin()+index-1);
    return old;
}