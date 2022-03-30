
#include "distribution.h"


Distribution::Distribution()
{}
Distribution::~Distribution()
{
    /*
    for (size_t i = 0; i != qos_data_temp.size(); ++i)
    {
        delete[] qos_data_temp[i];
    }
    for (size_t i = 0; i != bandwidthRemain.size(); ++i)
    {
        delete[] bandwidthRemain[i];
    }
    for (size_t i = 0; i != demandRemain.size(); ++i)
    {
        delete[] demandRemain[i];
    }
    for (size_t i = 0; i != solution.size(); ++i)
    {
        delete[] solution[i];
    }
    */
}

void Distribution::deleteUnusefulServer(void)
{
    vector<vector<int32>> qos_client_index_temp;
    for (size_t i = 0; i != qos_data.size(); ++i)
    {
        vector<int32> temp;
        for (size_t j = 0; j != (qos_data[i])->size(); ++j)
        {
            if ((*qos_data[i])[j] < qos_constraint)
                temp.push_back(j);
        }
        qos_client_index_temp.push_back(temp);
    }

    /*    
    // print qos_client_index_temp
    cout << "=======边缘节点对应的客户节点======= " << qos_client_index_temp.size() << endl;
    for (int32 i = 0; i != qos_client_index_temp.size(); ++i)
    {
        cout << server_name[i]<<": ";
        for (int32 j = 0; j != qos_client_index_temp[i].size(); ++j)
            cout << qos_client_index_temp[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    */
    

    vector<string> server_name_temp;
    vector<int32> bandwidth_temp;
    for (size_t i = 0; i != qos_client_index_temp.size(); ++i)
    {
        if (qos_client_index_temp[i].size())
        {
            qos_client_index.push_back(qos_client_index_temp[i]);
            server_name_temp.push_back(server_name[i]);
            bandwidth_temp.push_back(bandwidth[i]);
            qos_data_temp.push_back(qos_data[i]);
        }
    }
    server_name.clear();
    bandwidth.clear();
    qos_data.clear(); 
    server_name = server_name_temp;
    bandwidth = bandwidth_temp;
    qos_data = qos_data_temp;

    /*
    cout << "=======满足qos条件，边缘节点对应的客户节点======= " << qos_client_index.size() << endl;
    for (size_t i = 0; i != qos_client_index.size(); ++i)
    {
        cout << server_name[i] << ": ";
        for (size_t j = 0; j != qos_client_index[i].size(); ++j)
            cout << qos_client_index[i][j] << " ";
        cout << endl;
    }
    */
    
}

void Distribution::initTable(void)
{
    // 1.初始化用户需求剩余表
    demandRemain = client_demand;
   
   // 2.初始化边缘节点带宽剩余表
    for (size_t i = 0; i != time_label.size(); ++i)
    {
        vector<int32> *temp = new vector<int32>;
        for (size_t i = 0; i != bandwidth.size(); ++i)
        {
            temp->push_back(bandwidth[i]);
        }
        bandwidthRemain.push_back(temp);
    }
   
    // 3.初始化保存结果表
    vector<int32> index_and_bandwidth;
    for (size_t i = 0; i != time_label.size(); ++i)
    {
        vector<vector<int32>> *temp_ = new vector<vector<int32>>;
        for (size_t j = 0; j != client_name.size(); ++j)
        {
            temp_->push_back(index_and_bandwidth);
        }
        solution.push_back(temp_);
    }
}

int32 Distribution::getServerBandwidthResponse(const int32 &server_index, const int32 &time_index) 
{
    if ((server_index < 0) && (server_index > int32(qos_client_index.size() - 1)))
    {
        cerr << "server index input error!" << endl;
        return -1;
    }
    // cout << "====getServerBandwidthResponse()===" << endl;
    int32 sum = 0;
    int32 client_index = 0;
    for (size_t i = 0; i != qos_client_index[server_index].size(); ++i)
    {
        client_index = qos_client_index[server_index][i];
        sum = sum + (*demandRemain[time_index])[client_index];
    }
    return sum;
}

int32 Distribution::getServerBandwidthResponseAdjust(const int32 &server_index, const int32 &time_index, const vector<vector<int32> *> &demandRemain_)
{
    if ((server_index < 0) && (server_index > int32(qos_client_index.size() - 1)))
    {
        cerr << "server index input error!" << endl;
        return -1;
    }
    // cout << "====getServerBandwidthResponse()===" << endl;
    int32 sum = 0;
    int32 client_index = 0;
    for (size_t i = 0; i != qos_client_index[server_index].size(); ++i)
    {
        client_index = qos_client_index[server_index][i];
        sum = sum + (*demandRemain_[time_index])[client_index];
    }
    return sum;
}

