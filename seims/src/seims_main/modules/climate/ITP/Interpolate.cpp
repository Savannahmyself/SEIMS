#include "seims.h"
#include "Interpolate.h"

using namespace std;

Interpolate::Interpolate() : m_nCells(-1), m_nStatioins(-1),
                             m_month(-1), m_itpOutput(NULL), m_stationData(NULL), m_weights(NULL),
                             m_dem(NULL), m_hStations(NULL), m_lapseRate(NULL), m_vertical(false), m_dataType(0) {
}

void Interpolate::SetClimateDataType(float value) {
    if (FloatEqual(value, 1.0f)) {
        m_dataType = 0; /// Precipitation
    } else if (FloatEqual(value, 2.0f) || FloatEqual(value, 3.0f) || FloatEqual(value, 4.0f)) {
        m_dataType = 1; /// Temperature
    } else if (FloatEqual(value, 5.0f)) {
        m_dataType = 2; /// PET
    } else if (FloatEqual(value, 6.0f) || FloatEqual(value, 7.0f) || FloatEqual(value, 8.0f)) {
        m_dataType = 3;
    } /// Meteorology
}

Interpolate::~Interpolate(void) {
    if (m_itpOutput != NULL) Release1DArray(m_itpOutput);
};

int Interpolate::Execute() {
    CheckInputData();
    if (m_itpOutput == NULL) {
        Initialize1DArray(m_nCells, m_itpOutput, 0.f);
    }

    //cout<<"ITP: ";
    //for (int j = 0; j < m_nStatioins; ++j)
    //	cout<<m_stationData[j]<<",";
    //cout<<endl;
    size_t errCount = 0;
#pragma omp parallel for reduction(+: errCount)
    for (int i = 0; i < m_nCells; ++i) {
        int index = 0;
        float value = 0.f;
        //for (int j = 0; j < m_nStatioins; ++j){
        //	index = i * m_nStatioins + j;
        //	cout<<m_weights[index]<<",";
        //}
        //cout<<endl;
        for (int j = 0; j < m_nStatioins; ++j) {
            index = i * m_nStatioins + j;
            value += m_stationData[j] * m_weights[index];
            //cout<<m_stationData[j]<<",";
            //cout<<endl;
            if (value != value) {
                errCount++;
                cout << "CELL:" << i << ", Site: " << j << ", Weight: " << m_weights[index] <<
                    ", siteData: " << m_stationData[j] << ", Value:" << value << ";" << endl;
            }

            if (m_vertical) {
                float delta = m_dem[i] - m_hStations[j];
                float factor = m_lapseRate[m_month][m_dataType];
                float adjust = m_weights[index] * delta * factor / 100.f;
                value += adjust;
            }
        }
        m_itpOutput[i] = value;
    }
    if (errCount > 0) {
        throw ModelException(MID_ITP, "Execute", "Error occurred in weight data!\n");
    }
    //for (int i = 0; i < m_nCells; ++i)
    //	cout<<m_itpOutput[i]<<",";
    //Output1DArrayToTxtFile(m_nCells, m_itpOutput, "itp.txt");
    return true;
}

void Interpolate::SetDate(time_t date, int yearIdx) {
    m_date = date;
    m_yearIdx = yearIdx;
    struct tm t;
    LocalTime(date, &t);
    m_month = t.tm_mon;
}

void Interpolate::SetValue(const char *key, float value) {
    string sk(key);

    if (StringMatch(sk, VAR_OMP_THREADNUM)) {
        SetOpenMPThread((int) value);
    } else if (StringMatch(sk, VAR_TSD_DT)) {
        this->SetClimateDataType(value);
        //if (value == 1.0f) m_dataType = 0;
        //if (value == 2.0f || value == 3.0f || value == 4.0f) m_dataType = 1;
        //if (value == 5.0f) m_dataType = 2;
    } else if (StringMatch(sk, Tag_VerticalInterpolation)) {
        if (value > 0) {
            m_vertical = true;
        } else {
            m_vertical = false;
        }
    } else {
        throw ModelException(MID_ITP, "SetValue", "Parameter " + sk + " does not exist.");
    }

}

