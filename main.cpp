#include <iostream>
#include <vector>
#include <numeric>
#include <functional>
#include <iterator>
#include <ctime>
#include "math.h"

template<typename T>
class NDArray{
public:
    //constructors
    NDArray(const std::vector<int>& shape){
        this->shape = shape;
        size = std::accumulate(shape.begin(),shape.end(),1, std::multiplies<int>());
        arr.resize(size);
    }
    NDArray(const std::vector<int>& shape, const T array[]){
        this->shape = shape;
        size = std::accumulate(shape.begin(),shape.end(),1, std::multiplies<int>());
        arr.resize(size);
        arr.assign(array,array+size);
    }

    //fill NDArray
    void zeroes() {
        for (int i = 0; i < GetSize(); i++) arr[i] = 0;
    }

    void ones() {
        for (int i = 0; i < GetSize(); i++) arr[i] = 1;
    }

    void random() {
        srand(time(NULL));
        for (int i = 0; i < GetSize(); i++) arr[i] = rand();
    }

    //Getters
    int GetSize() {return size;}

    T GetItem(int index) const {
        if(index < 0 or index > size){std::cout << "\nerror: wrong index!\n"; return -1;}
        return arr[index];
    }

    //Ndim
    int ndim() {return shape.size();}

    //Arr cout << operator
    template <class U>
    friend std::ostream &operator <<(std::ostream& output, NDArray<U>& arr);

    // Shape print
    void shapePrint() {
        if (shape.size() < 2 or shape.size() > 2) std::cout << "\nerror: wrong size, use standard cout!\n";
        else {
            int buff = 0;
            for (int i = 0; i < shape.at(0); i++){
                for(int j = 0; j < shape.at(1); j ++)
                {
                    std::cout << arr[j+buff] << " ";
                }
                buff += shape.at(1);
                std::cout << std::endl;
            }
        }
    }


    //operators
    NDArray operator + (const NDArray<T> & other){
        if (shape == other.shape) {
            NDArray<T> temp(shape);
            for (int i = 0; i < GetSize(); i++) temp.arr[i] = arr[i] + other.GetItem(i);
            return temp;
        }
    };

    NDArray operator - (const NDArray<T> & other){
        if (shape == other.shape) {
            NDArray<T> temp(shape);
            for (int i = 0; i < GetSize(); i++) temp.arr[i] = arr[i] - other.GetItem(i);
            return temp;
        }
    };

    NDArray operator * (const NDArray<T> & other){
        if (shape == other.shape) {
            NDArray<T> temp(shape);
            for (int i = 0; i < GetSize(); i++) temp.arr[i] = arr[i] * other.GetItem(i);
            return temp;
        }
    };

    NDArray operator / (const NDArray<T> & other){
        if (shape == other.shape) {
            NDArray<T> temp(shape);
            for (int i = 0; i < GetSize(); i++) temp.arr[i] = arr[i] / other.GetItem(i);
            return temp;
        }
    };

    //matmull
    void matmul(NDArray<T> other){
        if (ndim() == 2 && other.ndim() == 2) {
            if (shape.at(0) == other.shape.at(1)) {
                NDArray<T> buff(shape);
                buff.shape.at(1) = other.shape.at(1);
                buff.size = shape.at(0)*other.shape.at(1);
                buff.arr.resize(buff.size);
                int temp = 0;
                int flag = 0;
                for (int i = 0; i < buff.size; i++) {
                    T sum = 0;
                    for (int j = flag, ind = 0; j < other.shape.at(0)+flag; j++) {
                        sum += arr[j] * other.arr[ind * other.shape.at(1) + temp];
                        ind++;
                    }
                    temp++;
                    if ((i+1) % shape.at(0) == 0) {
                        temp = 0;
                        flag += other.shape.at(0);
                    }
                    buff.arr[i] = sum;
                }
                arr = buff.arr;
                size = buff.size;
                shape = buff.shape;
            } else std::cout << "\nerror: rows!= columns!\n";
        }else std::cout << "\nerror: you can't matmul these matrices!\n";
    }

    //transpose
    void Tmatrix(){
        if (ndim() == 2) {
            NDArray<T> buff(shape);
            buff.shape.at(0) = shape.at(1);
            buff.shape.at(1) = shape.at(0);
            int temp = 0;
            int flag = 0;
            for (int i = 0; i < buff.GetSize(); i++) {
                buff.arr[i] = arr[flag*shape.at(1)+temp];
                flag++;
                if ((i+1) % shape.at(0) == 0) {
                    temp++;
                    flag = 0;
                }
            }
            arr = buff.arr;
            size = buff.size;
            shape = buff.shape;
        }
        else std::cout << "\nerror: you can't transpose this matrix!\n";
    }

    //min, max, avg without range

    T min(){
        T min = arr[0];
        for(int i = 1; i < GetSize(); i++){
            if (arr[i] < min) min = arr[i];
        }
        return min;
    }

    T max(){
        T max = arr[0];
        for(int i = 1; i < GetSize(); i++){
            if (arr[i] > max) max = arr[i];
        }
        return max;
    }

    float avg(){
        float sum = 0;
        for(int i = 0; i < GetSize(); i++){
            sum+=arr[i];
        }
        return sum / GetSize();
    }

    //error for check min, max, avg begin and end
    bool Check_begin_end(int begin, int end) {
        if(begin > end) {std::cout <<"\nerror: begin >end!\n"; return 1;}
        else if (begin < 0 or end > size) {std::cout <<"\nerror: wrong begin or end!\n"; return 1;}
        return 0;
    }

