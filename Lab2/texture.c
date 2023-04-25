#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "stb-master/stb_image.h"

#include "texture.h"

void Texture_LoadTexture(char *file_name, int *textureId, int wrap, int filter, TTexProc proc)
{
    int width,height,cnt;
    unsigned char *data = stbi_load(file_name, &width,&height,&cnt,0);
    if(!data)   printf("%s \"%s\" %s\n","File",file_name, "not found.");
    if(proc) proc(data,width,height,cnt);
    glGenTextures(1, textureId);
    glBindTexture(GL_TEXTURE_2D, *textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D,0);
    stbi_image_free(data);
}
