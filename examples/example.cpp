using namespace std;

#include "intervals.h"

int main()
{
    IntervalManager inmgr("cepdat");
    vector<Event> evseq =
        {
        2, 10, 2, 11, 1, 12, 2, 13, 1,
        3, 14, 5, 15, 4, 16, 5, 17, 3, 18, 4, 19, 5,
        6, 20, 7, 21, 6, 22, 8, 23, 6, 24, 7, 8, 6
        };
    for(auto e:evseq)
    {
        cout << "Sending event " << e << endl;
        inmgr.route(e);
    }
    cout << "Generated: cep.log" << endl;
}
