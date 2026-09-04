#include "../include/sort.h"

void Sort::bubbleSortByPlayCount(Song arr[], int size){
    for (int i = 0; i < size - 1; i++){
        for (int j = 0; j < size - i - 1; j++){
            if (arr[j].getCount() > arr[j + 1].getCount()){
                Song temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}