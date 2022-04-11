#include "WFConfig.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include <iostream>

using boost::property_tree::ptree;
using std::string;
using std::cout;

WFConfig::WFConfig(std::string const& filename)
{
    ReadFile(filename);
}

int WFConfig::FireWrite() const { return fire_write_; }
int WFConfig::FireInBuffer() const { return fire_in_buffer_; }

int WFConfig::NumMCSims() const { return numMCSims_; }
int WFConfig::IgnBuffer() const { return ignBuffer_; }
int WFConfig::NParamRuns() const { return nParamRuns_; }
int WFConfig::SpreadCalcType() const { return spreadCalcType_; }
int WFConfig::FireVerbose() const { return fireVerbose_; }

int WFConfig::FirstYear() const { return firstYear_; };
int WFConfig::LastYear() const { return lastYear_; };
int WFConfig::FirstMonth() const { return firstMonth_; };
int WFConfig::LastMonth() const { return lastMonth_; };
double WFConfig::IgnitionTMin() const { return ignitionTMin_; }
double WFConfig::IgnitionCol() const { return ignitionCol_; }
double WFConfig::IgnitionRow() const { return ignitionRow_; }

double WFConfig::SeveritySlope() const { return severitySlope_; } // intercept and slope to interpolate percent mortality based on burn probability
double WFConfig::SeverityIntercept() const { return severityIntercept_; }

double WFConfig::LitterIntercept() const { return litterIntercept_; } // intercept and slope to interpolate percent mortality based on burn probability
double WFConfig::LitterSlope() const { return litterSlope_; }

double WFConfig::MeanLogWind() const { return mean_log_wind_; }
double WFConfig::SDLogWind() const { return sd_log_wind_; }
	
double WFConfig::Mean1RVM() const { return mean1_rvm_; }
double WFConfig::Mean2RVM() const { return mean2_rvm_; }
double WFConfig::Kappa1RVM() const { return kappa1_rvm_; }
double WFConfig::Kappa2RVM() const { return kappa2_rvm_; }
double WFConfig::PRVM() const { return p_rvm_; }
double WFConfig::MaxWind() const { return max_wind_; }

int WFConfig::NRows() const { return n_rows_; }
int WFConfig::NCols() const { return n_cols_; }
int WFConfig::VegIgn() const { return veg_ign_; }
// Include windspeed and wind direction
double WFConfig::VegFuelWeighting() const { return veg_fuel_weighting; }
double WFConfig::NDaysAverage() const { return ndays_average_; }


char const* WFConfig::EtFile() const { return etFile_.c_str(); }
char const* WFConfig::PetFile() const { return petFile_.c_str(); }
char const* WFConfig::UnderstoryEtFile() const { return understory_etFile_.c_str(); }
char const* WFConfig::UnderstoryPetFile() const { return understory_petFile_.c_str(); }
char const* WFConfig::ElevationFile() const { return elevationFile_.c_str(); }
char const* WFConfig::PatchFile() const { return patchFile_.c_str(); }
char const* WFConfig::MoistureFile() const { return moistureFile_.c_str(); }
char const* WFConfig::LoadFile() const { return loadFile_.c_str(); }
char const* WFConfig::DefFile() const { return defFile_.c_str(); }
char const* WFConfig::VegFile() const { return vegFile_.c_str(); }
//char const* WFConfig::ObsDistFile() const { return obsDistFile_.c_str(); }
//char const* WFConfig::ObsStatsFile() const { return obsStatsFile_.c_str(); }
//char const* WFConfig::ObsBurnFile() const { return obsBurnFile_.c_str(); }
char const* WFConfig::CritFile() const { return critFile_.c_str(); }


