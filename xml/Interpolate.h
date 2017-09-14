/*!
 * \brief Interpolate function for site based data, e.g. precipitation
 * \author Junzhi Liu, LiangJun Zhu
 * \date Jan. 2010
 * \revised date Apr. 2016
 */
#pragma once

#include <visit_struct/visit_struct.hpp>
#include "SimulationModule.h"

using namespace std;
/** \defgroup ITP
 * \ingroup Climate
 * \brief Interpolation Module
 *  
 */
/*!
 * \class Interpolate
 * \ingroup ITP
 *
 * \brief Interpolation
 *
 */
class Interpolate : public SimulationModule {
public:
    //! Constructor
    Interpolate(void);

    //! Destructor
    ~Interpolate(void);

    //! Set data type
    void SetClimateDataType(float value);

    int Execute(void);

    void SetDate(time_t date, int yearIdx);

    void SetValue(const char *key, float value);

    void Set1DData(const char *key, int n, float *value);

    void Set2DData(const char *key, int nRows, int nCols, float **data);

    void Get1DData(const char *key, int *n, float **data);

    /*!
     * \brief Check length of the input variable
     * \param[in] key the key to identify the requested data
     * \param[in] n size of the input 1D data
     * \param[out] m_n the corresponding member variable of length
     */
    bool CheckInputSize(string &key, int n, int &m_n);

    void CheckInputData(void);

    // @In
    // @Description count of valid cells
    int m_nCells;

    // @In
    // @Description count of stations
    int m_nStatioins;

    // @In
    // @Description data of stations
    float *T;

    // @In
    // @Description weights of each sites of all valid cells
    float *WEIGHT;

    // @In
    // @Description whether using vertical interpolation
    bool VERTICALINTERPOLATION;

    // @In
    // @Description elevation of stations
    float *StationElevation;

    // @In
    // @Description elevation of cells
    float *DEM;

    // @In
    // @Description Lapse Rate, a 2D array. The first level is by month, and the second level is by data type in order of (P,T,PET).
    float **LapseRate;

    // @Out
    // @Description interpolation result
    float *D;


private:
	/// month
	int m_month;
	// This is the climate data type. It is used to get the specific lapse rate from lapse_rate table.
	// It is also used to create a string which can match the output id.
	// For example, if data_type = 1, i.e. the data type is P, main program will connect the output variable name "D"
	// and the data type to create a string like D_P,
	// this string is the same with the output id in the output lookup table and file.out.
	int m_dataType;   
};


VISITABLE_STRUCT(Interpolate, m_nCells, m_nStatioins, T, WEIGHT, VERTICALINTERPOLATION, StationElevation, DEM, LapseRate, D);