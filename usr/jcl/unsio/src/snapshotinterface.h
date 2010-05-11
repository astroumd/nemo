// ============================================================================
// Copyright Jean-Charles LAMBERT - 2008-2010                                       
// e-mail:   Jean-Charles.Lambert@oamp.fr                                      
// address:  Dynamique des galaxies                                            
//           Laboratoire d'Astrophysique de Marseille                          
//           Pole de l'Etoile, site de Chateau-Gombert                         
//           38, rue Frederic Joliot-Curie                                     
//           13388 Marseille cedex 13 France                                   
//           CNRS U.M.R 6110                                                   
// ============================================================================

/**
   @author Jean-Charles Lambert <Jean-Charles.Lambert@oamp.fr>
*/
#ifndef SNAPSHOTINTERFACE_H
#define SNAPSHOTINTERFACE_H
#include "componentrange.h"
#include "userselection.h"
#include "ctools.h"
//#include "uns.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <assert.h>

#define MAX_EPS 5

namespace uns {
  class CSelectTime;
  typedef std::vector<CSelectTime> CSelectTimeVector;

  //                             
  // Class to store selected time
  //                             
  class CSelectTime {
  public:
    CSelectTime(const float _i, const float _s, 
		const float _o, const float _l):inf(_i),sup(_s),offset(_o),lastt(_l) {
    };
    const CSelectTime & operator=(const CSelectTime& m) {
      inf=m.inf;
      sup=m.sup;
      lastt=m.lastt;
      offset=m.offset;
      return *this;
    }
    float inf,sup,offset,lastt;
  };
  //                                     
  // class to store virtual snapshot data
  //                                     
  class CSnapshotInterfaceIn {

  public:
    CSnapshotInterfaceIn() {};
    // READING constrcuctor
    CSnapshotInterfaceIn(std::string _name, const std::string _comp, 
                       const std::string _time, const bool verb=false) {

      filename = _name;
      select_part = _comp;
      select_time = _time;
      obj=NULL; pos=NULL; vel=NULL;
      mass=NULL;
      end_of_data=false;
      verbose = verb;
      first=true;
      valid=false;
      crvs=NULL;
      crv.clear();
      stv.clear();
      parseSelectTime();
      
    };
    virtual ~CSnapshotInterfaceIn() { crv.clear(); stv.clear();};
    // ---------------------------------------------------
    // READING operations
    // Pure Virtual functions, *** MUST be implemented ***
    // ---------------------------------------------------

    virtual ComponentRangeVector * getSnapshotRange() = 0;    
    // index_tab = array of selected indexes (size max=part_data->nbody)
    // nsel      = #particles selected in index_tab                     
    // particles not selected must have the value '-1'                  
    virtual int nextFrame(const uns::t_indexes_tab * index_tab, const int nsel)= 0;     
    virtual int close() = 0;
    virtual bool getData(const std::string,int *,float **)=0;
    virtual bool getData(const std::string,      float * )=0;
    virtual bool getData(const std::string,int *,int   **)=0;
    virtual bool getData(const std::string,      int   * )=0;
    virtual bool getData(const std::string, const std::string ,int *,float **)=0;
    virtual bool getData(const std::string, const std::string ,int *,int   **)=0;
    
    // Virtual function with a default behaviour
    virtual bool isEndOfData() const { return end_of_data;};
    virtual std::string getFileName() { return filename;};
    virtual int  getNSel() { return nsel;}
    virtual void setNsel(const int _nsel) { nsel = _nsel;}
    virtual bool isNewFrame() { return true;};
    virtual int nextFrame(std::string bits="mxvpaekXRMAHIU");
    virtual ComponentRangeVector * getCrvFromSelection() { return user_select.getCrvFromSelection();}
    virtual bool shift(std::string,const float x, const float y, const float z);
    virtual float   getEps(const std::string) { return -1.;};
    virtual int     getCod(const std::string select, const float time, 
			   float * tcod, const std::string base="ANALYSIS/cod",
			   const std::string ext="cod") { 
      if ((select=="")?0:1) {;};
      if (time) {;}
      if (tcod) {;}
      if ((base=="")?0:1) {;}
      if ((ext=="")?0:1) {;}
      return -1;
    };
    
    // normal functions        
    bool isValidData() { return valid; };
    void setFileName(std::string _f) { filename = _f;};
    bool getRangeSelect(const char *, int *, int *, int * , bool fortran=false);
    //std::string getFileName() const { return filename;};
    std::string getInterfaceType() { return interface_type;};
    int getInterfaceIndex() { return interface_index; };
    bool isFileExist() { return true; };
    std::string getSelectPart() { return select_part; };
    std::string getSelectTime() { return select_time; };    
    int nbody_first;
    float time_first, time;
    ComponentRangeVector crv_first;
    UserSelection user_select; // object to store user component selection
  protected:
    // READING    
    int nsel;
    
    CSnapshotInterfaceIn * obj;
    std::string filename;
    std::string interface_type;
    int interface_index;
    mutable bool end_of_data;
    std::string select_part, select_time;
    bool keep_all, load_vel;
    ComponentRangeVector crv, * crvs;
    float * pos, *vel, *mass;
    bool first,valid;
    static std::string sim_db_file;
    static std::string eps_db_file;
    static std::string nemo_range_file;

    
    int req_bits;
    void computeBits(std::string);
    CSelectTimeVector stv;
    void parseSelectTime();
    inline bool diffTime(const float t,const float fuzz=0.000001) {
      return ((fabs(t)<fuzz)?true:false);
    }
    virtual int nextFrameSelect(ComponentRangeVector * crvs);
    void getRangeTime(std::string);
    bool checkRangeTime(const float);
    float eps[MAX_EPS]; // gas, halo, disk, bulge, stars
    
    bool verbose; // display some info if verbose
  };  // end of class CSnapshotInterfaceIn


  class CSnapshotInterfaceOut {
  public:
    // WRITING constructor
    CSnapshotInterfaceOut(const std::string _n, const std::string _t, const bool _verb=false) {
      simname    = _n;
      simtype    = tools::Ctools::tolower(_t);
      verbose    = _verb;
    }
    virtual ~CSnapshotInterfaceOut() {};
    // ---------------------------------------------------
    // WRITING operations
    // Pure Virtual functions, *** MUST be implemented ***
    // ---------------------------------------------------
    virtual int setHeader(void * ) = 0;
    virtual int setNbody(const int) = 0;
    virtual int setData(std::string, float) = 0;
    virtual int setData(std::string, const int,  float *,const bool _addr=false)=0;
    // array by double keys
    // like "halo" "pos" => position of the halo requested
    virtual int setData(std::string, std::string, const int , float *,const bool _addr=false)=0;
    virtual int setData(std::string, std::string, const int , int   *,const bool _addr=false)=0;
    virtual int setData(std::string, const int , 
		float *, float *, float *,const bool _addr=false)=0;
    virtual int setData(std::string, const int, int *,const bool _addr=false)=0;
    virtual int save()=0;
    virtual std::vector<double> moveToCom()=0;

  protected:
    // WRITING
    std::string simname, simtype;
    bool verbose;
    
  }; // end of class CSnapshotInterfaceOut
} // namespace



#endif
