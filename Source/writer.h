#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std ;
struct weka_data
{
    double PR ;
    double Ramp ;
    double rate ;
    double QRS ;
    double
    int disease ;
};
class writer
{
public :
    writer();
    void setwekadata(weka_data input);
    void settofile();
protected:
private:
    vector<weka_data> buffer;

};
