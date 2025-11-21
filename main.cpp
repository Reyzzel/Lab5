#include <iostream>
#include <future>
#include <chrono>
#include <syncstream>
#include <thread>

using namespace std;

void compute(const string& name, int sec)
{
    this_thread::sleep_for(chrono::seconds(sec));
    osyncstream(cout) << name << "\n";
}

void slow(const string& name)  { 
    compute(name, 7); 
}

void quick(const string& name) { 
    compute(name, 1); 
}

void work() {
    auto fA1 = async(launch::async, []{
        slow("A1");
    });

    auto fC2 = async(launch::async, []{
        slow("C2");
        quick("D2");
    });

    quick("A2");
    quick("C1");

    fA1.get(); 
    quick("B");

    quick("D1");

    fC2.get();
}

int main() {
    auto start = chrono::steady_clock::now();

    work();

    auto end = chrono::steady_clock::now();
    double sec = chrono::duration<double>(end - start).count();

    cout << "Time: " << sec << " sec\n";
    cout << "Work is done!\n";
}