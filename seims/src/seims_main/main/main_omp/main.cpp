#if (defined _DEBUG) && (defined _MSC_VER) && (defined VLD)
#include "vld.h"
#endif /* Run Visual Leak Detector during Debug */
#include "seims.h"
#include "invoke.h"

#ifndef USE_MONGODB
#define USE_MONGODB
#endif /* USE_MONGODB */

int main(int argc, const char *argv[]) {
    /// Register GDAL
    GDALAllRegister();
    /// Read input parameters
    string modelPath;
    int scenarioID = -1;  /// By default, no BMPs Scenario is used, in case of lack of BMPs database.
    int i = 0;
    int numThread = 1;
    LayeringMethod layeringMethod = UP_DOWN;
    char mongodbIP[16];
    stringcpy(mongodbIP, "127.0.0.1");
    int port = 27017;
    if (argc < 2) {
        cout << "Error: To run the program, use either the Simple Usage option or Usage option as below." << endl;
        goto errexit;
    } else if (argc > 2) {
        i = 1;
    } else {
        i = 2;
    }
    while (argc > i) {
        if (PathExists(string(argv[i]))) {
            modelPath = argv[i];
            i++;
        } else {
            goto errexit;
        }
        if (argc > i) {
            if (atoi(argv[i]) > 0) {
                numThread = atoi(argv[i]);
                assert(numThread >= 1);
                i++;
            } else {
                goto errexit;
            }
        }
        if (argc > i) {
            if (atoi(argv[i]) == 0 || atoi(argv[i]) == 1) {
                if (atoi(argv[i]) == 0) {
                    layeringMethod = UP_DOWN;
                } else {
                    layeringMethod = DOWN_UP;
                }
                i++;
            } else {
                goto errexit;
            }
        }
        if (argc > i) {
            if (isIPAddress(argv[i])) {
                stringcpy(mongodbIP, argv[i]);
                i++;
                if (argc > i && atoi(argv[i]) > 0) {
                    port = atoi(argv[i]);
                    assert(port > 0);
                    i++;
                } else {
                    goto errexit;
                }
            } else {
                goto errexit;
            }
        }
        if (argc > i) {
            if (atoi(argv[i]) >= 0) {
                scenarioID = atoi(argv[i]);
                i++;
            } else {
                goto errexit;
            }
        }
    }
    if (argc == 2) {
        modelPath = argv[1];
    }

    //cout<<modelPath<<endl;
    //cout<<numThread<<endl;
    //cout<<layeringMethod<<endl;
    //cout<<mongodbIP<<":"<<port<<endl;
    //cout<<scenarioID<<endl;
    //SetOpenMPThread(2);

    try {
        MainMongoDB(modelPath, mongodbIP, port, scenarioID, numThread, layeringMethod);
    }
    catch (ModelException& e) {
        cout << e.toString() << endl;
        exit(EXIT_FAILURE);
    }
    catch (exception& e) {
        cout << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (...) {
        cout << "Unknown exception occurred!" << endl;
        exit(EXIT_FAILURE);
    }
    return 0;

    errexit:
    cout << "Simple Usage:\n " << argv[0] << " <ModelPath>" << endl;
    cout << "\tBy default: " << endl;
    cout << "\t\tThe threads or processor number is 1." << endl;
    cout << "\t\tThe Layering Method is 0, which means UP_DOWN." << endl;
    cout << "\t\tThe MongoDB IP is 127.0.0.1 (i.e., localhost), and the port is 27017." << endl;
    cout << "\t\tThe Scenario ID is 0" << endl << endl;
    cout << "Complete Usage: " << argv[0] << " <ModelPath> [<threadsNum> <layeringMethod> <IP> <port> <ScenarioID>]" << endl;
    cout << "\t<ModelPath> is the path of the configuration of the Model." << endl;
    cout << "\t<threadsNum> must be greater or equal than 1." << endl;
    cout << "\t<layeringMethod> can be 0 and 1, which means UP_DOWN and DOWN_UP respectively." << endl;
    cout << "\t<IP> is the address of MongoDB database, and <port> is its port number." << endl;
    cout << "\t<ScenarioID> is the ID of BMPs Scenario which will be defined in BMPs database." << endl;
    exit(0);
}
