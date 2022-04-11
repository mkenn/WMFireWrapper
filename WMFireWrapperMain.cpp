/***************************my wrapper*************************/
/* a wrapper for the WMFire shared library for testing 		*/
/****************************************************************/

#include <cstdio>
#include <ctime>
#include <iostream>
//#include "../util/rhessys_fire.h"
//#include "WMFire.h"
#include "WFConfig.h"
//#include "../util/WMFireInterface.h"
#include "Wrapper.h"
// need the header to create the interface

using std::cout;

void Usage()
{
    cout << "\nA win32 executable to execute WMFire\n";
    cout << "In current implementation, generates random fires on a square\n";
    cout << "window.  Fires are defined by a box with a random size, and\n";
    cout << "centered on a randomly generated ignition point in the window\n";
    cout << "\nPass a configuration file of the following format as the only\n";
    cout << "argument.\n\n";
    PrintWFConfigFile();        
}

int main(int cArgs, char* argVector[])
{
	if(2 == cArgs)
	{
	//	if(FileExists(argVector[1]))
	//	{
//			printf("in main, after if FileExists\n");
			WFConfig config(argVector[1]); // mk: declares the config argument, the constructor for config (so, reads the config xml file)
			Landscape landscape(config); // This just reads in the file header to set the dimensions of the grid
			setUpFiles();
			landscape.Wrapper(config);
	//	}
        }        
/*	else
        {
            cout << "\nError: The configuration file \"" << argVector[1] << " does not exist!!!\n";
        }
    }*/
    else // mk: if the executable is called without the name of the configuration file, then report the usage.
    {
        Usage();
    }
    

	// first we read in the grid data and create the grid
	return 0;
}
