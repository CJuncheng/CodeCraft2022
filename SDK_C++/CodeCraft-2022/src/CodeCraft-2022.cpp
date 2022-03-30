//#include "read_csv.h"
#include "distribution.h"

int main(int argc, char *argv[])
{
    Distribution obj;

    obj.getInputData(); // 数据读取，使得不同表通过索引对应
    // Test.printTest(); // 打印不同表索引对应的表
    obj.deleteUnusefulServer(); //删除无用边缘节点
    obj.initTable();
    obj.distributionNode();
    obj.saveSolution();

    // system("pause");

    return 0;
}
