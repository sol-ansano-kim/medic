#include "medic/parameter.h"
#include <iostream>


using namespace MEDIC;


int main()
{
    MdParameter *p1 = new MdParameter();
    p1->setName("test1");
    MdParameter *p2 = new MdParameter();
    p2->setName("test2");
    MdParamContainer pc;
    bool result = pc.append(p1);
    std::cout << "add test1 : " << result << std::endl;
    result = pc.remove(p2);
    std::cout << "remove test2 : " << result << std::endl;
    result = pc.append(p2);
    std::cout << "add test2 : " << result << std::endl;
    result = pc.remove(p2);
    std::cout << "remove test2 : " << result << std::endl;
    MdParameter *p3 = new MdParameter("test", "test1", MdInt, 123);
    result = pc.append(p3);
    std::cout << "add test3 : " << result << std::endl;

    MdParameter *p4 = pc.getParam("test");
    int v = 14;
    p4->set(v);
    int v2;
    p4->get(v2);
    std::cout << "value is " << v2 << std::endl;
    p4->getDefault(v2);
    std::cout << "default value is " << v2 << std::endl;

    if (pc.set("test", 21))
    {
        std::cout << "set test done \n";
    }
    else
    {
        std::cout << "set test failed \n";   
    }
    if (pc.set("asdasd", 42))
    {
        std::cout << "set asdasd done \n";
    }
    else
    {
        std::cout << "set asdasd failed \n";   
    }

    int v3;
    if (pc.get("test", v3))
    {
        std::cout << "get test done "<< v3 << "\n";
    }
    else
    {
        std::cout << "get test failed \n";   
    }
    if (pc.get("asdasd", v3))
    {
        std::cout << "get asdasd done "<< v3 << "\n";
    }
    else
    {
        std::cout << "get asdasd failed \n";   
    }

    int v4;
    if (pc.getDefault("test", v4))
    {
        std::cout << "getDefault test done "<< v4 << "\n";
    }
    else
    {
        std::cout << "getDefault test failed \n";   
    }
    if (pc.getDefault("asdasd", v4))
    {
        std::cout << "getDefault asdasd done "<< v4 << "\n";
    }
    else
    {
        std::cout << "getDefault asdasd failed \n";   
    }
    
    pc.clear();
    return 0;
}