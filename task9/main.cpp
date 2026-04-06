#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int main() {
    std::string str;
    std::vector<int> randArray;
    int val;
    
    if (std::getline(std::cin, str)) {
        std::stringstream ss(str);
        
        while (ss >> val) {
            randArray.push_back(val);
        }
    }

    size_t start = 0, end = 0, maxCount = 0, maxSum = 0, curStart = 0, count = 0, max = randArray[0];
    for(size_t i = 1; i < randArray.size(); ++i) {
        if(randArray[i - 1] < randArray[i]) {
            ++count;
            max += randArray[i];
        }
        else {
            if((count == maxCount && max > maxSum) || (count > maxCount)) {
                start = i - count - 1;
                end = i;
                maxCount = count;
                maxSum = max;
            }
            curStart = i;
            count = 0;
            max = randArray[i];
        }
    }

    if((count == maxCount && max > maxSum) || (count > maxCount)) {
        start = curStart;
        end = randArray.size();
    }
    
    for(size_t i = start; i < end; ++i) std::cout << randArray[i] << " ";
    std::cout << std::endl;

    return 0;
}