#include "geneBank.h"

// =================================== PUBLIC FUNCTIONS =======================================================

GENE_BANK::GENE_BANK() 
{
    this->fileSize = 0;
    this->entryByte = sizeof(Sample);
}

GENE_BANK::~GENE_BANK() 
{
    cout << "Destructing ..." << endl;
}

void GENE_BANK::sort(Sample array[], int fileSize) //sorts array by species code
{
    // complete this
    int numElements = fileSize / sizeof(Sample);
    int size0, size1, size2, size3, size4;
    for (int i = 0; i < numElements; i++)
    {
        if ( array[i].speciesCode == 0)
        {
            size0++;
        }
        else if ( array[i].speciesCode == 1)
        {
            size1++;
        }
        else if ( array[i].speciesCode == 2)
        {
            size2++;
        }
        else if ( array[i].speciesCode == 3)
        {
            size3++;
        }
        else
        {
            size4++;
        }
    }//now we know how many of each species code are in the sorted list, so we can sort with 1 pass
    int indexArray[5]; //stores working index we insert at for each value
    indexArray[0] = 0;
    indexArray[1] = size0;
    indexArray[2] = indexArray[1] + size1;
    indexArray[3] = indexArray[2] + size2;
    indexArray[4] = indexArray[3] + size3;

    Sample temp;
    for (int i = 0; i < numElements - size4; i++) //small optimization. Once we've sorted all 3s, the 4s are guaranteed to be sorted
    {
        if (array[i].speciesCode == 0)
        {
            temp = array[indexArray[0]];
            array[indexArray[0]] = array[i];
            array[i] = temp; //swap values
            
            while(array[indexArray[0]].speciesCode == 0 )
            {
                indexArray[0]++; //now we insert 0s at 1 index heigher
            }
        }
        else if (array[i].speciesCode == 1)
        {
            temp = array[indexArray[1]];
            array[indexArray[1]] = array[i];
            array[i] = temp; //swap values
            
            while(array[indexArray[1]].speciesCode == 0 )
            {
                indexArray[1]++; //now we insert 0s at 1 index heigher
            }
        }
        else if (array[i].speciesCode == 2)
        {
            temp = array[indexArray[2]];
            array[indexArray[2]] = array[i];
            array[i] = temp; //swap values
            
            while(array[indexArray[2]].speciesCode == 0 )
            {
                indexArray[2]++; //now we insert 0s at 1 index heigher
            }
        }
        else if (array[i].speciesCode == 3)
        {
            temp = array[indexArray[3]];
            array[indexArray[3]] = array[i];
            array[i] = temp; //swap values
            
            while(array[indexArray[3]].speciesCode == 0 )
            {
                indexArray[3]++; //now we insert 0s at 1 index heigher
            }
        }
        else //array[i] == 4
        {
            temp = array[indexArray[4]];
            array[indexArray[4]] = array[i];
            array[i] = temp; //swap values
            
            while(array[indexArray[4]].speciesCode == 0 )
            {
                indexArray[4]++; //now we insert 0s at 1 index heigher
            }
        }
    }
    
}

void GENE_BANK::indexSamples(Sample array[], int indexArray[]) 
{
    this->p_index(array, indexArray);
}

void GENE_BANK::displayResearcher(int speciesCode, int offset, char* filename) 
{
    bool checkSample;

    checkSample = searchSample(speciesCode, offset, filename);

    if(checkSample){
        p_displayResearcher(speciesCode, offset, filename);
    } else {
        cout << "Sample record doesn't exist! Can't display researcher name." << endl;
    }
}

bool GENE_BANK::searchSample(int speciesCode, int offset, char* filename) 
{
    fstream search;
    search.open(filename, ios::in|ios::binary);

    Sample temp;
    search.seekg(speciesCode + ((offset - 1) * sizeof(Sample))); //move read pointer with seekg
    search.read(reinterpret_cast<char*>(&temp), sizeof(temp));
    
    if (temp.researcher == "" && temp.sampleID == 0)//if no valid research or ID found
    {
        return false;
    }
    return true;
}


void GENE_BANK::updateResearcher(int speciesCode, int offset, char* newName, char* filename) 
{
    bool checkSample;

    checkSample = this->searchSample(speciesCode, offset, filename);

    if(checkSample){
        p_updateResearcher(speciesCode, offset, newName, filename);
    } else {
        cout << "Sample record to be updated doesn't exist!" << endl;
        p_updateResearcher(speciesCode, offset, newName, filename);
    }
}