vector<int32> Distribution::mySort(const vector<int32> &vec, const int32& num_index) //降序排序，返回前num_index个索引
{
    
    vector<int32> vec_temp = vec;
    vector<int> index(vec_temp.size());
    for (size_t i = 0; i != index.size(); ++i)
    {
        index[i] = i;
    }
    std::sort(index.begin(), index.end(), [&vec_temp](int32 x1, int32 x2)
              { return vec_temp[x1] > vec_temp[x2]; }); //逆向排序 即按照从大到小的顺序进行排序

    vector<int32> index_former_5;

    for (int32 i = 0; i != num_index; ++i)
    {
        index_former_5.push_back((index[i]));
    }
    
    /*
    for (int32 i = 0; i != vec_temp.size(); ++i)
        cout << "客户需求(降序): " << index[i] << " " << vec_temp[i] << endl;
    */
    return index_former_5;

}

void Distribution::getClientPriority(void)
{
    for (size_t i = 0; i != client_name.size(); ++i)
    {
        int32 temp = 0;
        for (size_t j = 0; j != server_name.size(); ++j)
        {
            if ((*qos_data[j])[i] < qos_constraint)
                ++temp;
        }
        client_priority.push_back(temp);
    }

    //打印 client priority
    cout << "client priority: " << endl;
    for (size_t i = 0; i != client_priority.size(); ++i)
        cout << client_priority[i] << " ";
    cout << endl;
}

void Distribution::getServerPriorityByClientNum(void)
{
    for (size_t i = 0; i != qos_client_index.size(); ++i)
        server_priority_by_client_num.push_back((qos_client_index[i]).size());

    //打印 client priority
    cout << "server priority(按访问客户节点数量): " << endl;
    for (size_t i = 0; i != server_priority_by_client_num.size(); ++i)
        cout << server_priority_by_client_num[i] << " ";
    cout << endl;
}

void Distribution::getServerPriorityByServerBandwidth(void)
{
    server_priority_by_server_bandwidth = bandwidth;
    //打印 client priority
    cout << "server priority(按边缘节点带宽): " << endl;
    for (size_t i = 0; i != server_priority_by_server_bandwidth.size(); ++i)
        cout << server_priority_by_server_bandwidth[i] << " ";
    cout << endl;
}

vector<int32> Distribution::sortClientPriority(const int32 &server_index)
{
    vector<int32> client_priority_temp;
    for (size_t k = 0; k != qos_client_index[server_index].size(); ++k)
    {
        int32 client_index = qos_client_index[server_index][k];
        client_priority_temp.push_back(client_priority[client_index]);
    }

    vector<int> index(client_priority_temp.size());
    for (size_t i = 0; i != index.size(); ++i)
    {
        index[i] = i;
    }
    std::sort(index.begin(), index.end(), [&client_priority_temp](int x1, int x2)
              { return client_priority_temp[x1] < client_priority_temp[x2]; });
    

    vector<int32> client_priority_index;
    for (size_t i = 0; i != index.size(); ++i)
    {
        int32 client_index = qos_client_index[server_index][index[i]];
        client_priority_index.push_back(client_index);
    }
    /*
    cout << "client priority index for server index(" << server_index <<"):"<< endl;
    for (size_t k = 0; k != client_priority_index.size(); ++k)
        cout << client_priority_index[k] << " ";
    cout << endl;
    */
    return client_priority_index;
}

