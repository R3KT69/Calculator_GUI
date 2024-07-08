using namespace std;
#include <sstream>   
#include <iomanip>

string convert_3f(float *_speed)
{
    ostringstream str1;

    str1 << fixed << setprecision(3) << *_speed;

    string final_str = str1.str();

    return final_str;
}

int getFloatLength(float num) {
    ostringstream oss;
    oss << fixed << num;  
    string numStr = oss.str();
    
    return numStr.length();
}