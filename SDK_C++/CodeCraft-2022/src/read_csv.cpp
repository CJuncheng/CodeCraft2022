#include "read_csv.h"

ReadCsv::ReadCsv()
{}


ReadCsv::~ReadCsv()
{
    /*
    delete[] data_buffer_demand;
    delete[] data_buffer_qos;
    delete[] data_buffer_bandwidth;
   
    for (size_t i = 0; i != client_demand_temp.size(); ++i)
    {
        delete[] client_demand_temp[i];
    }

    for (size_t i = 0; i != client_demand.size(); ++i)
    {
        delete[] client_demand[i];
    }

    for (size_t i = 0; i != qos_data.size(); ++i)
    {
        delete[] qos_data[i];
    }

    for (auto it = client_demand_map.begin(); it != client_demand_map.end(); ++it)
    {
        delete[] it->second;
    }
    */
}

vector<int>
ReadCsv::readBase(const string &file_path, vector<string> *data_buffer)
{
    int32 num_row = 0;
    int32 num_col = 0;
    ifstream ifs(file_path, ios::in);
    if (ifs.is_open())
    {
        string buf;
        while (getline(ifs, buf))
        {
            //getline() 跨平台读取csv文件
            if (buf[buf.length() - 1] == '\r')
            {
                buf.erase(buf.end() - 1);
                //cout << "这是linux系统" << endl;
            }
            //#endif
            stringstream ss;
            ss << buf;
            string buff;
            while (getline(ss, buff, ','))
            {
                data_buffer->push_back(buff); 
                if (!num_row)
                    ++num_col; //获取原始数据列数
            }
            ++num_row; //获取原始数据行数
        }
    }
    else
    {
        cerr << "File Open failed" << endl;
    }
    ifs.close();

    vector<int> num_row_and_col;
    num_row_and_col.push_back(num_row);
    num_row_and_col.push_back(num_col);

    return num_row_and_col;
}

void ReadCsv::readClientDemand(void)
{
    vector<int> num_row_and_col = readBase(demand_path, data_buffer_demand);
    int32 num_row = num_row_and_col[0];
    int32 num_col = num_row_and_col[1];
    cout << "Demand file: " << "row:" << num_row << " " << "col:" << num_col << endl;

    for (int32 i = 1; i != num_row; ++i)
    {
        time_label.push_back((*data_buffer_demand)[i * num_col]);
    }

    for (int32 i = 1; i != num_col; ++i)
    {
        vector<int32>* temp = new vector<int32>;
        for (int32 j = 1; j != num_row; ++j)
        {
            temp->push_back(stoi((*data_buffer_demand)[j * num_col + i]));
        }
        client_demand_map.insert(make_pair((*data_buffer_demand)[i], temp));
    }
}

void ReadCsv::readQos(void)
{
    
    vector<int> num_row_and_col = readBase(qos_path, data_buffer_qos);
    int32 num_row = num_row_and_col[0];
    int32 num_col = num_row_and_col[1];
    cout << "Qos file: " << "row:" << num_row << " " << "col:" << num_col << endl;

    for (int32 i = 1; i != num_row; ++i)
    {
        vector<int> *temp = new vector<int>;
        for (int32 j = 1; j != num_col; ++j)
        {
            temp->push_back(stoi((*data_buffer_qos)[i * num_col + j]));
        }
        qos_data.push_back(temp);
    }

    for (int32 i = 1; i != num_col; ++i)
    {
        client_name.push_back((*data_buffer_qos)[i]);
    }

    for (int32 i = 1; i != num_row; ++i)
    {
        server_name.push_back((*data_buffer_qos)[i * num_col]);
    }
}

void ReadCsv::readServerBandwidth(void)
{
    vector<int32> num_row_and_col = readBase(site_bandwidth_path, data_buffer_bandwidth);
    int32 num_row = num_row_and_col[0];
    int32 num_col = num_row_and_col[1];
    cout << "Server bandwidth file: " << "row:" << num_row << " " << "col:" << num_col << endl;

    int32 bandwidth_temp = 0;
    for (int32 i = 1; i != num_row; ++i)
    {
        bandwidth_temp = stoi((*data_buffer_bandwidth)[i * num_col + 1]);
        server_bandwidth_map.insert(make_pair((*data_buffer_bandwidth)[i * num_col], bandwidth_temp));
    } 
}

void ReadCsv::readConfig(void)
{
    ifstream ifs(config_path, ios::in);

    if (ifs.is_open())
    {
        string buff;
        ifs >> buff;
        ifs >> buff;
        int start = 15;
        string temp = buff.substr(start, buff.size() - start);
        qos_constraint = stoi(temp); // string转int
        cout << "Qos constraint: " << qos_constraint << endl;
    }
    else
    {
        cerr << "File Open failed" << endl;
    }
    ifs.close();
}

void ReadCsv::getInputData(void)
{
    readQos();
    readClientDemand();
    readServerBandwidth();
    readConfig();
    for (size_t i = 0; i != server_name.size(); ++i)
    {
        bandwidth.push_back(server_bandwidth_map[server_name[i]]);
    }

    for (size_t i = 0; i != client_name.size(); ++i)
    {
        client_demand_temp.push_back(client_demand_map[client_name[i]]);
    }

    // client_demand_temp 转置
    int32 num_row = (client_demand_temp[0])->size();
    int32 num_col = client_demand_temp.size();
    for (int32 i = 0; i != num_row; ++i)
    {
        vector<int32> *temp = new vector<int32>;
        for (int32 j = 0; j != num_col; ++j)
        {
            temp->push_back((*client_demand_temp[j])[i]);
        }
        client_demand.push_back(temp);
    }
}

void ReadCsv::printTest(void)
{
    cout << "=======demand.csv=======" << endl;
    cout << "time label is: " << endl;
    for (size_t i = 0; i != time_label.size(); ++i)
    {
        cout << time_label[i] << endl;
    }

    cout << "client demand is: " << endl;
    for (size_t i = 0; i != client_demand.size(); ++i)
    {
        for (size_t j = 0; j != (client_demand[i])->size(); ++j)
        {
            cout << (*client_demand[i])[j] << " ";
        }
        cout << endl;
    }

    cout << "=======qos.csv=======" << endl;
    cout << "server name is: " << endl;
    for (size_t i = 0; i != server_name.size(); ++i)
    {
        cout << server_name[i] << endl;
    }

    cout << "client name is: " << endl;
    for (size_t i = 0; i != client_name.size(); ++i)
    {
        cout << client_name[i] << " ";
    }
    cout << endl;

    cout << "qos data is: " << endl;
    for (size_t i = 0; i != qos_data.size(); ++i)
    {
        for (size_t j = 0; j != (qos_data[i])->size(); ++j)
        {
            cout << (*qos_data[i])[j] << " ";
        }
        cout << endl;
    }

    cout << "=======bandwidth.csv=======" << endl;
    cout << "server bandwidth is: " << endl;
    for (size_t i = 0; i != bandwidth.size(); ++i)
    {
        cout << bandwidth[i] << endl;
    }
}



/*
vector<string>*ReadCsv::getRowData(const int32 &row) // 读取特定行
{
    vector<string> *vr = new vector<string>;
    for (int j = 0; j < num_columns; j++)
    {
        vr->push_back((*data_buffer)[row * num_columns + j]);
    }

    return vr;
}

vector<string>*
ReadCsv::getColData(const int32 &col) // 读取特定列
{
    vector<string> *vc = new vector<string>;
    for (int i = 0; i < num_rows; i++)
    {
        vc->push_back((*data_buffer)[i * num_columns + col]);
    }
    return vc;
}
*/