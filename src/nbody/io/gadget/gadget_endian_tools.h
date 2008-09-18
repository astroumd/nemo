// ============================================================================
// Copyright Jean-Charles LAMBERT - 2006
// e-mail:   Jean-Charles.Lambert@oamp.fr
// address:  Dynamique des galaxies
//           Laboratoire d'Astrophysique de Marseille
//           2, place Le Verrier
//           13248 Marseille Cedex 4, France
//           CNRS U.M.R 6110
// ============================================================================
// gadget_endian_tools.h                                                        
// Definitition of the class GadgetEndianTools                                  
//                                                                              
// This class perform data IO operation according to the endianness             
//                                                                              
// Intel/Amd/Alpha are little-endian compliant                                  
//                                                                              
// PowerPC IBM/Sparc/SGI are big-endian compliant                              
// ============================================================================ 
#ifndef GADGET_ENDIAN_TOOLS_H
#define GADGET_ENDIAN_TOOLS_H

#include <cstdio>          // changed from <stdio.h>  18-Sep-2008 (WD)
#include "gadget_data_structure.h"

class GadgetEndianTools {
public:
  enum ioop {
    READ,WRITE
  };

  //constructor
  GadgetEndianTools(const FILE*_fd, bool _swap) : swap(_swap), fd(_fd) {}
  //destructor
  ~GadgetEndianTools() {}

  int ioHeader(t_io_header_1 * header, ioop op);
  int ioData(char * ptr,int size_bytes, int items, ioop op);

 private:
  bool swap;                // control swapping variable
  const FILE * fd;          // file descriptor
  
  void swapBytes(void * x,const int size);
  
};
#endif
// -----------------------------------------------------------------------------
