/***********************************************************/
/* Wrapper.h									*/
/* includes data structures for the WMFire wrapper	*/
/* for testing the shared library					*/
/***********************************************************/
#pragma once

#include "../util/WMFireInterface.h"
#include "WFConfig.h"
#include "boost/multi_array.hpp"


typedef boost::multi_array<fire_object, 2> FireGrid;

void setUpFiles();


// keeps ids and information about unburned patches
/*struct unburnedPatch // keeps ids and information about unburned patches
{
	int label;
	double size;
//	unburnedPatch() { ZeroMem(this); }
};
*/

struct localFireObject
{
	double burn;
//	int isPerim;
//	int patchLabel;	
};
typedef boost::multi_array<localFireObject, 2> LocalFireGrid;

struct fire_years
{
	double year;	//which year is this
	double update_size;	// what is the size of the fire that is being burned, to be updated each iteration and be tested against size
//	double fd; // fractal dimension, calculated in RasterStats
//	double lacunarity; // lacunarity, calculated in RasterStats
//	double meanKappa;
//	int numUnburnedPatches;

//	std::vector<unburnedPatch >patches; // mk added 2/17/12 to account for the distribution of unburned patches in the landscape
//	struct localFireObject ** curGrid;
 //   fire_years() { ZeroMem(this); }
};
//typedef std::vector<fire_years> Fires;



/************************************************************/
/* Analysis structure										*/
/*															*/
/* contains information for an the current analysis			*/
/************************************************************/
struct analysis
{
	int numFires;	    // the number of fires to be generated for the current pattern
	fire_default def_;
	std::vector<fire_years> fires_; // I think I need to separate the fire_years from the landscape...
};



class Landscape
{
public:
	Landscape() : rows_(0), cols_(0), buffer_(5), cell_res_(0) 
	{
	}
	Landscape(WFConfig const& config);
	int Rows() const { return rows_; }
	int Cols() const { return cols_; }
	int BufferValue() const { return buffer_; }
	double CellResolution() const { return cell_res_; }
	void Wrapper(WFConfig const& config);
	void read_def(WFConfig const& config);


 	struct fire_object **& FireGrids() { return fireGrid_; }
 	struct localFireObject **& LocalFireGrids() { return localFireGrid_; }
//	fire_default& FireDefault() {return def_;}
	analysis& Analyses() {return an_;}

	
private:
	int rows_;    // to record the number of rows and columns in the landscape
	int cols_; 
	int buffer_;     // how many cells from edge will ignitions not be allowed?
	double cell_res_; // the resolution of each pixel, so the area of a pixel is cell_res_^2
//	double lenPerim; // the length of the fire perimeter (m)
	int borders_[4];
//	int maxRow_; // identify the x and y coordinates that represent a box around the fire perimeter
//	int maxCol_;
//	int minRow_;
//	int minCol_;
	int successes;
//	FireGrid fireGrid_;	// 2-D array of pixels for the current landscape, FireNodes above
	struct fire_object **fireGrid_;
	struct localFireObject **localFireGrid_;
//	struct localFireObject **obsFireGrid_;
	struct fire_years curYear_;
//	struct fire_years obsYear_;
	
	std::vector<int> patchLabels_;
	struct analysis an_;
	
	void resetFire(WFConfig const& config);
	void ReadGrids(WFConfig const& config, int curYear, int curMonth);
	void WriteFireGrid(WFConfig const& config, int curYear, int curMonth);
	void saLoop(WFConfig const& config, int curYear, int curMonth, int paramSet);
//	void writeFire(int curYear, int curParam);
	void freeGrid();
//	void freeCurParam();
//	void idPerimeter();
//	void findUnburnedPatches(fire_years&);
	void summarizeFire(int year,WFConfig const& config);
//	void writeFailedCrits(WFConfig const& config, int paramSet);
//	void writeStatCrits(WFConfig const& config, int paramSet);
	void WriteStopFileContents(int run, int curYear, int curMonth, int paramSet);
//	double calcFractalDim(double area, double perimeter); // calculated from the perimeter (m) and area (ha) of the lansdcape
//	double calcLacunarity(double boxSize); // this requires navigating the grid with a moving window, but the argument should probably
//	void calcMeanKappa(WFConfig const& config);
//	double calcKappa(int curSol, int compSol);
	void createNewFireYear();



};