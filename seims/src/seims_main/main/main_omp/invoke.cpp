#include "invoke.h"

int MainMongoDB(string modelPath, char *host, uint16_t port, int scenarioID, int numThread, LayeringMethod layeringMethod) {
    /// Get module path
    string modulePath = GetAppPath();
    /// Check model path
    if (!DirectoryExists(modelPath)) {
        cout << "Model folder " << modelPath << " is not existed!" << endl;
        return -1;
    }
    /// Create data center according to subbasin number, 0 means the whole basin which is default for omp version.
    int subbasinID = 0;
    DataCenterMongoDB* dataCenter = new DataCenterMongoDB(host, port, modelPath, modulePath, layeringMethod, 
                                                          subbasinID, scenarioID, numThread);
    /// Create module factory
    ModuleFactory* moduleFactory = new ModuleFactory(dataCenter);
    /// Create SEIMS model by dataCenter and moduleFactory
    unique_ptr<ModelMain> main(new ModelMain(dataCenter, moduleFactory));
    /// Execute model and write outputs
    main->Execute();
    main->Output();

    return 0;
}