void Interpolate::Set2DData(const char *key, int nRows, int nCols, float **data) {
    string sk(key);

    if (StringMatch(sk, Tag_LapseRate)) {
        if (m_vertical) {
            int nMonth = 12;
            CheckInputSize(sk, nRows, nMonth);
            m_lapseRate = data;
        }
    } else {
        throw ModelException(MID_ITP, "Set2DData", "Parameter " + sk + " does not exist.");
    }
}

void Interpolate::Set1DData(const char *key, int n, float *data) {
    string sk(key);

    if (StringMatch(sk, Tag_DEM)) {
        if (m_vertical) {
            CheckInputSize(sk, n, m_nCells);
            m_dem = data;
        }
    } else if (StringMatch(sk, Tag_Weight)) {
        CheckInputSize(sk, n, m_nCells);
        m_weights = data;
    } else if (StringMatch(sk, Tag_Elevation_Precipitation) || StringMatch(sk, Tag_Elevation_Meteorology)
        || StringMatch(sk, Tag_Elevation_Temperature) || StringMatch(sk, Tag_Elevation_PET)) {
        if (m_vertical) {
            CheckInputSize(sk, n, m_nStatioins);
            m_hStations = data;
        }
    } else {
        string prefix = sk.substr(0, 1);
        if (StringMatch(prefix, DataType_Prefix_TS)) {
            CheckInputSize(sk, n, m_nStatioins);
            this->m_stationData = data;
            //for (int i = 0; i < n; i++)
            //	cout << m_stationData[i] << "\t";
            //cout << endl;
        } else {
            throw ModelException(MID_ITP, "Set1DData", "Parameter " + sk + " does not exist.");
        }
    }
}

bool Interpolate::CheckInputSize(string &key, int n, int &m_n) {
    if (n <= 0) {
        throw ModelException(MID_ITP, "CheckInputSize", "Input data for " + key
            + " is invalid. The size could not be less than zero.");
    }
    if (n != m_n) {
        if (m_n <= 0) {
            m_n = n;
        } else {
            throw ModelException(MID_ITP, "CheckInputSize",
                                 "Input data for " + key + " is invalid." + " The size of input data is " +
                                     ValueToString(n) +
                                     ". The number of columns in weight file and the number of stations should be same.");
        }
    }
    return true;
}

void Interpolate::CheckInputData() {
    if (m_dataType < 0) {
        throw ModelException(MID_ITP, "CheckInputData", "The parameter: Climate DataType has not been set.");
    }
    if (m_month < 0) {
        throw ModelException(MID_ITP, "CheckInputData", "The date has not been set.");
    }
    if (m_weights == NULL) {
        throw ModelException(MID_ITP, "CheckInputData", "The parameter: Weight has not been set.");
    }
    if (m_vertical) {
        if (m_lapseRate == NULL) {
            throw ModelException(MID_ITP, "CheckInputData", "The parameter: LapseRate has not been set.");
        }
        if (m_dem == NULL) {
            throw ModelException(MID_ITP, "CheckInputData", "The parameter: DEM has not been set.");
        }
        if (m_hStations == NULL) {
            throw ModelException(MID_ITP, "CheckInputData", "The parameter: StaionElevation have not been set.");
        }
    }
    if (m_stationData == NULL) {
        throw ModelException(MID_ITP, "CheckInputData", "The parameter: Climate data has not been set.");
    }

}

void Interpolate::Get1DData(const char *key, int *n, float **data) {
    string sk(key);

    if (StringMatch(sk, Tag_DEM)) {
        *n = m_nCells;
        *data = m_dem;
    } else if (StringMatch(sk, Tag_StationElevation)) {
        *n = m_nStatioins;
        *data = m_hStations;
    } else if (StringMatch(sk, DataType_Prefix_TS)) {
        *n = m_nStatioins;
        *data = m_stationData;
    } else {
        //throw ModelException(MID_ITP, "Get1DData", "Parameter " + sk
        //	+ " does not exist in the Interpolate module. Please contact the module developer.");
        *n = m_nCells;
        *data = m_itpOutput;

        //cout << m_output[0] << "\t";
    }

}
