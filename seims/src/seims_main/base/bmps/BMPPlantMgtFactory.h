/*!
 * \brief Plant management operations factory
 * \author Liang-Jun Zhu
 * \date June 2016
 */
#ifndef SEIMS_BMP_PLANTMGT_H
#define SEIMS_BMP_PLANTMGT_H

#include "utilities.h"
#include "BMPFactory.h"
#include "PlantManagementOperation.h"

using namespace MainBMP;
using namespace PlantManagement;

namespace MainBMP {
/*!
 * \class BMPPlantMgtFactory
 * \ingroup MainBMP
 *
 * \brief Initiate a plant management BMP
 * Actually, it contains a series BMPs, such as plant, fertilize, harvest, etc.
 *
 */
class BMPPlantMgtFactory : public BMPFactory {
public:
    /// Constructor
    BMPPlantMgtFactory(const int scenarioId, const int bmpId, const int subScenario,
                       const int bmpType, const int bmpPriority, vector<string> &distribution,
                       const string collection, const string location);

    /// Destructor
    ~BMPPlantMgtFactory(void);

    /// Load BMP parameters from MongoDB
    void loadBMP(MongoClient* conn, const string &bmpDBName);

    /// Output
    void Dump(ostream *fs);

    /// Set management fields data
    void setRasterData(map<string, FloatRaster*> &sceneRsMap);

    /// Get management fields data
    float* getRasterData() { return m_mgtFieldsRs; };

    /// Get landuse / landcover ID
    int GetLUCCID(void) {
        return m_luccID;
    }

    /// Get locations
    vector<int> &GetLocations(void) {
        return m_location;
    }

    /// Get operation sequence
    vector<int> &GetOperationSequence(void) {
        return m_bmpSequence;
    }

    /// Get operations
    map<int, PlantManagementOperation *> &GetOperations(void) {
        return m_bmpPlantOps;
    }

    /// Get operation by ID
    PlantManagementOperation *GetOperation(int ID) {
        return m_bmpPlantOps.at(ID);
    }

private:
    /// subSecenario name
    string m_name;
    /// management fields name, defined in 'distribution'
    string m_mgtFieldsName;
    /// management fields data (1D array raster)
    float* m_mgtFieldsRs;
    /// landuse / landcover
    int m_luccID;
    /// parameters
    float *m_parameters;
    /// field index for where to apply the subScenario
    vector<int> m_location;
    /*!
     * The first element is the sequence number of plant management operations
     * and the second is the corresponding unique management code, i.e., index * 1000 + operationCode
     * m_bmpSequence[0] = 1002 means the first (1001 / 1000 = 1) operation is Irrigation (1002 % 1000 = 2)
     */
    vector<int> m_bmpSequence;
    /*!
     * Key is the unique management code in m_bmpSequence, e.g., 1002
     * Value is the corresponding PlantMangementOperation instance
     */
    map<int, PlantManagementOperation *> m_bmpPlantOps;
};
}
#endif /* SEIMS_BMP_PLANTMGT_H */
