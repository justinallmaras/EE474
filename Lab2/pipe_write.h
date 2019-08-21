#ifndef TOOL_H
#define TOOL_H

void lineWrap(char* str, int wrapLen);
void centerText(char* str, int size, int disp_size, char* result);
int readFile(char * buf, int BUF_SIZE, char* path);

#endif