void GENE_BANK::deleteSample(int speciesCode, int offset, char* filename)
{
    bool checkSample;

    checkSample = this->searchSample(speciesCode, offset, filename);

    if(checkSample){
        p_deleteSample(speciesCode, offset, filename);
    } else {
        cout << "Sample record to be deletesd doesn't exist!" << endl;
    }
}

void GENE_BANK::printSampleRange(int speciesCode, int startIndex, int endIndex, char* filename) 
{
    if(startIndex >= endIndex)
        throw MyException("ERROR: start index is larger than end index!");
    else
        this->p_printRange(speciesCode, startIndex, endIndex, filename);
}

// =================================== PRIVATE FUNCTIONS =======================================================


void GENE_BANK::p_index(Sample array[], int indexArray[]) 
{
    int numElements = fileSize / sizeof(Sample);
    int size0, size1, size2, size3, size4;

    for (int i = 0; i < numElements; i++)
    {
        if ( array[i].speciesCode == 0)
        {
            size0++;
        }
        else if ( array[i].speciesCode == 1)
        {
            size1++;
        }
        else if ( array[i].speciesCode == 2)
        {
            size2++;
        }
        else if ( array[i].speciesCode == 3)
        {
            size3++;
        }
        else
        {
            size4++;
        }
    }

    indexArray[0] = 0;
    indexArray[1] = (size0-1) * sizeof(Sample);
    indexArray[2] = indexArray[1] + ((size1-1) * sizeof(Sample));
    indexArray[3] = indexArray[2] + ((size2-1) * sizeof(Sample));
    indexArray[4] = indexArray[3] + ((size3-1) * sizeof(Sample));

}

void GENE_BANK::p_displayResearcher(int speciesCode, int offset, char* filename) 
{//int speciec code is passed as indexarray[i], offset is which number entry from section
    /* These are the 5 unique species codes
        0 - H_SAP
        1 - M_MUS
        2 - D_MEL
        3 - E_COL
        4 - A_THA
    */

    fstream displayResearcher;
    displayResearcher.open(filename, ios::in|ios::binary);

    Sample temp;
    displayResearcher.seekg(speciesCode + ((offset-1)* sizeof(Sample))); //move read pointer with seekg
    displayResearcher.read(reinterpret_cast<char*>(&temp), sizeof(temp));
    printf("Researcher with code: %d, offset %d: %s", speciesCode, offset, temp.researcher);
}

void GENE_BANK::p_updateResearcher(int speciesCode, int offset, char* newName, char* filename) 
{
    fstream update;

    update.open(filename, ios::in|ios::binary);

    Sample temp;
    update.seekg(speciesCode + ((offset - 1) * sizeof(Sample)));
    update.read(reinterpret_cast<char*>(&temp), sizeof(temp));

    strncpy(temp.researcher, newName, 20); //using strncpy to convert from char* to char[20]

    update.seekp(speciesCode + ((offset-1)* sizeof(Sample)));
    update.write(reinterpret_cast<char*>(&temp), sizeof(temp));;
    update.close();
}

void GENE_BANK::p_deleteSample(int speciesCode, int offset, char* filename) 
{
    fstream update;

    update.open(filename, ios::in|ios::binary);

    Sample temp;
    update.seekg(speciesCode + ((offset - 1) * sizeof(Sample)));
    update.read(reinterpret_cast<char*>(&temp), sizeof(temp));

    temp.sampleID = -1;
    //temp.researcher = "\0";
    temp.purityScore = -1;

    update.seekp(speciesCode + ((offset-1)* sizeof(Sample)));
    update.write(reinterpret_cast<char*>(&temp), sizeof(temp));;
    update.close();
}

void GENE_BANK::p_printRange(int speciesCode, int startIndex, int endIndex, char* filename) 
{
    fstream print;

    print.open(filename, ios::in|ios::binary);
    Sample temp;

    for (int i = startIndex; i < endIndex + 1; i++)
    {
        print.seekg(speciesCode + ((startIndex + i - 1) * sizeof(Sample))); //move to position of each entry in range
        print.read(reinterpret_cast<char*>(&temp), sizeof(temp)); //read into temp
        cout << "entry at index " << startIndex + i << ": researcher is " << temp.researcher << " with ID " << temp.sampleID << " and purity score " << temp.purityScore << endl;
    }
}
