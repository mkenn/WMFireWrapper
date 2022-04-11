/****************************************************************/
/* reads in landscape data, allocates a grid, calls the wmfire	*/
/* shared library, writes the results to a file for analysis		*/
/* used to test the WMFire shared library				*/
/****************************************************************/
/* needs the header file that establishes the interface with 	*/
/* the shared library								*/
/****************************************************************/
#include <cstdio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "boost/shared_ptr.hpp"

using std::cout;
using std::stringstream;
using std::fstream;
using std::ifstream;
using std::ofstream;
using boost::shared_ptr;
#include "unistd.h"

#include "WFConfig.h"
#include "Wrapper.h"
//#include "../util/WMFireInterface.h"

using std::cout;

/**********************************************************************/
/* reads in landscape data, allocates the grid, calls the library	*/
/* and writes out each fire								*/
/**********************************************************************/
void Landscape::Wrapper(WFConfig const& config)
{
	// allow for multiple runs for SA
	char const* filenameDef=config.DefFile();
	ifstream defInFile;
	defInFile.open(filenameDef);
	char dummy[1000];
	defInFile.getline(dummy,1000);
	
	an_.def_.veg_fuel_weighting=config.VegFuelWeighting();
	an_.def_.ignition_col=config.IgnitionCol();
	an_.def_.ndays_average=config.NDaysAverage();
	an_.def_.ignition_row=config.IgnitionRow();
	an_.def_.ignition_tmin=config.IgnitionTMin();
	an_.def_.fire_write=config.FireWrite();
	an_.def_.fire_in_buffer=config.FireInBuffer();
	an_.def_.n_rows=config.NRows();
	an_.def_.n_cols=config.NCols();
	an_.def_.spread_calc_type=config.SpreadCalcType();
	an_.def_.fire_verbose=config.FireVerbose();
	an_.def_.mean_log_wind=config.MeanLogWind();
	an_.def_.sd_log_wind=config.SDLogWind();
	an_.def_.mean1_rvm=config.Mean1RVM();
	an_.def_.mean2_rvm=config.Mean2RVM();
	an_.def_.kappa1_rvm=config.Kappa1RVM();
	an_.def_.kappa2_rvm=config.Kappa2RVM();
	an_.def_.p_rvm=config.PRVM();
	an_.def_.veg_ign=config.VegIgn();
	an_.def_.windmax=config.MaxWind();

	for(int paramSet=0;paramSet<config.NParamRuns(); paramSet++) // loop through the parameter file; if just want MC replicates, copy parameter file Nsim times
	{
		resetFire(config);
//		cout<<"ParamSetloop? "<<paramSet<<"\n\n";
		defInFile>>an_.def_.slope_k1; // k1 for the concave up slope function (k1*exp(k2*slope^2)), where sign(k2) is sign(slope)
		defInFile>>an_.def_.slope_k2; // k2 for the concave up slope function (k1*exp(k2*slope^2)), where sign(k2) is sign(slope)
		defInFile>>an_.def_.winddir_k1; // the modifying constant for wind direction
		defInFile>>an_.def_.winddir_k2; // the modifying constant for wind direction
		defInFile>>an_.def_.load_k1;
		defInFile>>an_.def_.load_k2; // k2 for the fuel load sigmoid function
		defInFile>>an_.def_.moisture_k1; //k1 for the fuel moisture sigmoid function
		defInFile>>an_.def_.moisture_k2; // k2 for the fuel moisture sigmoid function
//		defInFile>>an_.def_.windmax;
		defInFile>>an_.def_.ign_def_mod;
		defInFile>>an_.def_.veg_k1;
		defInFile>>an_.def_.veg_k2;
		defInFile>>an_.def_.mean_ign;
		defInFile>>an_.def_.ran_seed;
		defInFile>>an_.def_.moisture_ign_k1;
		defInFile>>an_.def_.moisture_ign_k2;

		int curYear;
		int curMonth;
		
		// modify loop to do each time series first, then MC reps
		
		for(curYear=config.FirstYear(); curYear<=config.LastYear(); curYear++)
		{
			if(curYear==config.FirstYear())
			{
				for(curMonth=config.FirstMonth(); curMonth<=12;curMonth++)
				{
					resetFire(config);
					saLoop(config,curYear,curMonth,paramSet);
				//	cout<<"First year, in month loop: "<<curYear<<"\t"<<curMonth<<"\t"<<paramSet<<"\n";
				}
			}
			else 
			{
				if(curYear==config.LastYear())
				{
					for(curMonth=1;curMonth<=config.LastMonth(); curMonth++)
					{
						resetFire(config);
						saLoop(config,curYear,curMonth,paramSet);
					}
				}
				else
				{
					for(curMonth=1; curMonth<=12;curMonth++)
					{
						resetFire(config);
						saLoop(config,curYear,curMonth,paramSet);
					}
				}
			}
		}
	}
	defInFile.close();
	freeGrid();	

	return ;
}