vector<int32> Distribution::sortServerPriority(const vector<int32> &server_priority, const bool& flag)
{ // flag ==0 false,边缘节点按访问客户从少到多排序；flag ==1 true,边缘节点按访问客户从多到少排序
    vector<int32> server_priority_temp = server_priority;
    vector<int> index(server_priority_temp.size());
    for (size_t i = 0; i != index.size(); ++i)
    {
        index[i] = i;
    }
    if(flag == 0 )
    {
        std::sort(index.begin(), index.end(), [&server_priority_temp](int x1, int x2)
                 { return server_priority_temp[x1] < server_priority_temp[x2]; });
    }
    else 
    {
        std::sort(index.begin(), index.end(), [&server_priority_temp](int x1, int x2)
                 { return server_priority_temp[x1] > server_priority_temp[x2]; });
    }
    
    vector<int32> server_priority_index;
    for (size_t i = 0; i != index.size(); ++i)
    {
        server_priority_index.push_back(index[i]);
    }

    //打印
    if (flag == 0)
    {
        cout << "server priority index(按访问客户节点数量由少到多排序): " << endl;
        for (size_t k = 0; k != server_priority_index.size(); ++k)
            cout << server_priority_index[k] << " ";
        cout << endl;
    }
    else
    {
        cout << "server priority index(按访问客户节点数量由多到少排序): " << endl;
        for (size_t k = 0; k != server_priority_index.size(); ++k)
            cout << server_priority_index[k] << " ";
        cout << endl;
    }

    return server_priority_index;
}

void Distribution::printDemandRemain(void)
{
    cout << "=======打印需求剩余表=======" << endl;
    for (size_t i = 0; i != demandRemain.size(); ++i)
    {
        cout << "demandRemain: " << i << " ";
        for (size_t j = 0; j != (demandRemain[i])->size(); ++j)
        {
            cout << (*demandRemain[i])[j] << " ";
        }
        cout << endl;
    }
}

void Distribution::printBandwidthRemain(void)
{
    cout << "=======打印带宽剩余表=======" << endl;
    for (size_t i = 0; i != bandwidthRemain.size(); ++i)
    {
        cout << "bandwidthRemain: " << i << " ";
        for (size_t j = 0; j != (bandwidthRemain[i])->size(); ++j)
        {
            cout << (*bandwidthRemain[i])[j] << " ";
        }
        cout << endl;
    }
}

void Distribution::printSolution(void)
{
    cout << "=======打印分配结果表=======" << endl;
    for (size_t i = 0; i != solution.size(); ++i)
    {
        cout << "solution: " << i << " ";
        for (size_t j = 0; j != (solution[i])->size(); ++j)
        {
            cout << "<";
            for (size_t k = 0; k != (*solution[i])[j].size(); ++k)
            {
                cout << (*solution[i])[j][k] ;
                if ((k+1)!= (*solution[i])[j].size())
                    cout << ",";
            }
            cout << ">";
        }
        cout << endl;
    }
}

void Distribution::formerPercentage5(const vector<int32> &server_priority_index)
{
    
    // cout << "time_label" << time_label.size() << endl;
    cout << endl;
    cout << "================开始前5%================" << endl;
    for (size_t i = 0; i != server_priority_index.size(); ++i)
    {
        int32 server_index = server_priority_index[i];  //根据优先级获取边缘节点索引
        //cout << "=======边缘节点索引：" << server_index << " =======" << endl;
        
        vector<int32> client_priority_index = sortClientPriority(server_index); //根据边缘节点索引获取对用优先级的客户节点索引
                                                                          
        vector<int32> server_bandwidth_response;
        vector<int32> index_former_5;

        for (size_t j = 0; j != time_label.size(); ++j)
        {
            //cout << "客户需求(按时刻)：" << j << " " << getServerBandwidthResponse(server_index, j) << endl;
            server_bandwidth_response.push_back(getServerBandwidthResponse(server_index, j));
        }

        index_former_5 = mySort(server_bandwidth_response, num_former_5);

        // cout << index_former_5.size() << " " << num_former_5;
        /*
        for (size_t j = 0; j != index_former_5.size(); ++j)
        {
            cout << "客户需求(5%)：" << index_former_5[j] << " " << getServerBandwidthResponse(server_index, index_former_5[j]) << endl;
        }
        */
        
        //=================分配前 5%===============
        for (size_t j = 0; j != index_former_5.size(); ++j)
        {
            int32 time_index = index_former_5[j];
            int32 bandwidth_response = getServerBandwidthResponse(server_index, time_index);
            int32 bandwidth_remain = (*bandwidthRemain[time_index])[server_index];

            if (bandwidth_remain && bandwidth_response) //带宽剩余和带宽需求均不为零才执行以下分配程序
            {
                if (bandwidth_response > bandwidth_remain) //边缘节点带宽剩余不足
                {

                    for (size_t k = 0; k != client_priority_index.size(); ++k) //按客户优先级暴力塞
                    {
                        int32 client_index = client_priority_index[k];
                        if ((*bandwidthRemain[time_index])[server_index] == 0)
                        {
                            //cout << "边缘节点带宽剩余为零！" << endl;
                            break;
                        }
                        if ((*demandRemain[time_index])[client_index] > (*bandwidthRemain[time_index])[server_index])
                        {
                            (*demandRemain[time_index])[client_index] -= (*bandwidthRemain[time_index])[server_index];

                            //保存结果
                            if ((*bandwidthRemain[time_index])[server_index])
                            {
                                (*solution[time_index])[client_index].push_back(server_index);
                                (*solution[time_index])[client_index].push_back((*bandwidthRemain[time_index])[server_index]);
                            }

                            (*bandwidthRemain[time_index])[server_index] = 0;
                        }
                        else
                        {
                            (*bandwidthRemain[time_index])[server_index] -= (*demandRemain[time_index])[client_index]; //更新bandwidthRemain

                            //保存结果
                            if ((*demandRemain[time_index])[client_index])
                            {
                                (*solution[time_index])[client_index].push_back(server_index);
                                (*solution[time_index])[client_index].push_back((*demandRemain[time_index])[client_index]);
                            }

                            (*demandRemain[time_index])[client_index] = 0; //更新 demandRemain
                        }
                    }
                }
                else //边缘节点带宽剩余充分
                {
                    for (size_t k = 0; k != client_priority_index.size(); ++k)
                    {
                        int32 client_index = client_priority_index[k];
                        (*bandwidthRemain[time_index])[server_index] -= (*demandRemain[time_index])[client_index]; //更新bandwidthRemain

                        //保存结果
                        if ((*demandRemain[time_index])[client_index])
                        {
                            (*solution[time_index])[client_index].push_back(server_index);
                            (*solution[time_index])[client_index].push_back((*demandRemain[time_index])[client_index]);
                        }

                        (*demandRemain[time_index])[client_index] = 0; //更新 demandRemain
                    }
                }
            }
        }
    }
}

