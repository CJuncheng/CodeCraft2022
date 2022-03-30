#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H
#include "read_csv.h"
#include <cmath>
#include <algorithm>

class Distribution : public ReadCsv
{
    public:
        vector<vector<int32>> qos_client_index; //满足qos的每一个边缘节点对用的客户节点索引
       
        //建立三个表，时刻序列数代表表得行数
        vector<vector<int32> *> demandRemain;  //客户需求剩余表
        vector<vector<int32> *> bandwidthRemain; //边缘节点带宽剩余表
        vector<vector<vector<int32>> *> solution; //分配结果表: 最里层的vector装的是客户节点的索引和带宽获取结果

    public: 
        Distribution();
        ~Distribution();
        void deleteUnusefulServer(void);  

        void initTable(void); //初始化三个表
        void distributionNode(void); //核心处理节点
        void saveSolution(void);

    private: 
        vector<vector<int32> *> qos_data_temp;
        int32 num_former_5 = 0;
        int32 num_latter_95 = 0;
        vector<int32> client_priority; //存放每个客户节点可以访问的边缘节点数量
        vector<int32> server_priority_by_client_num; //按客户节点数量获取边缘节点优先级
        vector<int32> server_priority_by_server_bandwidth; //按边缘节点带宽容量来获取边缘节点优先级
        const string output_path = "/output/solution.txt";

        //一阶段调参变量(默认不进行压线)
        float scale_stage_1 = 1.0; //(0, 1] 越小越好
        float scale_server_num_1 = 0.0; // [0, 1) 越大越好

        //二阶段调参变量(默认不进行压线)
        float scale_stage_2 = 1.0; //(0, 1] 越小越好
        float scale_server_num_2 = 0.0; // [0, 1) 越大越好 ；scale_server_num_1 < scale_server_num_2

    private:
        int32 getServerBandwidthResponse(const int32 &server_index, const int32 &time_index);  //计算某边缘节点在某一时刻带宽请求量
        vector<int32> mySort(const vector<int32> &vec, const int32 &num_index);        //降序排序，返回前num_index对应的索引序列
        void getClientPriority(void);                       //确定每个客户可以访问的边缘节点数，以此反应客户节点的优先级
        void getServerPriorityByClientNum(void);
        void getServerPriorityByServerBandwidth(void);
        vector<int32> sortClientPriority(const int32 &server_index); //按客户节点优先级高返回客户节点索引
        vector<int32> sortServerPriority(const vector<int32> &server_priority, const bool &flag);

        void formerPercentage5(const vector<int32> &server_priority_index);

        vector<float> parameterSearchS1(const vector<int32> &server_priority_index);
        vector<float> parameterSearchS2(const vector<int32> &server_priority_index);

        int32 getServerBandwidthResponseAdjust(const int32 &server_index, const int32 &time_index, const vector<vector<int32> *> &demandRemain_);
        void latterPercentage95(const vector<int32> &server_priority_index);

        int32 getAllCost(const vector<vector<int32> *> & bandwidth_remain_table);
        bool isDistributionLegal(const vector<vector<int32> *> & demand_remain_table);

        void printDemandRemain(void);
        void printBandwidthRemain(void);  
        void printSolution(void);        
};

#endif