/**********************saLoop*********************************/
void Landscape::saLoop(WFConfig const& config, int curYear, int curMonth, int paramSet)
{
	ReadGrids(config,curYear,curMonth); // fill in the grids with the data. Assumes 1 data set for the entire SA

	
	
	double tmpArea;
	//	struct fire_object **tmpFireGrid;
	cout<<"\n\nentering MC sims loop. paramset: "<<paramSet<<"\n";
//	mctest.resetMCSim();
	an_.fires_.clear();
		
	for(int i=0;i<rows_;i++)
	{
		for(int j=0;j<cols_;j++)
		{
			localFireGrid_[i][j].burn=0;
		}
	}
	
		//	for(int i=0; i<config.NumMCSims(); i++)
		//while(successes<config.NumMCSims()&&failures<=config.MaxFailures()) //keep generating fires until we get at least MCSims successes, and give up if we accumulate too many failures 
	for(int fireID=0;fireID<config.NumMCSims();fireID++) //keep generating fires until we get at least MCSims successes, and give up if we accumulate too many failures 
	{
			
	//		cout<<"\n\n\n MC sims loop before WMFire call\n\n\n";
		//	tmpFireGrid=WMFire(fireGrid_,an_.def_,cell_res_,rows_,cols_,1,2012); // invoke the shared library function, update the fire grid
		fireGrid_=WMFire(cell_res_,rows_,cols_,curYear,curMonth,fireGrid_,an_.def_); // invoke the shared library function, update the fire grid
		
		summarizeFire(fireID,config);
//		if(curYear_.update_size>0)
//		{
//			createNewFireYear();
			WriteStopFileContents(paramSet, fireID,curMonth,curYear);
//		}
//		fireID+=1;
	}
	WriteFireGrid(config,curYear,curMonth);
	return ;
}


/***************************************************************************/
/* initialize and allocate the landscape grid						*/
/***************************************************************************/
Landscape::Landscape(WFConfig const& config)
    : rows_(0), cols_(0), buffer_(5), cell_res_(0)
{
	rows_=config.NRows();
	cols_=config.NCols();
	cell_res_=30;
	
	fireGrid_=new fire_object*[rows_];	// first, allocate an array with the # of rows
	for(int i=0;i<rows_;i++) // for each row, allocate the columns
		fireGrid_[i]=new fire_object[cols_];
		
	localFireGrid_=new localFireObject*[rows_];	// first, allocate an array with the # of rows
	for(int i=0;i<rows_;i++) // for each row, allocate the columns
		localFireGrid_[i]=new localFireObject[cols_];
		
	//	cout<<"grid is allocated!\n";
	// now we assign the wind direction to each cell
	for(int i=0;i<rows_;i++)
	{
		for(int j=0;j<cols_;j++)
		{
			fireGrid_[i][j].wind_direction=-1;
			fireGrid_[i][j].wind=-1;
			fireGrid_[i][j].temp=30;
			localFireGrid_[i][j].burn=0;
		}
	}
	return ;
}
/***************************************************************************/
/* create a ew fire year for the analysis							*/
/***************************************************************************/
void Landscape::createNewFireYear()
{
//	cout<<"creating new fire year\n";
//	struct fire_years tmpYear;
	an_.fires_.push_back(curYear_);

	return ;
}


/**************************************************************************************/
/* write the grids for fire occurrence (not perimeters!)		*/
/**************************************************************************************/
void Landscape::WriteFireGrid(WFConfig const& config, int curYear, int curMonth) // to read the data grids, for now just the elevation 1/11/12, will need to add fuels and moistures
{
	std::string curFile;
	
	std::string Month;
	std::stringstream out;
	out<<curMonth;
	Month=out.str();
	
	std::string Year;
	std::stringstream outYear;
	outYear<<curYear;
	Year=outYear.str();

	curFile.assign("FireSpreadNumGridYear");
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");


	ofstream fire_out;

	fire_out.open(curFile.c_str());
	if(fire_out.is_open())
	{
//		for(int i=0;i<6;i++)
//			load_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				fire_out<<localFireGrid_[i][j].burn<<"\t";
//				cout<<fireGrid_[i][j].fuel_litter<<" ";
			}
			fire_out<<"\n";

		}
		fire_out.close();
//		cout<<"read landscape load\n";
	}
	else
		cout<<"can't open fire file\n";

	
	return ;
}