void Distribution::latterPercentage95(const vector<int32> &server_priority_index)
{
    
    cout << endl;
    cout << "================开始后95%================" << endl;
    for (size_t i = 0; i != server_priority_index.size(); ++i)
    {
        int32 server_index = server_priority_index[i]; //根据优先级获取边缘节点索引
        //cout << "=======边缘节点索引：" << server_index << " =======" << endl;
        vector<int32> client_priority_index = sortClientPriority(server_index); //根据边缘节点索引获取对用优先级的客户节点索引

        //=================分配后 95%===============
        for (size_t j = 0; j != time_label.size(); ++j)
        {
            int32 time_index = j;
            int32 bandwidth_response = getServerBandwidthResponse(server_index, time_index); //某一时刻边缘节点对应的客户节点总需求
            int32 bandwidth_remain = (*bandwidthRemain[time_index])[server_index];
    
            //开始动态压线操作
            const int32 bandwidth_temp = bandwidth[0];  //获取一个带宽上限定值
            //尽可能小 [0,1]
            //(server_priority_index.size() * scale_server_num)
            if (i < (server_priority_index.size() * scale_server_num_1))
            {
                //cout << "========开始动态压线操作========" << endl;
                if (bandwidth_remain > (bandwidth_temp * scale_stage_1))
                    bandwidth_remain = bandwidth_temp * scale_stage_1;
            }
            else if ((i >= (server_priority_index.size() * scale_server_num_1)) && (i < (server_priority_index.size() * scale_server_num_2)))
            {
                if (bandwidth_remain > (bandwidth_temp * scale_stage_2))
                    bandwidth_remain = bandwidth_temp * scale_stage_2;
            }
        
            // cout << "bandwidth_remain:" << bandwidth_remain << endl;
            if (bandwidth_remain && bandwidth_response) //带宽剩余不为零和带宽需求均才执行以下分配程序
            {
                if (bandwidth_response > bandwidth_remain) //边缘节点带宽剩余不足
                {

                    for (size_t k = 0; k != client_priority_index.size(); ++k) //按客户优先级暴力塞
                    {
                        int32 client_index = client_priority_index[k];
                        if (bandwidth_remain == 0)
                        {
                            // cout << "边缘节点带宽剩余为零！" << endl;
                            break;
                        }
                        if ((*demandRemain[time_index])[client_index] > bandwidth_remain)
                        {
                            (*demandRemain[time_index])[client_index] -= bandwidth_remain;

                            //保存结果
                            if (bandwidth_remain)
                            {
                                (*solution[time_index])[client_index].push_back(server_index);
                                (*solution[time_index])[client_index].push_back(bandwidth_remain);
                            }

                            (*bandwidthRemain[time_index])[server_index] -= bandwidth_remain;
                            bandwidth_remain = 0;
                        }
                        else
                        {
                            bandwidth_remain -= (*demandRemain[time_index])[client_index]; //更新bandwidthRemain

                            //保存结果
                            if ((*demandRemain[time_index])[client_index])
                            {
                                (*solution[time_index])[client_index].push_back(server_index);
                                (*solution[time_index])[client_index].push_back((*demandRemain[time_index])[client_index]);
                            }

                            (*bandwidthRemain[time_index])[server_index] -= (*demandRemain[time_index])[client_index];
                            (*demandRemain[time_index])[client_index] = 0; //更新 demandRemain
                        }
                    }
                }
                else //边缘节点带宽剩余充分
                {
                    for (size_t k = 0; k != client_priority_index.size(); ++k)
                    {
                        int32 client_index = client_priority_index[k];
                        bandwidth_remain -= (*demandRemain[time_index])[client_index];

                        //保存结果
                        if ((*demandRemain[time_index])[client_index])
                        {
                            (*solution[time_index])[client_index].push_back(server_index);
                            (*solution[time_index])[client_index].push_back((*demandRemain[time_index])[client_index]);
                        }
                        (*bandwidthRemain[time_index])[server_index] -= (*demandRemain[time_index])[client_index]; //更新bandwidthRemain
                        (*demandRemain[time_index])[client_index] = 0;                                             //更新 demandRemain
                    }
                }
            }
        }
    }
}

