/*!
 * \brief Read hydroclimate data
 * \author Zhiqiang Yu
 * \date Apr. 2010
 */
#pragma once

#include <visit_struct/visit_struct.hpp>
#include "SimulationModule.h"

using namespace std;
/** \defgroup TSD_RD
 * \ingroup Climate
 * \brief Read Time Series Data
 */
/*!
 * \class clsTSD_RD
 * \ingroup TSD_RD
 * \brief Read Time Series Data, e.g., Maximum temperature.
 */
class clsTSD_RD : public SimulationModule {
public:
	// @Parameter
    // @Description data row number
    // @Unit none
    // @Dimension SINGLE
	// @Source HydroClimateDB
    int numSites;

	// @Out
    // @Description time series data
    // @Unit none
    // @Dimension RASTER1D
	// @Source HydroClimateDB
    float *T;

    clsTSD_RD(void);

    ~clsTSD_RD(void);

    void Set1DData(const char *key, int n, float *data);

    void Get1DData(const char *key, int *n, float **data);
};