void WFConfig::ReadFile(std::string const& filename)
{
    ptree configuration;
    read_xml(filename, configuration);	// mk: filename is an xml file with the configuration variables.  read_xml assigns the configuration to the ptree configuration object, which contains the method "get", 
										// which assigns the elements of the xml file to the appropriate configuration variables. get is a template, which can accept different data types
										// the <> indicates with which data type get should be implemented.	
										// the gets don't have to be in the same order as in the file.
	// mk: Not sure why the FireSizeDistType(), etc., is necessary for these variables (?) i.e., above 

	printf("in config ReadFile, after read_xml\n");
//	calcStats_ =               configuration.get<bool>("WFConfig.CalcStats");#
//	writeEachFire_ =               configuration.get<bool>("WFConfig.WriteEachFire");
//	writeAllDists_=					configuration.get<bool>("WFConfig.WriteAllDists");
//	patchSizes_=					configuration.get<bool>("WFConfig.PatchSizes");
//	calcKappa_=					configuration.get<bool>("WFConfig.CalcKappa");
//	useSize_=					configuration.get<bool>("WFConfig.UseSize");
	numMCSims_=						configuration.get<int>("WFConfig.NumMCSims");
//	maxFailures_=						configuration.get<int>("WFConfig.MaxFailures");
	ignBuffer_=						configuration.get<int>("WFConfig.IgnBuffer");
//	minFireSize_=					configuration.get<int>("WFConfig.MinFireSize");
	nParamRuns_=					configuration.get<int>("WFConfig.NParamRuns");
	spreadCalcType_=				configuration.get<int>("WFConfig.SpreadCalcType");
	fireVerbose_=				configuration.get<int>("WFConfig.FireVerbose");
	firstYear_=				configuration.get<int>("WFConfig.FirstYear");
	lastYear_=				configuration.get<int>("WFConfig.LastYear");
	firstMonth_=				configuration.get<int>("WFConfig.FirstMonth");
	lastMonth_=				configuration.get<int>("WFConfig.LastMonth");
	fire_write_ = 				configuration.get<int>("WFConfig.FireWrite");
	
//	windSpeed_=					configuration.get<double>("WFConfig.WindSpeed");
//	windDir_=					configuration.get<double>("WFConfig.WindDir");
 	ignitionTMin_=					configuration.get<double>("WFConfig.IgnitionTMin");
	ignitionCol_=					configuration.get<double>("WFConfig.IgnitionCol");
	ignitionRow_=					configuration.get<double>("WFConfig.IgnitionRow");
// mk: for the enum variables, use the static_cast template, with variable type the enum variable.  It is an integer, which indicates which in the enumeration is selected.

	veg_fuel_weighting=configuration.get<double>("WFConfig.VegFuelWeighting");
	ndays_average_=configuration.get<double>("WFConfig.NDaysAverage");

	n_rows_=configuration.get<int>("WFConfig.NRows");
	n_cols_=configuration.get<int>("WFConfig.NCols");
	veg_ign_=configuration.get<int>("WFConfig.VegIgn");


//	severitySlope_=					configuration.get<double>("WFConfig.SeveritySlope");
//	severityIntercept_=					configuration.get<double>("WFConfig.SeverityIntercept");

	mean_log_wind_=configuration.get<double>("WFConfig.MeanLogWind");
	sd_log_wind_=configuration.get<double>("WFConfig.SDLogWind");
	
	mean1_rvm_=configuration.get<double>("WFConfig.Mean1RVM");
	mean2_rvm_=configuration.get<double>("WFConfig.Mean2RVM");
	kappa1_rvm_=configuration.get<double>("WFConfig.Kappa1RVM");
	kappa2_rvm_=configuration.get<double>("WFConfig.Kappa2RVM");
	p_rvm_=configuration.get<double>("WFConfig.PRVM");
	max_wind_=configuration.get<double>("WFConfig.MaxWind");
	
	elevationFile_=configuration.get<string>("WFConfig.ElevationFile");
	patchFile_=configuration.get<string>("WFConfig.PatchFile");
	moistureFile_=configuration.get<string>("WFConfig.MoistureFile");
	loadFile_=configuration.get<string>("WFConfig.LoadFile");
	etFile_=configuration.get<string>("WFConfig.EtFile");
	petFile_=configuration.get<string>("WFConfig.PetFile");
	understory_etFile_=configuration.get<string>("WFConfig.UnderstoryEtFile");
	understory_petFile_=configuration.get<string>("WFConfig.UnderstoryPetFile");
	defFile_=configuration.get<string>("WFConfig.DefFile");
	vegFile_=configuration.get<string>("WFConfig.VegFile");
//	obsDistFile_=configuration.get<string>("WFConfig.ObsDistFile");
//	obsStatsFile_=configuration.get<string>("WFConfig.ObsStatsFile");
//	obsBurnFile_=configuration.get<string>("WFConfig.ObsBurnFile");
	critFile_=configuration.get<string>("WFConfig.CritFile");
	printf("in config ReadFile, at end\n");
}

