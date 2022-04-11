#pragma once
#include <string>

void PrintWFConfigFile(); // mk: called in the Usage after main, shows how the configuration file should be structured

class WFConfig // mk: a class to read in the configuration file and assign each element to the appropriate variable.  
						// These are constants, so they cannot be changed in the function once a value is assigned
{
public:
	WFConfig(std::string const& filename); // mk: basically, the main function for the configuration, calls ReadFile, the constructor?
 
	int FireWrite() const;
	int FireInBuffer() const;
//	bool CalcStats() const;
//	bool WriteEachFire() const;
//	bool WriteAllDists() const;
//	bool PatchSizes() const;
//	bool CalcKappa() const;
//	bool UseSize() const;
	int NumMCSims() const;
//	int MaxFailures() const;
	int IgnBuffer() const;
//	int MinFireSize() const;
	int NParamRuns() const;
	int SpreadCalcType() const;
	int FireVerbose() const;

	int FirstYear() const;
	int LastYear() const;
	int FirstMonth() const;
	int LastMonth() const;

	// Include windspeed and wind direction
//	double WindSpeed() const; // m/s?
//	double WindDir() const; // rad
	double IgnitionTMin() const; //deg C
	double IgnitionCol() const; 
	double IgnitionRow() const; 

	int NRows() const;
	int NCols() const;
	int VegIgn() const;
	// Include windspeed and wind direction
	double VegFuelWeighting() const; // m/s?
	double NDaysAverage() const; // rad


	double SeveritySlope() const;
	double SeverityIntercept() const;
	
	double LitterSlope() const;
	double LitterIntercept() const;
	
	double MeanLogWind() const;
	double SDLogWind() const;
	
	double Mean1RVM() const;
	double Mean2RVM() const;
	double Kappa1RVM() const;
	double Kappa2RVM() const;
	double PRVM() const;
	
	double MaxWind() const;

	// mk: some configurations for filenames
	char const* ElevationFile() const;
	char const* PatchFile() const;

	char const* PetFile() const; // fuel moisture grid filename
	char const* EtFile() const; // fuel moisture grid filename
	char const* UnderstoryPetFile() const; // fuel moisture grid filename
	char const* UnderstoryEtFile() const; // fuel moisture grid filename
	char const* MoistureFile() const; // fuel moisture grid filename
	char const* LoadFile() const;	// fuel load grid filename
	char const* DefFile() const ;
	char const* VegFile() const ;
//	char const* ObsDistFile() const ;
//	char const* ObsStatsFile() const ;
//	char const* ObsBurnFile() const ;
	char const* CritFile() const ;
	

 
private:
   // whether to write the individual fires to a file
	int fire_write_;
	int fire_in_buffer_;
//	bool calcStats_;
//	bool writeEachFire_;
//	bool writeAllDists_;
//	bool patchSizes_;
//	bool calcKappa_;
//	bool useSize_;
	int numMCSims_;
//	int maxFailures_;
	int ignBuffer_;
//	int minFireSize_;
	int nParamRuns_;
	int spreadCalcType_;
	int fireVerbose_;
	int firstYear_;
	int lastYear_;
	int firstMonth_;
	int lastMonth_;

//	double windSpeed_;
//	double windDir_;
	double ignitionTMin_;
	double ignitionCol_;
	double ignitionRow_;

	double severitySlope_;
	double severityIntercept_;
	
	double litterSlope_;
	double litterIntercept_;

	double ndays_average_;
	int n_rows_;
	int n_cols_;
	int veg_ign_;
	double veg_fuel_weighting;

	double mean_log_wind_;
	double sd_log_wind_;
	double mean1_rvm_;
	double mean2_rvm_;
	double kappa1_rvm_;
	double kappa2_rvm_;
	double p_rvm_;
	double max_wind_;

	std::string petFile_;
	std::string etFile_;
	std::string understory_petFile_;
	std::string understory_etFile_;
	std::string elevationFile_;
	std::string patchFile_;
	std::string moistureFile_;
	std::string loadFile_;
	std::string defFile_;
	std::string vegFile_;
//	std::string obsDistFile_;
//	std::string obsStatsFile_;
//	std::string obsBurnFile_;
	std::string critFile_;

	void ReadFile(std::string const& filename);  
};