int32 Distribution::getAllCost(const vector<vector<int32> *> &bandwidth_remain_table)
{
    //mySort(const vector<int32> &vec, const int32 &num_index);
    int32 cost = 0;
    for (size_t i = 0; i != server_name.size(); ++i)
    {
        vector<int32> bandwidth_used;
        for (size_t j = 0; j != bandwidth_remain_table.size(); ++j)
        {
            bandwidth_used.push_back(bandwidth[i] - (*bandwidth_remain_table[j])[i]);
        }
        int32 index_cost = mySort(bandwidth_used, bandwidth_used.size())[num_former_5];
        cost += bandwidth_used[index_cost];
    }
    return cost;
}

bool Distribution::isDistributionLegal(const vector<vector<int32> *> &demand_remain_table)
{
    int32 sum_demand_remain = 0;
    for (size_t i = 0; i != demand_remain_table.size(); ++i)
    {
        for (size_t j = 0; j != demand_remain_table[i]->size(); ++j)
            sum_demand_remain += (*demand_remain_table[i])[j];
    }
    //cout << "需求剩余和为： " << sum_demand_remain << endl;
    if (!sum_demand_remain)
        return true;
    else
        return false;
}

vector<float> Distribution::parameterSearchS1(const vector<int32> &server_priority_index)
{
    cout << endl;
    cout << "================开始后95%第一阶段调参================" << endl;

    vector<float> vec_scale_bandwidth;
    vector<float> vec_scale_server_num;
    vector<int32> vec_all_cost;

    for (float scale_server_num = 0.5; scale_server_num < 0.8; scale_server_num += 0.01)
    {
        for (float scale_bandwidth = 0.6; scale_bandwidth > 0.15; scale_bandwidth -= 0.01)
        {

            vector<vector<int32> *> demandRemainS1;
            vector<vector<int32> *> bandwidthRemainS1;
            for (size_t t = 0; t != demandRemain.size(); ++t)
            {
                vector<int32> *temp = new vector<int32>;
                for (size_t tt = 0; tt != (demandRemain[t])->size(); ++tt)
                {
                    temp ->push_back((*demandRemain[t])[tt]);
                }
                demandRemainS1.push_back(temp);
            }
            for (size_t t = 0; t != bandwidthRemain.size(); ++t)
            {
                vector<int32> *temp = new vector<int32>;
                for (size_t tt = 0; tt != (bandwidthRemain[t])->size(); ++tt)
                {
                    temp->push_back((*bandwidthRemain[t])[tt]);
                }
                bandwidthRemainS1.push_back(temp);
            }

            for (size_t i = 0; i != server_priority_index.size(); ++i)
            {
                int32 server_index = server_priority_index[i]; //根据优先级获取边缘节点索引
                // cout << "=======边缘节点索引：" << server_index << " =======" << endl;
                vector<int32> client_priority_index = sortClientPriority(server_index); //根据边缘节点索引获取对用优先级的客户节点索引

                //=================分配后 95%===============
                for (size_t j = 0; j != time_label.size(); ++j)
                {
                    int32 time_index = j;
                    int32 bandwidth_response = getServerBandwidthResponseAdjust(server_index, time_index, demandRemainS1); //某一时刻边缘节点对应的客户节点总需求
                    int32 bandwidth_remain = (*bandwidthRemainS1[time_index])[server_index];

                    //开始动态压线操作
                    const int32 bandwidth_temp = bandwidth[0]; //获取一个带宽上限定值
                    //尽可能小 [0,1]
                    //(server_priority_index.size() * scale_server_num)
                    if (i < (server_priority_index.size() * scale_server_num))
                    {
                        // cout << "========开始动态压线操作========" << endl;
                        if (bandwidth_remain > (bandwidth_temp * scale_bandwidth))
                            bandwidth_remain = bandwidth_temp * scale_bandwidth;
                    }
                    
                    // cout << "bandwidth_remain:" << bandwidth_remain << endl;
                    if (bandwidth_remain && bandwidth_response) //带宽剩余不为零和带宽需求均才执行以下分配程序
                    {
                        if (bandwidth_response > bandwidth_remain) //边缘节点带宽剩余不足
                        {
                            
                            for (size_t k = 0; k != client_priority_index.size(); ++k) //按客户优先级暴力塞
                            {
                                int32 client_index = client_priority_index[k];
                                if (bandwidth_remain == 0)
                                {
                                    // cout << "边缘节点带宽剩余为零！" << endl;
                                    break;
                                }
                                if ((*demandRemainS1[time_index])[client_index] > bandwidth_remain)
                                {
                                    (*demandRemainS1[time_index])[client_index] -= bandwidth_remain;
                                    (*bandwidthRemainS1[time_index])[server_index] -= bandwidth_remain;
                                    bandwidth_remain = 0;
                                }
                                else
                                {
                                    bandwidth_remain -= (*demandRemainS1[time_index])[client_index]; //更新bandwidthRemain
                                    (*bandwidthRemainS1[time_index])[server_index] -= (*demandRemainS1[time_index])[client_index];
                                    (*demandRemainS1[time_index])[client_index] = 0; //更新 demandRemain
                                }
                            }
                            
                        }
                        else //边缘节点带宽剩余充分
                        {
                            
                            for (size_t k = 0; k != client_priority_index.size(); ++k)
                            {
                                int32 client_index = client_priority_index[k];
                                bandwidth_remain -= (*demandRemainS1[time_index])[client_index];
                                (*bandwidthRemainS1[time_index])[server_index] -= (*demandRemainS1[time_index])[client_index]; //更新bandwidthRemain
                                (*demandRemainS1[time_index])[client_index] = 0;                                             //更新 demandRemain
                            }
                            
                        }
                    }
                    
                }
            }
            bool flag = isDistributionLegal(demandRemainS1);
            if (flag == false)
                break;
            cout << "scale_stage_1: " << scale_bandwidth << " "<< "scale_server_num_1: " << scale_server_num << endl;
            vec_scale_bandwidth.push_back(scale_bandwidth);
            vec_scale_server_num.push_back(scale_server_num);
            vec_all_cost.push_back(getAllCost(bandwidthRemainS1));
        }
       
    }
    std::vector<int32>::iterator smallest = std::min_element(std::begin(vec_all_cost), std::end(vec_all_cost));
    auto index_cost = std::distance(std::begin(vec_all_cost), smallest);
    //int32 index_cost = mySort(vec_all_cost, vec_all_cost.size())[vec_all_cost.size()-1];
    vector<float> scale;
    scale.push_back(vec_scale_bandwidth[index_cost]);
    scale.push_back(vec_scale_server_num[index_cost]);
    return scale;
}

