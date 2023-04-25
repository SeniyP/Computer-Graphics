#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

typedef void (*TTexProc) (unsigned char *data, int width, int height, int cnt);

void Texture_LoadTexture(char *file_name, int *textureId, int wrap, int filter, TTexProc proc);

#endif // TEXTURE_H_INCLUDED
