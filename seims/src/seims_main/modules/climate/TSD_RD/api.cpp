#include "clsTSD_RD.h"

#include "api.h"

extern "C" SEIMS_MODULE_API SimulationModule *

GetInstance() {
    return new clsTSD_RD();
}

/// function to return the XML Metadata document string
extern "C" SEIMS_MODULE_API const char *MetadataInformation() {
    MetadataInfo mdi;
    string res;
    mdi.SetAuthor("Zhiqiang Yu");
    mdi.SetClass(MCLS_CLIMATE, MCLSDESC_CLIMATE);
    mdi.SetDescription(MDESC_TSD_RD);
    mdi.SetID(MID_TSD_RD);
    mdi.SetName(MID_TSD_RD);
    mdi.SetVersion("1.0");
    mdi.SetEmail(SEIMS_EMAIL);
    mdi.SetWebsite(SEIMS_SITE);
    mdi.SetHelpfile("");

    /// set the input variables (time series), and T means time series. D means distribution.
    mdi.AddInput(DataType_Prefix_TS, UNIT_NON_DIM, DESC_TSD_CLIMATE, Source_HydroClimateDB, DT_Array1D);

    /// set the output variables
    mdi.AddOutput(DataType_Prefix_TS, UNIT_NON_DIM, DESC_TSD_CLIMATE, DT_Array1D);

    /// write out the XML file.
    res = mdi.GetXMLDocument();

    char *tmp = new char[res.size() + 1];
    strprintf(tmp, res.size() + 1, "%s", res.c_str());
    return tmp;
}