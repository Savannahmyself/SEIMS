#if (defined _DEBUG) && (defined MSVC) && (defined VLD)
#include "vld.h"
#endif /* Run Visual Leak Detector during Debug */
#include <iostream>
#include <fstream>
#include <sstream>

#include "MongoUtil.h"
#include "clsRasterData.cpp"

#include "SubbasinIUHCalculator.h"

using namespace std;

void MainMongoDB(const char *modelStr, const char *gridFSName, int nSubbasins, const char *host, int port, int dt) {
    // connect to mongodb
    MongoClient* client = MongoClient::Init(host, port);
    if (NULL == client) {
        throw ModelException("DataCenterMongoDB", "Constructor", "Failed to connect to MongoDB!");
    }
    MongoGridFS* gfs = new MongoGridFS(client->getGridFS(string(modelStr), string(gridFSName)));
    int subbasinStartID = 1;
    if (nSubbasins == 0) subbasinStartID = 0;
    for (int i = subbasinStartID; i <= nSubbasins; i++) {
        //cout << "subbasin: " << i << endl;
        //input
        ostringstream oss;
        string deltaName, streamLinkName, tName, maskName, landcoverName;
        oss << i << "_DELTA_S";
        deltaName = oss.str();

        oss.str("");
        oss << i << "_T0_S";
        tName = oss.str();

        oss.str("");
        oss << i << "_MASK";
        maskName = oss.str();

        oss.str("");
        oss << i << "_LANDCOVER";
        landcoverName = oss.str();

        clsRasterData<int> rsMask;
        rsMask.ReadFromMongoDB(gfs, maskName.c_str());

        clsRasterData<float> rsTime, rsDelta, rsLandcover;
        rsTime.ReadFromMongoDB(gfs, tName.c_str());
        rsDelta.ReadFromMongoDB(gfs, deltaName.c_str());
        rsLandcover.ReadFromMongoDB(gfs, landcoverName.c_str());

        SubbasinIUHCalculator iuh(dt, rsMask, rsLandcover, rsTime, rsDelta, gfs);
        iuh.calCell(i);
    }
}

int main(int argc, const char **argv) {
    try { 	
        const char *host = argv[1];
        int port = atoi(argv[2]);
        const char *modelName = argv[3];
        const char *gridFSName = argv[4];
        int dt = atoi(argv[5]); //time interval in hours
        int nSubbasins = atoi(argv[6]); // the whole basin is 0

        MainMongoDB(modelName, gridFSName, nSubbasins, host, port, dt);

        cout << " IUH calculation is OK!" << endl;
    }
    catch (ModelException& e) {
        cout << e.toString() << endl;
        return -1;
    }
    catch (exception& e) {
    	cout << e.what() << endl;
        return -1;
    }
    catch (...) {
        cout << "Unknown exception occurred!" << endl;
        return -1;
    }
    return 0;
}