vector<float> Distribution::parameterSearchS2(const vector<int32> &server_priority_index)
{
    cout << endl;
    cout << "================开始后95%第二阶段调参================" << endl;

    vector<float> vec_scale_bandwidth;
    vector<float> vec_scale_server_num;
    vector<int32> vec_all_cost;

    for (float scale_server_num = scale_server_num_1; scale_server_num < 0.90; scale_server_num += 0.01)
    {
        for (float scale_bandwidth = 1; scale_bandwidth > 0.8; scale_bandwidth -= 0.01)
        {

            vector<vector<int32> *> demandRemainS2;
            vector<vector<int32> *> bandwidthRemainS2;
            for (size_t t = 0; t != demandRemain.size(); ++t)
            {
                vector<int32> *temp = new vector<int32>;
                for (size_t tt = 0; tt != (demandRemain[t])->size(); ++tt)
                {
                    temp->push_back((*demandRemain[t])[tt]);
                }
                demandRemainS2.push_back(temp);
            }
            for (size_t t = 0; t != bandwidthRemain.size(); ++t)
            {
                vector<int32> *temp = new vector<int32>;
                for (size_t tt = 0; tt != (bandwidthRemain[t])->size(); ++tt)
                {
                    temp->push_back((*bandwidthRemain[t])[tt]);
                }
                bandwidthRemainS2.push_back(temp);
            }

            for (size_t i = 0; i != server_priority_index.size(); ++i)
            {
                int32 server_index = server_priority_index[i]; //根据优先级获取边缘节点索引
                // cout << "=======边缘节点索引：" << server_index << " =======" << endl;
                vector<int32> client_priority_index = sortClientPriority(server_index); //根据边缘节点索引获取对用优先级的客户节点索引

                //=================分配后 95%===============
                for (size_t j = 0; j != time_label.size(); ++j)
                {
                    int32 time_index = j;
                    int32 bandwidth_response = getServerBandwidthResponseAdjust(server_index, time_index, demandRemainS2); //某一时刻边缘节点对应的客户节点总需求
                    int32 bandwidth_remain = (*bandwidthRemainS2[time_index])[server_index];

                    //开始动态压线操作
                    const int32 bandwidth_temp = bandwidth[0]; //获取一个带宽上限定值
                    //尽可能小 [0,1]
                    //(server_priority_index.size() * scale_server_num)
                    if (i < (server_priority_index.size() * scale_server_num_1))
                    {
                        // cout << "========开始动态压线操作========" << endl;
                        if (bandwidth_remain > (bandwidth_temp * scale_stage_1))
                            bandwidth_remain = bandwidth_temp * scale_stage_1;
                    }
                    else if ((i >= (server_priority_index.size() * scale_server_num_1)) && (i < (server_priority_index.size() * scale_server_num)))
                    {
                        if (bandwidth_remain > (bandwidth_temp * scale_bandwidth))
                            bandwidth_remain = bandwidth_temp * scale_bandwidth;
                    }

                    // cout << "bandwidth_remain:" << bandwidth_remain << endl;
                    if (bandwidth_remain && bandwidth_response) //带宽剩余不为零和带宽需求均才执行以下分配程序
                    {
                        if (bandwidth_response > bandwidth_remain) //边缘节点带宽剩余不足
                        {
                            for (size_t k = 0; k != client_priority_index.size(); ++k) //按客户优先级暴力塞
                            {
                                int32 client_index = client_priority_index[k];
                                if (bandwidth_remain == 0)
                                {
                                    // cout << "边缘节点带宽剩余为零！" << endl;
                                    break;
                                }
                                if ((*demandRemainS2[time_index])[client_index] > bandwidth_remain)
                                {
                                    (*demandRemainS2[time_index])[client_index] -= bandwidth_remain;
                                    (*bandwidthRemainS2[time_index])[server_index] -= bandwidth_remain;
                                    bandwidth_remain = 0;
                                }
                                else
                                {
                                    bandwidth_remain -= (*demandRemainS2[time_index])[client_index]; //更新bandwidthRemain
                                    (*bandwidthRemainS2[time_index])[server_index] -= (*demandRemainS2[time_index])[client_index];
                                    (*demandRemainS2[time_index])[client_index] = 0; //更新 demandRemain
                                }
                            }
                        }
                        else //边缘节点带宽剩余充分
                        {
                            for (size_t k = 0; k != client_priority_index.size(); ++k)
                            {
                                int32 client_index = client_priority_index[k];
                                bandwidth_remain -= (*demandRemainS2[time_index])[client_index];
                                (*bandwidthRemainS2[time_index])[server_index] -= (*demandRemainS2[time_index])[client_index]; //更新bandwidthRemain
                                (*demandRemainS2[time_index])[client_index] = 0;                                               //更新 demandRemain
                            }
                        }
                    }
                }
            }
            bool flag = isDistributionLegal(demandRemainS2);
            if (flag == false)
                break;
            cout << "scale_stage_2: " << scale_bandwidth << " " << "scale_server_num_2: " << scale_server_num << endl;
            vec_scale_bandwidth.push_back(scale_bandwidth);
            vec_scale_server_num.push_back(scale_server_num);
            vec_all_cost.push_back(getAllCost(bandwidthRemainS2));    
        }
    }
    std::vector<int32>::iterator smallest = std::min_element(std::begin(vec_all_cost), std::end(vec_all_cost));
    auto index_cost = std::distance(std::begin(vec_all_cost), smallest);
    //int32 index_cost = mySort(vec_all_cost, vec_all_cost.size())[vec_all_cost.size() - 1];
    vector<float> scale;
    scale.push_back(vec_scale_bandwidth[index_cost]);
    scale.push_back(vec_scale_server_num[index_cost]);

    return scale;
}

