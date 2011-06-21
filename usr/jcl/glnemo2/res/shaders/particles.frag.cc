// ============================================================================
// Copyright Jean-Charles LAMBERT - 2007-2011                                  
// e-mail:   Jean-Charles.Lambert@oamp.fr                                      
// address:  Dynamique des galaxies                                            
//           Laboratoire d'Astrophysique de Marseille                          
//           Pôle de l'Etoile, site de Château-Gombert                         
//           38, rue Frédéric Joliot-Curie                                     
//           13388 Marseille cedex 13 France                                   
//           CNRS U.M.R 6110                                                   
// ============================================================================
// See the complete license in LICENSE and/or "http://www.cecill.info".        
// ============================================================================
//
// Fragment shader for billboarding particles
// 
// ============================================================================

uniform sampler2D splatTexture;                                        

void main()                                                            
{           
  vec4 color = gl_Color * texture2D(splatTexture, gl_TexCoord[0].st);
  gl_FragColor = color ;        
}
// ============================================================================