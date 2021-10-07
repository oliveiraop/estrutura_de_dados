cmake_minimum_required(VERSION 2.6)

include_directories(D:/Mazinho/Problema2/Arvore/external/SOIL/inc)

ADD_LIBRARY( soil STATIC
  D:/Mazinho/Problema2/Arvore/external/SOIL/src/image_helper.c
  D:/Mazinho/Problema2/Arvore/external/SOIL/src/stb_image_aug.c
  D:/Mazinho/Problema2/Arvore/external/SOIL/src/image_DXT.c
  D:/Mazinho/Problema2/Arvore/external/SOIL/src/SOIL.c
  )