void Distribution::distributionNode(void)
{
    num_former_5 = floor((time_label.size()) * 0.05);
    num_latter_95 = time_label.size() - num_former_5;
    cout << "num_former_5: " << num_former_5 << endl;
    cout << "num_former_95: " << num_latter_95 << endl;
    getClientPriority();

    // ======== 获取服务器索引序列 ==========
    // 1. 服务器优先级按访问客户节点数量
    getServerPriorityByClientNum();
    vector<int32> server_priority_index_1 = sortServerPriority(server_priority_by_client_num, false); //边缘节点按访问客户从少到多排序
    vector<int32> server_priority_index_2 = sortServerPriority(server_priority_by_client_num, true);  //边缘节点按访问客户从多到少排序
    // 1. 边缘节点优先级按边缘节点带宽
    getServerPriorityByServerBandwidth();
    vector<int32> server_priority_index_3 = sortServerPriority(server_priority_by_server_bandwidth, false); //边缘节点按边缘节点带宽从少到多排序
    vector<int32> server_priority_index_4 = sortServerPriority(server_priority_by_server_bandwidth, true);  //边缘节点按边缘节点带宽从多到少排序

    // ======== 优化5部分 ==========
    formerPercentage5(server_priority_index_1);
    //printDemandRemain();
    // ======== 两阶段法优化95部分 ==========
    
    //压线操作变量
    
    //===自动====
    //阶段一
    //reverse(server_priority_index_1.begin(), server_priority_index_1.end());
    vector<float> vec_scale = parameterSearchS1(server_priority_index_2);
    
    scale_stage_1 = vec_scale[0]; //越低越好
    scale_server_num_1 = vec_scale[1]; //越大越好
    //阶段二
    vector<float> vec_scale_2 = parameterSearchS2(server_priority_index_2);
    scale_stage_2 = vec_scale_2[0]; //越低越好
    scale_server_num_2 = vec_scale_2[1]; //越大越好
    
    /*
    //===手动====
    //阶段一
    scale_stage_1 = 0.19; //越低越好
    scale_server_num_1 = 0.71; //越大越好
    //阶段二
    scale_stage_2 = 0.8; //越低越好
    scale_server_num_2 = 0.71; //越大越好
    */
    
    //printDemandRemain();
    cout << "两阶段变量的设定值：" << scale_stage_1 << " " << scale_server_num_1 << " " << scale_stage_2 << " " << scale_server_num_2 << endl;    
    latterPercentage95(server_priority_index_2);
    int32 cost = getAllCost(bandwidthRemain);
    cout << "=======All cost is： " << cost << " =======" << endl;
    
    bool flag = isDistributionLegal(demandRemain);
    cout << "是否分配合法： " << flag << " =======" << endl;
    //printDemandRemain();
    //printBandwidthRemain();
    //printSolution();
}

void Distribution::saveSolution(void)
{
    //输出：
    ofstream ofs; //写入txt文件，写一个时刻的数据
    ofs.open(output_path, ios::out);

    for (size_t i = 0; i < solution.size(); ++i) //时间
    {
        for (size_t j = 0; j < solution[i]->size(); ++j) // 客户
        {
            ofs << client_name[j] << ":";
            for (size_t k = 0; k < (*solution[i])[j].size(); ++k)
            {
                int32 server_index_temp = (*solution[i])[j][k];
                ofs << "<" << server_name[server_index_temp] << "," << (*solution[i])[j][++k] << ">";
                if ((k+1) != (*solution[i])[j].size())
                {
                    ofs << ",";
                }   
            }
            ofs << endl;
        }
    }
    ofs.close();
}

