#include "seims.h"
#include "ClimateParams.h"

#include "SoilTemperatureFINPL.h"

using namespace std;

SoilTemperatureFINPL::SoilTemperatureFINPL(void) : soil_ta0(NODATA_VALUE), soil_ta1(NODATA_VALUE), soil_ta2(NODATA_VALUE),
                                                   soil_ta3(NODATA_VALUE),
                                                   soil_tb1(NODATA_VALUE), soil_tb2(NODATA_VALUE), soil_td1(NODATA_VALUE),
                                                   soil_td2(NODATA_VALUE),
                                                   k_soil10(NODATA_VALUE), julianDay(-1), m_nCells(-1),
                                                   landuse(NULL),
                                                   soil_t10(NULL), SOTE(NULL), TMEAN(NULL), TMEAN1(NULL),
                                                   TMEAN2(NULL) {
    w = PI * 2.f / 365.f;
}

SoilTemperatureFINPL::~SoilTemperatureFINPL(void) {
    if (SOTE != NULL) Release1DArray(SOTE);
    if (TMEAN1 != NULL) Release1DArray(TMEAN1);
    if (TMEAN2 != NULL) Release1DArray(TMEAN2);
}

int SoilTemperatureFINPL::Execute() {
    /// check the data
    CheckInputData();
    /// initial output of TMEAN1 and TMEAN2 for the first run
    initialOutputs();
    julianDay = JulianDay(m_date);
    size_t errCount = 0;
#pragma omp parallel for reduction(+: errCount)
    for (int i = 0; i < m_nCells; ++i) {
        float t = TMEAN[i];
        float t1 = TMEAN1[i];
        float t2 = TMEAN2[i];
        if ((t > 60.f || t < -90.f) || (t1 > 60.f || t1 < -90.f) || (t2 > 60.f || t2 < -90.f)) {
            cout << "cell index: " << i << ", t1: " << t1 << ", t2: " << t2 << endl;
            errCount++;
        }
        else{
            if (FloatEqual((int) landuse[i], LANDUSE_ID_WATR)) {
                /// if current landuse is water
                SOTE[i] = t;
            } else {
                float t10 = soil_ta0 + soil_ta1 * t2 + soil_ta2 * t1 + soil_ta3 * t
                    + soil_tb1 * sin(w * julianDay) + soil_td1 * cos(w * julianDay)
                    + soil_tb2 * sin(2.f * w * julianDay) + soil_td2 * cos(2.f * w * julianDay);
                SOTE[i] = t10 * k_soil10 + soil_t10[i];
                if (SOTE[i] > 60.f || SOTE[i] < -90.f) {
                    cout << "The calculated soil temperature at cell (" << i
                        << ") is out of reasonable range: " << SOTE[i]
                        << ". JulianDay: " << julianDay << ",t: "<< t << ", t1: " 
                        << t1 << ", t2: " << t2 << ", relativeFactor: " << soil_t10[i] << endl;
                    errCount++;
                }
            }
            //save the temperature
            TMEAN2[i] = TMEAN1[i];
            TMEAN1[i] = t;
        }
    }
    if (errCount > 0) {
        throw ModelException(MID_STP_FP, "Execute", "The calculation of soil temperature failed!");
    }
    return 0;
}

