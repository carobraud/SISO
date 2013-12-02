#ifndef CsaveNetCDF
#define CsaveNetCDF

/**
 * \file      CsaveNetCDF.h (from DAQdata.h of DAQlml)
 * \author    BRAUD C.
 * \version   1.0
 * \date      2 December 2013
 * \brief     simple NetCDF saving
 * \details    
 */



//! save recorded data and additional informations
/**
 * save all recoreded data regarding to a single time axis (in order to display it with time axis under ncview for example)
 * \note each recorded channel is also written with its corresponding time axis (especially with the channel name prefix)
 *
 * @param fo 
 * @param data 
 * @param time 
 * @param DAQdev
 * @param data_unit_name 
 *
 * @return 
 */
//! \todo [medium] make NetCDF file object version (\c  save_data(NcFile*fp,... ) and its overload with file name
template <typename Tdata, typename Ttime>
int save_data(
  std::string file_name,
  cimg_library::CImgList<Tdata>& data,
  cimg_library::CImgList<Ttime>& time,
  DAQdevice DAQdev,
  std::string data_unit_name
 // Tacqu acqu_range_min, Tacqu acqu_range_max, std::string &acqu_range_unit,
 // Tdata phys_range_min, Tdata phys_range_max, std::string &phys_range_unit
)
{
//std::cerr<<__FILE__<<"/"<<__func__<<"(\""<<file_name<<"\",...)\n"<<std::flush;
//data.print("data");
//time.print("time");
  ///open file (shared between \c fod and \c fot )
  //NetCDF file objects for both data and time
  CImgListNetCDF<Tdata> fod;//e.g. "pressure", "hot_wire"
  CImgListNetCDF<Ttime> fot;//e.g. "time","pressure__time","hot_wire__time"
  //open NetCDF file
  fod.saveNetCDFFile((char*)file_name.c_str());
  //set NetCDF file object for time
  fot.setNetCDFFile(fod.getNetCDFFile());
  ///create dimension (shared between \c fod and \c fot )
  //std::cerr<<"create dimension\n"<<std::flush;
  //dimension name (only one here, as 1D data)
  std::vector<std::string> dim_names;
  //!\note set both dimension and axis names of time to "time"
  dim_names.push_back("time");
  //create dimension
  fod.addNetCDFDims(data,dim_names);
  //set dimension for time
  fot.setNetCDFDims(fod.vpNCDim,fod.pNCDimt);
  ///create variables (and attributes)
  //std::cerr<<"create variables\n"<<std::flush;
  NcFile *fp=fod.getNetCDFFile();
  ///- data variables
  ////data attribute units
  std::vector<std::string> data_unit_names;
  cimglist_for(data,c) data_unit_names.push_back(data_unit_name);
  ////create data variables
  fod.addNetCDFVar(data,DAQdev.channel_name_AI,data_unit_names);
  ////data attribute channel indexes
  std::string index_name("channel_index");
  cimglist_for(data,c) (fod.pNCvars[c])->add_att(index_name.c_str(),DAQdev.channel_index_AI[c]);
  ////data attribute ranges
  /*std::string range_name("physical_range");
  Tdata phys_range[2];
  phys_range[0]=phys_range_min;phys_range[1]=phys_range_max;
  cimglist_for(data,c) (fod.pNCvars[c])->add_att(range_name.c_str(),2,phys_range);
  range_name="physical_range_unit";
  cimglist_for(data,c) (fod.pNCvars[c])->add_att(range_name.c_str(),(const char*)phys_range_unit.c_str());
  range_name="acquisition_range";
  Tacqu acqu_range[2];
  acqu_range[0]=acqu_range_min;acqu_range[1]=acqu_range_max;
  cimglist_for(data,c) (fod.pNCvars[c])->add_att(range_name.c_str(),2,acqu_range);
  range_name="acquisition_range_unit";
  cimglist_for(data,c) (fod.pNCvars[c])->add_att(range_name.c_str(),(const char*)acqu_range_unit.c_str());
  */
  ///- time variables (first for all data, then each data have it own time)
  ////time data
  cimg_library::CImgList<Ttime> store_time(time.size()+1);
  store_time[0]=time[0];
  store_time[1]=time[0];
  //  cimglist_for(time,c) store_time[c+1]=time[c];
  ////time names
  std::vector<std::string> time_var_names;
  time_var_names.push_back(dim_names[0]);
  time_var_names.push_back(dim_names[0]);
  ////time units
  //! \todo add unit__long_name="second" refering to unit="s"
  std::vector<std::string> time_unit_names(store_time.size(),"second");
  ////create data variables
  fot.addNetCDFVar(store_time,time_var_names,time_unit_names);
  ///create global attributes
//std::cerr<<"create globals\n"<<std::flush;
  ///- sampling rate
  fp->add_att("sampling_rate", DAQdev.sampling_rate_AI);
  ///- sampling rate
//! \todo . save range: acquisition [min, max], physical [min, max] (as both global and local attribute)
  fp->add_att("range_id", DAQdev.range_id_AI);
  ///write data
//std::cerr<<"create data\n"<<std::flush;
  ///- data
  fod.addNetCDFData(data,0);
  ///- time
  fot.addNetCDFData(store_time,0);
  return 0;
}


#endif// CsaveNetCDF