// mk: This shows, under "Usage" (see just before main), the format of the xml configuration file (prints to the screen)
void PrintWFConfigFile()
{
    cout << "<WFConfig>\n";
	cout << "\t<FireReturnInterval>3</FireReturnInterval><!-- How many years between fires?  If 0, then the number of fires=MaxYears below-->\n"; // mk added 10/13/11
	cout << "\t<DynTreeList>false</DynTreeList><!-- should the recorder trees randomly enter and leave the record?-->\n"; // mk added 3/31/11
	cout << "\t<CallCalcPBA>false</CallCalcPBA><!-- This is to evaluate P(B|A), rarely used and requires additional executable-->\n"; // mk: these match up to the get calls above, so callCalc_PBA_ gets from RFConfig.CallCalcPBA
	 cout << "\t<EvaluateFirePercolation>true</EvaluateFirePercolation> <!-- Writes a file of fire sizes if true-->\n";
	 cout << "\t<WriteEachFire>false</WriteEachFire><!-- Should fire progressions be written to a file? Slows the program-->\n";
	 cout << "\t<WriteAllDists>false</WriteAllDists><!-- Should the tree distances be written to a file?-->\n";
	 cout << "\t<WriteSOCResults>false</WriteSOCReslts><!-- if SOC, should fuel accumulation be written to file?-->\n";
	 cout << "\t<RecorderTreeScarring>true</RecorderTreeScarring><!-- Are we modeling recorder tree scarring as a fire effect?-->\n";
	 cout << "\t<IncludeTopography>false</IncludeTopography><!-- Should topography be included in fire spread calculation?-->\n";
	 cout << "\t<MaxSDDistance>11000</MaxSDDistance><!-- the max distance at which SD should be calculated-->\n";
    cout << "\t<RandomSeed>0</RandomSeed> <!-- seed is used if non-zero -->\n";
	cout<<"\t<MaxFires>80</MaxFires><!-- Maximum number of fires allowed in history for SOC-->\n";
 	cout<<"\t<MinFires>1</MinFires><!-- First fire to calculate statistics for SOC-->\n";
  	cout<<"\t<MaxYears>200</MaxYears><!-- Maximum number of years in the fire history, or the number of iterations for SOC -->\n";
  	cout<<"\t<IgnBuffer>5</IgnBuffer><!-- How many pixels from edge for ignitions? -->\n";
  	cout<<"\t<IgnRow>-1</IgnX><!-- -1 for random ignitions, otherwise row index (where 0 is the top) of fixed ignition -->\n";
  	cout<<"\t<IgnColumn>-1</IgnY><!-- -1 for random ignitions, otherwise column index (where 0 is the left) of fixed ignition -->\n";
    cout << "\t<FireSizeDistType>0</FireSizeDistType><!-- 0=gamma,1=Pareto,2=negative exponential,3=lognormal  -->\n";
    cout << "\t<FireSpreadStrategy>0</FireSpreadStrategy><!-- 0=simple spread,1=SODP2Param,2=SODP3Param,3=SOC,4=memory -->\n";// mk edited 6/3/11, to make this configuration switch between simple spread probability, SODP, and SOC
	cout << "\t<PointProcessType>0</PointProcessType><!-- 0=CSR,1=clustered,2=observed -->\n";
	cout <<"\tNumMCSims>1</NumMCSims><!-- number of replicate simulations -->\n";
	cout << "\t<LandscapeDataFile>your_file_here</LandscapeDataFile><!-- grid file name -->\n";
	cout << "\t<ElevationFile>your_file_here</ElevationFile><!-- grid file name for dem (m) -->\n";
	cout << "\t<MoistureFile>your_file_here</MoistureFile><!-- grid file name for fuel moisture-->\n";
	cout << "\t<EtFile>your_file_here</EtFile><!-- grid file name for actual evapotranspiration-->\n";
	cout << "\t<PetFile>your_file_here</PetFile><!-- grid file name for potential evapotranspiration-->\n";
	cout << "\t<LoadFile>your_file_here</LoadFile><!-- grid file name for fuel load-->\n";
	cout <<"\t<WindSpeed>0</WindSpeed><!--wind speed in m/s-->\n";
	cout <<"\t<WindDir>0</WindSpeed><!--wind direction in rad-->\n";

  cout << "</WFConfig>\n";
}