/**************************************************************************************/
/* read in the grids for elevation (dem), fuel loads and fuel moistures			*/
/**************************************************************************************/
void Landscape::ReadGrids(WFConfig const& config, int curYear, int curMonth) // to read the data grids, for now just the elevation 1/11/12, will need to add fuels and moistures
{
	char dummy[1000];
	ifstream land_in;
	land_in.open(config.ElevationFile());
//	cout<<"Elevation file before opened: "<<config.ElevationFile()<<"\n";
	if(land_in.is_open())
	{
//		cout<<"now trying to assign values to grid...\n";
//		for(int i=0;i<6;i++)
//			land_in.getline(dummy,1000); // or something to discard the first six lines
//		cout<<"entering grid loop for dem\n";
		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
//			cout<<"i loop for dem: "<<i<<"\n";
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
//				cout<<"j loop for dem: "<<j<<"\n";
				land_in>>fireGrid_[i][j].z;
//				cout<<fireGrid_[i][j].z<<" ";
			}
		}
		land_in.close();
//		cout<<"read landscape dem\n";
	}
	else
		cout<<"still can't open file grr\n";
// Now read in the fuel loads

	std::string curFile;
//	std::string curPropFile;
	
	std::string Month;
	std::stringstream out;
	out<<curMonth;
	Month=out.str();
	
	std::string Year;
	std::stringstream outYear;
	outYear<<curYear;
	Year=outYear.str();

//	curFile.assign("FireSpreadIterGridYear");
	curFile.assign(config.LoadFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");


	ifstream load_in;

	load_in.open(curFile.c_str());
	if(load_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			load_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				load_in>>fireGrid_[i][j].fuel_litter;
//				cout<<fireGrid_[i][j].fuel_litter<<" ";
			}
		}
		load_in.close();
//		cout<<"read landscape load\n";
	}
	else
		cout<<"can't open load file either\n";

	curFile.assign(config.MoistureFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");

	ifstream moisture_in;
	moisture_in.open(curFile.c_str());

	
	//	moisture_in.open(config.MoistureFile());
	if(moisture_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			moisture_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				moisture_in>>fireGrid_[i][j].fuel_moist;
//				cout<<fireGrid_[i][j].fuel_moist<<" ";
			}
//			fscanf(land_in,"\n");
		}
		moisture_in.close();
//		cout<<"read landscape moisture\n";
	}
	else
		cout<<"can't open moisture file\n";

	curFile.assign(config.EtFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");

	ifstream et_in;
//	cout<<"et file before opened: "<<curFile.c_str()<<"\n";

	et_in.open(curFile.c_str());
//	ifstream et_in;
//	et_in.open(config.EtFile());
	if(et_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			et_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				et_in>>fireGrid_[i][j].et;
//				cout<<fireGrid_[i][j].fuel_moist<<" ";
			}
//			fscanf(land_in,"\n");
		}
		et_in.close();
//		cout<<"read landscape moisture\n";
	}
	else
		cout<<"can't open et file\n";

	curFile.assign(config.UnderstoryEtFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");

	ifstream understory_et_in;
//	cout<<"et file before opened: "<<curFile.c_str()<<"\n";

	understory_et_in.open(curFile.c_str());
//	ifstream et_in;
//	et_in.open(config.EtFile());
	if(understory_et_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			et_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				understory_et_in>>fireGrid_[i][j].understory_et;
//				cout<<fireGrid_[i][j].fuel_moist<<" ";
			}
//			fscanf(land_in,"\n");
		}
		understory_et_in.close();
//		cout<<"read landscape moisture\n";
	}
	else
		cout<<"can't open understory et file\n";

	
	curFile.assign(config.PetFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");

	ifstream pet_in;
	pet_in.open(curFile.c_str());	
	//ifstream pet_in;
	//pet_in.open(config.PetFile());
	if(pet_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			pet_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				pet_in>>fireGrid_[i][j].pet;
				if(fireGrid_[i][j].pet==1000)
					fireGrid_[i][j].ign_available=0;
				else
					fireGrid_[i][j].ign_available=1;
			//	cout<<fireGrid_[i][j].pet<<" ";
			}
//			fscanf(land_in,"\n");
		}
		pet_in.close();
//		cout<<"read landscape moisture\n";
	}
	else
		cout<<"can't open pet file\n";

	curFile.assign(config.UnderstoryPetFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");

	ifstream understory_pet_in;
	understory_pet_in.open(curFile.c_str());	
	//ifstream pet_in;
	//pet_in.open(config.PetFile());
	if(understory_pet_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			pet_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				understory_pet_in>>fireGrid_[i][j].understory_pet;
				if(fireGrid_[i][j].understory_pet==1000)
					fireGrid_[i][j].ign_available=0;
				else
					fireGrid_[i][j].ign_available=1;
			//	cout<<fireGrid_[i][j].pet<<" ";
			}
//			fscanf(land_in,"\n");
		}
		understory_pet_in.close();