bool SoilTemperatureFINPL::CheckInputData(void) {
    if (FloatEqual(soil_ta0, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTa0 has not been set.");
    }
    if (FloatEqual(soil_ta1, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTa1 has not been set.");
    }
    if (FloatEqual(soil_ta2, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTa2 has not been set.");
    }
    if (FloatEqual(soil_ta3, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTa3 has not been set.");
    }
    if (FloatEqual(soil_tb1, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTb1 has not been set.");
    }
    if (FloatEqual(soil_tb2, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTb2 has not been set.");
    }
    if (FloatEqual(soil_td1, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTd1 has not been set.");
    }
    if (FloatEqual(soil_td2, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: SoilTd2 has not been set.");
    }
    if (FloatEqual(k_soil10, NODATA_VALUE)) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: KSoil10 has not been set.");
    }
    if (m_date < 0) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The date has not been set.");
    }
    if (soil_t10 == NULL) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: LandcoverFactor has not been set.");
    }
    if (TMEAN == NULL) {
        throw ModelException(MID_STP_FP, "CheckInputData", "The parameter: mean air temperature has not been set.");
    }
    //if (TMEAN1 == NULL) {
    //    throw ModelException(MID_STP_FP, "CheckInputData",
    //                         "The parameter: mean air temperature of (d-1) day has not been set.");
    //}
    //if (TMEAN2 == NULL) {
    //    throw ModelException(MID_STP_FP, "CheckInputData",
    //                         "The parameter: mean air temperature of (d-2) day has not been set.");
    //}
    if (landuse == NULL) {
        throw ModelException(MID_STP_FP, "CheckInputData",
                             "The parameter: landuse type has not been set.");
    }
    return true;
}

bool SoilTemperatureFINPL::CheckInputSize(const char *key, int n) {
    if (n <= 0) {
        throw ModelException(MID_STP_FP, "CheckInputSize",
                             "Input data for " + string(key) + " is invalid. The size could not be less than zero.");
    }
    if (this->m_nCells != n) {
        if (this->m_nCells <= 0) { this->m_nCells = n; }
        else {
            throw ModelException(MID_STP_FP, "CheckInputSize", "Input data for " + string(key) +
                " is invalid. All the input raster data should have same size.");
        }
    }
    return true;
}

void SoilTemperatureFINPL::SetValue(const char *key, float value) {
    string sk(key);
    if (StringMatch(sk, VAR_OMP_THREADNUM)) { SetOpenMPThread((int) value); }
    else if (StringMatch(sk, VAR_SOL_TA0)) { soil_ta0 = value; }
    else if (StringMatch(sk, VAR_SOL_TA1)) { soil_ta1 = value; }
    else if (StringMatch(sk, VAR_SOL_TA2)) { soil_ta2 = value; }
    else if (StringMatch(sk, VAR_SOL_TA3)) { soil_ta3 = value; }
    else if (StringMatch(sk, VAR_SOL_TB1)) { soil_tb1 = value; }
    else if (StringMatch(sk, VAR_SOL_TB2)) { soil_tb2 = value; }
    else if (StringMatch(sk, VAR_SOL_TD1)) { soil_td1 = value; }
    else if (StringMatch(sk, VAR_SOL_TD2)) { soil_td2 = value; }
    else if (StringMatch(sk, VAR_K_SOIL10)) k_soil10 = value;
}

void SoilTemperatureFINPL::Set1DData(const char *key, int n, float *data) {
    //check the input data
    CheckInputSize(key, n);
    string sk(key);
    if (StringMatch(sk, VAR_SOIL_T10)) {
        this->soil_t10 = data;
    } else if (StringMatch(sk, DataType_MeanTemperature)) {
        this->TMEAN = data;
    //} else if (StringMatch(sk, VAR_TMEAN1)) {
    //    this->TMEAN1 = data;
    //} else if (StringMatch(sk, VAR_TMEAN2)) {
    //    this->TMEAN2 = data;
    } else if (StringMatch(sk, VAR_LANDUSE)) {
        this->landuse = data;
    } else {
        throw ModelException(MID_STP_FP, "Set1DData", "Parameter " + sk + " does not exist.");
    }
}

void SoilTemperatureFINPL::Get1DData(const char *key, int *n, float **data) {
    initialOutputs();
    string sk(key);
    *n = m_nCells;
    if (StringMatch(sk, VAR_SOTE)) { *data = SOTE; }
    else if (StringMatch(sk, VAR_TMEAN1)) { *data = TMEAN1; }
    else if (StringMatch(sk, VAR_TMEAN2)) { *data = TMEAN2; }
    else {
        throw ModelException(MID_STP_FP, "Get1DData", "Parameter " + sk + " does not exist in current module.");
    }
}

void SoilTemperatureFINPL::initialOutputs() {
    if (this->m_nCells <= 0) {
        throw ModelException(MID_STP_FP, "initialOutputs", "The cell number of the input can not be less than zero.");
    }
    // initialize TMEAN1 and TMEAN2 as TMEAN
    if (TMEAN1 == NULL && TMEAN != NULL) {
        Initialize1DArray(m_nCells, TMEAN1, TMEAN);
    }
    if (TMEAN2 == NULL && TMEAN != NULL) {
        Initialize1DArray(m_nCells, TMEAN2, TMEAN);
    }
    if (this->SOTE == NULL) {
        Initialize1DArray(m_nCells, SOTE, 0.f);
    }
}