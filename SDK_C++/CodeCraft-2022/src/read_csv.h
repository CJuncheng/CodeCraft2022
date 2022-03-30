#ifndef READ_CSV_H
#define READ_CSV_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;

using int32 = int;

class ReadCsv
{
    public:
        //从文件中获取的数据变量
        // qos.csv
        vector<string> client_name, server_name;
        vector<vector<int32> *> qos_data;
        //demand.csv
        vector<string> time_label;
        vector<vector<int32> *> client_demand;
        // bandwidth.csv
        vector<int32> bandwidth;
        //config.ini
        int32 qos_constraint = 0;

    public : 
    
        ReadCsv();
        ~ReadCsv();
        void getInputData(void);
        void printTest(void);
    
        
    private:
        vector<vector<int32>*> client_demand_temp;
        unordered_map<string, vector<int32> *> client_demand_map;
        unordered_map<string, int32> server_bandwidth_map;

        vector<string> *data_buffer_demand = new vector<string>; // 开辟堆空间，存储大规模读取的原始数据
        vector<string> *data_buffer_qos = new vector<string>;
        vector<string> *data_buffer_bandwidth = new vector<string>;
 
        const string demand_path = "/data/demand.csv";
        const string qos_path = "/data/qos.csv";
        const string site_bandwidth_path = "/data/site_bandwidth.csv";
        const string config_path = "/data/config.ini";
        
    private:
    
        vector<int32> readBase(const string &file_path, vector<string>* data_buffer);
        void readClientDemand(void);
        void readQos(void);
        void readServerBandwidth(void);
        void readConfig(void);
        

};

#endif
