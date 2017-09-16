/** 
* @version	1.1
* @author	Junzhi Liu
* @date	5-January-2011
* @revised LiangJun Zhu
* @date 27-May-2016
* @brief	Finn Plauborg Method to Compute Soil Temperature
*
*/
#pragma once

#include <visit_struct/visit_struct.hpp>
#include "SimulationModule.h"

using namespace std;
/*!
 * \defgroup STP_FP
 * \ingroup Hydrology_longterm
 * \brief Finn Plauborg Method to Compute Soil Temperature
 *
 */

/*!
 * \class SoilTemperatureFINPL
 * \ingroup STP_FP
 * \brief Soil temperature
 * 
 */
class SoilTemperatureFINPL : public SimulationModule {
public:
    //! Constructor
    SoilTemperatureFINPL(void);

    //! Destructor
    ~SoilTemperatureFINPL(void);

    virtual void SetValue(const char *key, float value);

    virtual void Set1DData(const char *key, int n, float *data);

    virtual void Get1DData(const char *key, int *n, float **data);

    virtual int Execute(void);

    /// from parameter database

    // @Parameter
    // @Description coefficients in the Equation
    // @Unit none
    // @Dimension SINGLE
    // @Source ParameterDB
    float soil_ta0, soil_ta1, soil_ta2, soil_ta3, soil_tb1, soil_tb2, soil_td1, soil_td2;

    // @Parameter
    // @Description ratio between soil temperature at 10 cm and the mean
    // @Unit none
    // @Dimension SINGLE
    // @Source ParameterDB
    float k_soil10;
 
    // @Parameter
    // @Description count of cells
    // @Unit none
    // @Dimension SINGLE
    // @Source ParameterDB
    int m_nCells;

    // @Parameter
    // @Description factor of soil temperature relative to short grass (degree
    // @Unit none
    // @Dimension RASTER1D
    // @Source ParameterDB
    float *soil_t10;

    // @Parameter
    // @Description landuse type, for distinguish calculation, such as water body.
    // @Unit none
    // @Dimension RASTER1D
    // @Source ParameterDB
    float *landuse;

    /// from interpolation module

    // @In
    // @Description mean air temperature of the current day
    // @Unit deg C
    // @Dimension RASTER1D
    // @Source ParameterDB
    float *TMEAN;

    // @Out
    // @Description mean air temperature of the day(d-1)
    // @Unit deg C
    // @Dimension RASTER1D
    float *TMEAN1;

    // @Out
    // @Description mean air temperature of the day(d-2)
    // @Unit deg C
    // @Dimension RASTER1D
    float *TMEAN2;

    // @Out
    // @Description output soil temperature
    // @Unit deg C
    // @Dimension RASTER1D
    float *SOTE;

private:

    // Julian day
    int julianDay;

	/// temporary variable
	float w;
    /*!
     * \brief Initialize output variables for the first run of the entire simulation
     */
    void initialOutputs(void);

private:
    /**
    *	@brief check the input data. Make sure all the input data is available.
    *
    *	@return bool The validity of the input data.
    */
    bool CheckInputData(void);

    /**
    *	@brief check the input size. Make sure all the input data have same dimension.
    *
    *	@param key The key of the input data
    *	@param n The input data dimension
    *	@return bool The validity of the dimension
    */
    bool CheckInputSize(const char *, int);
};

VISITABLE_STRUCT(SoilTemperatureFINPL, m_nCells, soil_ta0, soil_ta1, soil_ta2, soil_ta3, soil_tb1, soil_tb2, soil_td1, soil_td2,
	k_soil10, soil_t10, landuse, TMEAN, TMEAN1, TMEAN2, SOTE);