//		cout<<"read landscape moisture\n";
	}
	else
		cout<<"can't open understory pet file\n";

	curFile.assign(config.VegFile());
	curFile.append(Year);
	curFile.append("Month");
	curFile.append(Month);
	curFile.append(".txt");

//	cout<<"veg file before opened: "<<curFile.c_str()<<"\n";
	ifstream veg_in;
	veg_in.open(curFile.c_str());	
	//ifstream pet_in;
	//pet_in.open(config.PetFile());
	if(veg_in.is_open())
	{
//		for(int i=0;i<6;i++)
//			pet_in.getline(dummy,1000); // or something to discard the first three lines

		for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
		{
			for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
			{
				veg_in>>fireGrid_[i][j].fuel_veg;
//				cout<<fireGrid_[i][j].fuel_moist<<" ";
			}
//			fscanf(land_in,"\n");
		}
		veg_in.close();
//		cout<<"read landscape moisture\n";
	}
	else
		cout<<"can't open veg file\n";

	return ;
}


/**********************************************************************/
/* reset the fire grid each fire								*/
/**********************************************************************/
void Landscape::resetFire(WFConfig const& config)	// just to fill in the raster fire object.  Called when the Raster grid is pre-processed
{
	for(int i=0; i<rows_; i++)	//then, for each row, allocate an array with the # of columns.  this is now a 2-D array of fireGrids
	{
		for(int j=0; j<cols_; j++)	// fill in the landscape information for each pixel
		{
			fireGrid_[i][j].burn=0;		// 0 indicates that the pixel has not been burned
		}
	}
	return ;
}



/****************************************************************/
/* free the grid										*/
/****************************************************************/
void Landscape::freeGrid()
{
/*	for(int i=0; i<rows_;i++)
		free(fireGrid_[i]);
	free(fireGrid_);*/
	
	for(int i=0;i<rows_;i++)
		delete fireGrid_[i];
	delete fireGrid_;
	return ;
}

/************************************************************/
/* summarize each fire year						*/
/************************************************************/
void Landscape::summarizeFire(int year, WFConfig const& config)
{
	// first reset the current year
	curYear_.year=year;
	curYear_.update_size=0;

	for(int i=0;i<rows_;i++)
	{
		for(int j=0;j<cols_;j++)
		{
			if(fireGrid_[i][j].burn>0)
			{
				curYear_.update_size+=1;
				localFireGrid_[i][j].burn+=1; // tally how many times this pixel burns across the MC sims
			}
		}
	}
	
	cout<<"FireID and size: "<<year<<"\t"<<curYear_.update_size<<"\t";
	
	return ;
}

/******************************WriteStopFileContents*********************************/
/* writes the fire sizes and statistics for each fire								*/
/* record the stopping rule, which is 1 if reaches fire size,						*/
/* 2 if fails all burned tests, and 3 if reaches all 4 borders before fire size		*/ 
/************************************************************************************/
void Landscape::WriteStopFileContents(int paramSet,int run , int curMonth, int curYear)
{
	ofstream stopFile;
	stopFile.open("StopOut.txt", ofstream::app);
//	assert(stop_out);
//	if(stop_out)
//	{
//	for(size_t i = 0; i < an_.fires_.size(); ++i)
//	{
		stopFile<<paramSet<<"\t"<<run<<"\t"<<curYear<<"\t"<<curMonth<<"\t"<<curYear_.update_size<<"\n";//<<an_.fires_[i].fd<<"\t"<<an_.fires_[i].lacunarity<<"\t"<<an_.fires_[i].numUnburnedPatches<<"\n";
//		stopFile<<paramSet<<"\t"<<run<<"\t"<<curYear<<"\t"<<curMonth<<"\t"<<an_.fires_[i].update_size<<"\n";//<<an_.fires_[i].fd<<"\t"<<an_.fires_[i].lacunarity<<"\t"<<an_.fires_[i].numUnburnedPatches<<"\n";

//	}
	stopFile.close();
//	}
	return ;	
}

/***********************setUpFiles*****************************************/
void setUpFiles()
{
	ofstream stopFile;
	stopFile.open("StopOut.txt");
	stopFile<<"ParamSet\tRep\tYear\tMonth\tFireSize\n";//\tFD\tlac\tnumUnburnedPatches\n";
	stopFile.close();

	ofstream sizesFile;
	sizesFile.open("FireSizes.txt");
	sizesFile<<"FireSize\tYear\tMonth\twindir\twindspeed\n";
	sizesFile.close();


	return ;
}
