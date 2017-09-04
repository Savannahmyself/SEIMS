/*!
 * \brief Potential evapotranspiration using PriestleyTaylor method
 * \author Junzhi Liu
 * \date Nov. 2010
 * \revised LiangJun Zhu
 * \date May. 2016
 * \note:     1. Add TMEAN from database, which may be measurement value or the mean of tMax and tMin;
			  2. The PET calculate is changed from site-based to cell-based, because PET is not only dependent on Climate site data;
			  3. Add m_VPD, daylength as outputs, which will be used in BIO_EPIC module
			  4. Add PHUBASE as outputs, which will be used in MGT_SWAT module
 */
#pragma once

#include <visit_struct/visit_struct.hpp>
#include "SimulationModule.h"

using namespace std;
/*!
 * \defgroup PET_PT
 * \ingroup Hydrology_longterm
 * \brief Calculate potential evapotranspiration using PriestleyTaylor method
 *
 */
/*!
 * \class PETPriestleyTaylor
 * \ingroup PET_PT
 *
 * \brief Priestley Taylor Method to Compute PET
 *
 */
class PETPriestleyTaylor : public SimulationModule {
public:
    //! Constructor
    PETPriestleyTaylor(void);

    //! Destructor
    ~PETPriestleyTaylor(void);

    virtual void SetValue(const char *key, float value);

    virtual void Set1DData(const char *key, int n, float *value);

    virtual void Get1DData(const char *key, int *n, float **data);

    virtual int Execute(void);

private:

    /*!
     * \brief check the input data. Make sure all the input data is available.
     * \return bool The validity of the input data.
     */
    bool CheckInputData(void);

    /*!
     * \brief check the input size. Make sure all the input data have same dimension.
     *
     *
     * \param[in] key The key of the input data
     * \param[in] n The input data dimension
     * \return bool The validity of the dimension
     */
    bool CheckInputSize(const char *, int);

    //! Initialize of output variables
    void initialOutputs(void);

    // @In
    // @Description mean air temperature for a given day(degree)
    float *TMEAN;

    // @In
    // @Description maximum air temperature for a given day(degree)
    float *TMAX;

    // @In
    // @Description minimum air temperature for a given day(degree)
    float *TMIN;

    // @In
    // @Description solar radiation(MJ/m2/d)
    float *SR;

    // @In
    // @Description relative humidity(%)
    float *RM;

    // @In
    // @Description elevation(m)
    float *DEM;

    // @In
    // @Description valid cells number
    int m_nCells;

    // @In
    // @Description Correction Factor for PET
    float K_pet;

    // @In
    // @Description latitude of the stations
    float *celllat;

    // @In
    // @Description annual PHU
    float *PHU0;

    // @In
    // @Description The temperature of snow melt
    float T_snow;

    // @In
    // @Description maximum solar radiation of current day
    float srMax;

    // @In
    // @Description Julian day
    int jday;

    /// output

    // @Out
    // @Description day length (hr)
    float *daylength;

    // @Out
    // @Description base zero total heat units (used when no land cover is growing)
    float *PHUBASE;

    // @Out
    // @Description pet
    float *PET;

    // @Out
    // @Description vapor pressure deficit
    float *VPD;
};

VISITABLE_STRUCT(PETPriestleyTaylor, m_nCells, TMEAN, TMAX, TMIN, SR, RM, DEM, K_pet, celllat, PHU0, T_snow,
	srMax, jday, daylength, PHUBASE, PET, VPD);
