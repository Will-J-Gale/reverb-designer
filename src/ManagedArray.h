/*
  ==============================================================================

    ManagedArray.h
    Created: 13 Sep 2020 3:19:45pm
    Author:  Will

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <memory>

class ManagedArray
{
protected:

    template<class T>
    void deleteArray(Array<T*>& arr)
    {
        while (arr.size() > 0)
        {
            int index = arr.size() - 1;
            delete arr[index];

            arr.remove(index);
        }

        arr.clear();
    }

    template<class T>
    void deleteFromArray(Array<T*>& arr, T* item)
    {
        int index = arr.indexOf(item);
        delete arr[index];
        arr.remove(index);
    }

    template<class T>
    void removeFromArray(Array<T*>& arr, T* item)
    {
        int index = arr.indexOf(item);
        arr.remove(index);
    }

    template<class T>
    void removeFromArray(Array<T>& arr, T item)
    {
        int index = arr.indexOf(item);
        arr.remove(index);
    }

    template<class T>
    void removeFromArray(Array<std::shared_ptr<T>>& arr, T* item)
    {
        int index = -1;

        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i].get() == item)
            {
                index = i;
                break;
            } 
        }

        arr.remove(index);
    }

    template<class T>
    bool arrayContains(Array<std::shared_ptr<T>>& arr, T* item)
    {
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i].get() == item)
            {
                return true;
            }
        }

        return false;
    }
};