    //min, max, avg with range
    T min(int begin, int end){
        if(Check_begin_end(begin,end) != 0) return -1;
        T min = arr[begin];
        for(int i = begin; i < end; i++){
            if (arr[i] < min) min = arr[i];
        }
        return min;
    }

    T max(int begin, int end){
        if(Check_begin_end(begin,end) != 0) return -1;
        T max = arr[begin];
        for(int i = begin; i < end; i++){
            if (arr[i] > max) max = arr[i];
        }
        return max;
    }

    float avg(int begin, int end){
        if(Check_begin_end(begin,end) != 0) return -1;
        float sum = 0;
        for(int i = begin; i < end; i++){
            sum+=arr[i];
        }
        return sum / (end-begin);
    }

    //slice
    T* slice (int begin, int end) {
        if(Check_begin_end(begin,end) != 0) return 0;
        int len = end-begin+1;
        T * array[len];
        for (int i = 0; i < len; i++) {
            array[i] = &arr[i];
            std::cout << *array[i] << " ";
        }
        std::cout << std::endl;
        return *array;
    }

private:
    std::vector<T> arr;
    int size;
    std::vector<int> shape;
};

template<typename T>
std::ostream &operator <<(std::ostream& output, NDArray<T>& arr)
{
    for (int i = 0; i < arr.GetSize(); i ++) {
        output << arr.GetItem(i) << ' ';
    }
    return output;
}


int main() {
    //basic func and etc...
    NDArray<int> arr(std::vector<int>{3,3});
    std::cout << "Size: " << arr.GetSize() << std::endl;
    std::cout << "Ndim: " << arr.ndim() << std::endl;
    arr.zeroes();
    std::cout << "Fill array with 0: " << arr << std::endl;
    arr.ones();
    std::cout << "Fill array with 1: " << arr << std::endl;
    arr.random();
    std::cout << "Fill array with random numbers: " << arr << std::endl;

    std::cout << "\n=======================================\n" << std::endl;

    NDArray<int> res(std::vector<int>{3,3});
    NDArray<int> arr1(std::vector<int>{3,3});
    arr1.ones();
    NDArray<int> arr2(std::vector<int>{3,3});
    arr2.ones();
    std::cout << "First array: " << arr1 << std::endl;
    std::cout << "Second array: " << arr2 << std::endl;
    res = arr1 + arr2;
    std::cout << "arr1 + arr2: " << res << std::endl;
    res = arr1 - arr2;
    std::cout << "arr1 - arr2: " << res << std::endl;
    std::cout << "fill arr1.random() and arr2.zeroes().\n";
    arr1.random(); arr2.zeroes();
    res = arr1 * arr2;
    std::cout << "arr1 * arr2: " << res << std::endl;
    std::cout << "arr2.ones().\n";
    arr2.ones();
    res = arr1 / arr2;
    std::cout << "arr1 / arr2: " << res << std::endl;

    std::cout << "\n=======================================\n" << std::endl;

    //matmul
    int array1[6]{1,2,3,4,5,6};
    NDArray<int> arr1_1(std::vector<int>{2,3}, array1);
    NDArray<int> arr2_2(std::vector<int>{3,2}, array1);
    std::cout << "Array1:\n";
    arr1_1.shapePrint();
    std::cout << "Array2:\n";
    arr2_2.shapePrint();
    std::cout << "Matmul array:\n";
    arr1_1.matmul(arr2_2);
    arr1_1.shapePrint(); //[[22,28], [49,64]]

    std::cout << "\n=======================================\n" << std::endl;

    //transpose
    int array2[8]{2,6,3,7,4,8,5,9};
    NDArray<int> brr(std::vector<int>{4,2}, array2);
    std::cout << "Basic Array: " << std::endl;
    brr.shapePrint();
    std::cout << "Transpose Array: " << std::endl;
    brr.Tmatrix();
    brr.shapePrint();

    std::cout << "\n=======================================\n" << std::endl;

    //min,max,avg without range
    int array3[8]{11,6,3,7,4,8,5,9};
    NDArray<int> crr(std::vector<int>{4,2}, array3);
    std::cout << "Basic Array: " << std::endl;
    crr.shapePrint();
    std::cout << "Min elem in Array: " <<  crr.min() << std::endl;
    std::cout << "Max elem in Array: " << crr.max() << std::endl;
    std::cout << "Avg elems in Array: " << crr.avg() << std::endl;

    std::cout << "\n=======================================\n" << std::endl;

    //min,max,avg with range
    int array[9]{11,6,3,7,4,8,5,9, 10};
    NDArray<int> drr(std::vector<int>{3,3}, array);
    std::cout << "Basic Array: " << std::endl;
    drr.shapePrint();
    std::cout << "Min arr elem in range(0-3): " <<  drr.min(0,3) << std::endl;
    std::cout << "Max arr elem in range(3-6): " << drr.max(3,6) << std::endl;
    std::cout << "Avg arr elems in range(6-9): " << drr.avg(6,9) << std::endl;

    std::cout << "\n=======================================\n" << std::endl;

    //slice
    int array4[9]{11,6,3,7,4,8,5,9, 10};
    NDArray<int> err(std::vector<int>{3,3}, array4);
    std::cout << "Basic Array: " << std::endl;
    err.shapePrint();

    std::cout << "Arr slice: ";
    int *i = err.slice(0,2);
    std::cout << "Change first row for 0,1,2 with slice.\n";
    for(int j = 0; j < 3; j ++) {
        i[j] = j;
    }

    std::cout << "Changed Array: " << std::endl;
    err.shapePrint();
    return 0;
}